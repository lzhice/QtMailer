#include "mailer.h"

Mailer::Mailer(const QString &server, QObject *parent) :
    QObject(parent), server{server}
{
    socket = new QSslSocket(this);
    connect(
             socket,
             SIGNAL(readyRead()),
             this,
             SLOT(dataReadyForReading())
             );
    connect(
            socket,
            SIGNAL( error(QAbstractSocket::SocketError)),
            this,
            SLOT(errorReceived(QAbstractSocket::SocketError))
            );

    connect(
            socket,
            SIGNAL(sslErrors(QList<QSslError>)),
            this,
            SLOT(sslErrorsReceived(QList<QSslError>))
            );
}

/**
 * @brief Returns the number of mails beeing held in the mailqueue
 * @return the size oh the mailqueue
 */
int Mailer::sizeOfQueue() const
{
    return mailqueue.size();
}

/**
 * Starts the process of sending all mails in the mailqueue
 *
 * @return false if mailqueue is empty or the mailer is alredy busy
 */
bool Mailer::sendAllMails()
{
    // Only start sending if we aren't busy
    if (currentState != Disconnected)   return false;
    // don't send if we have no mails.
    if (mailqueue.size() == 0 )         return false;

    mailsToSend = mailqueue.size();

    // And the magic begins...
    if (!connectToServer())             return false;
    return true;
}

/**
 * External interface to cancel the sending of mails.
 *
 * Sends QUIT to the server and then closes the connection.
 */
void Mailer::cancelSending()
{
    sendQUIT();
}

/**
 * Pushes a mailobject to the end of the mailqueue
 *
 * @param mail  mailobject to enqueue
 */
void Mailer::enqueueMail(const Mail &mail)
{
    mailqueue.push_back(mail);
}

/**
 * Returns the currently set mailserver
 *
 * @return the current mailserverstring
 */
QString Mailer::getServer() const
{
    return server;
}

/**
 * Sets the mailserver to use with the future sendingAllMails()
 * @param value newMailserver to use
 */
void Mailer::setServer(const QString &value)
{
    server = value;
}

/**
 * Returns true if the mailer ist currently connected to a mailserver
 *
 * Only if the mailer is noch connected sendingAllMails() can be started.
 *
 * @return if mailer ist busy
 */
bool Mailer::isBusy()
{
    if (currentState == Disconnected) return false;
    return true;
}

/**
 * Returns after when the mailer ist not busy anymore.
 */
void Mailer::waitForProcessing()
{
    QEventLoop loop;
    while (isBusy()){
        loop.processEvents();
    }
}

/**
 * Connects to the currently set mailserver
 *
 * can only be invokes if the mailer isn't busy at the moment.
 *
 * @return true if the connection is established
 */
bool Mailer::connectToServer()
{
    if (currentState != Disconnected) return false;
    tempErrors      = 0;
    permErrors      = 0;

    switch (encryptionUsed){
        case SSL :
                    socket->connectToHostEncrypted(server, smtpPort);
                    if (!socket->waitForEncrypted(smtpTimeout)){
                        emit errorSendingMails(0, ERROR_ENCCONNECTIONNOTPOSSIBLE);
                        return false;
                    }
                    break;
        case STARTTLS    :
        case UNENCRYPTED :
                    socket->connectToHost(server, smtpPort);
                    if (!socket->waitForConnected(smtpTimeout)){
                        emit errorSendingMails(0, ERROR_UNENCCONNECTIONNOTPOSSIBLE);
                        return false;
                    }
                    break;
        }
    socketStream.setDevice(socket);
    currentState = Connected;
    return true;
}

/**
 * Disconnects from the server and emits finishedSending()
 */
void Mailer::disconnectFromServer()
{
    if (currentState == Disconnected) return;
    socket->disconnectFromHost();
    mailsProcessed  =   0;
    mailsToSend     =   0;
    recepientsSent  =   0;
    loginState      =   PRELOGIN;
    emit finishedSending( mailqueue.size() == 0 ? true : false);
    currentState = Disconnected;
}

void Mailer::sendAUTHLOGIN()
{
    currentState = AUTH;
    QString sendstring;
    switch (loginState){
        case PRELOGIN       :
                                sendstring = "AUTH LOGIN\r\n";
                                loginState = AUTHLOGINsent;
                                break;
        case AUTHLOGINsent       :
                                sendstring = username.toLocal8Bit().toBase64()+"\r\n";
                                loginState = USERNAMEsent;
                                break;
        case USERNAMEsent   :
                                sendstring = password.toLocal8Bit().toBase64()+"\r\n";
                                loginState = PASSWORDsent;
                                break;
        case PASSWORDsent   :
                                sendMAILFROM();
                                loginState = PRELOGIN;
                                break;

    }

#ifdef DEBUG
    qDebug() << "Sending: " << sendstring.left(sendstring.size()-2);;
#endif
    socketStream << sendstring;
    socketStream.flush();
}

void Mailer::sendAUTHLOGINuser()
{
    QString sendstring = username.toLocal8Bit().toBase64()+"\r\n";
#ifdef DEBUG
    qDebug() << "Sending: " << sendstring.left(sendstring.size()-2);;
#endif
    socketStream << sendstring;
    socketStream.flush();
    currentState = AUTH;
}

void Mailer::sendAUTHLOGINpassword()
{
    QString sendstring = password.toLocal8Bit().toBase64()+"\r\n";
#ifdef DEBUG
    qDebug() << "Sending: " << sendstring.left(sendstring.size()-2);;
#endif
    socketStream << sendstring;
    socketStream.flush();
    currentState = AUTH;
}

/**
 * Sends EHLO to the SMTP-server
 */
void Mailer::sendEHLO()
{
    QString sendstring = "EHLO " + QHostInfo::localHostName() + "\r\n";
#ifdef DEBUG
    qDebug() << "Sending: " << sendstring.left(sendstring.size()-2);;
#endif
    socketStream << sendstring;
    socketStream.flush();
    currentState = EHLOsent;
}

/**
 * Sends MAIL FROM: to the SMTP-server
 */
void Mailer::sendMAILFROM()
{
    QString sendstring = "MAIL FROM:<" + mailqueue.front().getSender() + ">\r\n";
#ifdef DEBUG
    qDebug() << "Sending: " << sendstring.left(sendstring.size()-2);;
#endif
    socketStream << sendstring;
    socketStream.flush();
    currentState = MAILFROMsent;
}

/**
 * Sends RCPT TO: to the SMTP-server
 */
void Mailer::sendTO()
{
    QString sendstring = "RCPT TO:<" +
                         mailqueue.front().getAllReceipients().at(recepientsSent++) +
                         ">\r\n";
#ifdef DEBUG
    qDebug() << "Sending: " << sendstring.left(sendstring.size()-2);;
#endif
    socketStream << sendstring;
    socketStream.flush();
    if (recepientsSent == mailqueue.front().getAllReceipients().size()) {
        recepientsSent = 0;
        currentState = TOsent;
    }
}

/**
 * Sends DATA to the SMTP-server
 */
void Mailer::sendDATA()
{
    QString sendstring = "DATA\r\n";
#ifdef DEBUG
    qDebug() << "Sending: " << sendstring.left(sendstring.size()-2);;
#endif
    socketStream << sendstring;
    socketStream.flush();
    currentState = DATAsent;
}

/**
 * Sends the messagecontent to the SMTP-server
 */
void Mailer::sendMessagecontent()
{
    QString sendstring = mailqueue.front().plaintextMail();
#ifdef DEBUG
    qDebug() << "Sending: " << sendstring.left(sendstring.size()-2);;
#endif
    socketStream << sendstring;
    socketStream.flush();
    currentState = CONTENTsent;
}

/**
 * Sends QUIT to the SMTP-server
 */
void Mailer::sendQUIT()
{
    QString sendstring = "QUIT\r\n";
#ifdef DEBUG
    qDebug() << "Sending: " << sendstring.left(sendstring.size()-2);
#endif
    socketStream << sendstring;
    socketStream.flush();
    currentState = QUITsent;
}

/**
 * Sends RSET to the SMTP-server
 */
void Mailer::sendRSET()
{
    QString sendstring = "RSET\r\n";
#ifdef DEBUG
    qDebug() << "Sending: " << sendstring.left(sendstring.size()-2);
#endif
    socketStream << sendstring;
    socketStream.flush();
    currentState = RSETsent;
}

/**
 * Invokes sendQUIT() to QUIT the session if all mails in the queue are processed once.
 * If not all mails are processed sendEHLO() starts the sending process for
 * next mail.
 */
void Mailer::sendNextMailOrQuit()
{
    if (mailsProcessed >= mailsToSend){
        sendQUIT();
    } else {
        sendMAILFROM();
    }
}

/**
 * Pops the first element from the mailqueue and emits mailsHaveBeenProcessedTillNow()
 * and increments mailsProcesses by 1
 */
void Mailer::mailProcessed()
{
    if (mailqueue.size() > 0){
        mailqueue.pop_front();
        mailsProcessed++;
        emit mailsHaveBeenProcessedTillNow(mailsProcessed);
    }
}

/**
 * Is called every time when there is new data ready to read on the tcp-socket
 *
 * Is called by connect from socket::readyRead()
 */
void Mailer::dataReadyForReading()
{
    QString replyCode;
    while(socket->canReadLine()){   // for multiline replys
        replyCode = socketStream.readLine();
#ifdef DEBUG
    qDebug() << "Received: " << replyCode;
#endif
    }
    replyCode.truncate(3);

    switch (replyCode.at(0).toLatin1()){
        case '5'    :   // Permanent error => The mail will be lost...
                        permErrors++;
                        emit errorSendingMails(replyCode.toInt(), socket->errorString());
                        mailProcessed();
                        qDebug() << "Permanent error: " <<  socket->errorString();
                        sendRSET();
                        return;
                        break; // Just in case...
        case '4'    :   // Transient error => The mail will be enqueued again
                        tempErrors++;
                        mailqueue.push_back(mailqueue.front());
                        mailProcessed();
                        sendRSET();
                        emit errorSendingMails(replyCode.toInt(), socket->errorString());
                        qDebug() << "Temporary error: " << socket->errorString();
                        return;
                        break; // Just in case...
        case '3'    :   // Positive intermediate reply => Wonderful nothing to do.
                        break;
        case '2'    :   // Positive completion reply => Wonderful nothing to do.
                        break;
    }

    switch (currentState){

        case Disconnected   :
        case Connected      :
                                sendEHLO();
                                break;
        case EHLOsent       :
                                if (authMethodToUse == NO_Auth)
                                    sendMAILFROM();
                                else if (authMethodToUse == LOGIN)
                                    sendAUTHLOGIN();
                                break;
        case AUTH           :   sendAUTHLOGIN();
                                break;
        case MAILFROMsent   :   sendTO();
                                break;
        case TOsent         :   sendDATA();
                                break;
        case DATAsent       :   sendMessagecontent();
                                break;
        case CONTENTsent    :
                                mailProcessed();
                                sendNextMailOrQuit();
                                break;
        case RSETsent       :
                                sendNextMailOrQuit();
                                break;
        case QUITsent       :   disconnectFromServer();
                                break;

    }
}

/**
 * @brief Mailer::errorReceived
 *
 * This method is called if there comes an error from the tcp-socket, in this
 * case we close the connection and emit errorSendingMails with the
 * fake-smtp-error code 0
 */
void Mailer::errorReceived(QAbstractSocket::SocketError)
{
    //After having disconnected we receive a host closed the session error.
    // which is not a real error due to the fact that the session is already closed
    if (currentState == Disconnected) return;

    //closing the session should not overwrite our errorstring
    QString errorString = socket->errorString();

    disconnectFromServer();
    emit errorSendingMails(0, errorString);
}

/**
 * @brief Mailer::errorReceived
 *
 * This method is called if there are ssl-errors from the tcp-socket, in this
 * case we close the connection and emit errorSendingMails with the
 * fake-smtp-error code 1 (for ssl-errors)
 */
void Mailer::sslErrorsReceived(QList<QSslError> errors)
{
    foreach (QSslError error, errors){
        emit errorSendingMails(1, error.errorString());
        // Uncomment to accept ANY SSL-Errors (eg. self signed)... not the best idea.
//        socket->ignoreSslErrors();
    }
    disconnectFromServer();
}

/**
 * Returns the number of mails having errors during last connection.
 *
 * The errors are seperated in temporary errors (which are again enqueued to the mailqueue)
 * and permanent errors (where the mails are removed from the mailqueue due to the fact
 * that those mails can never be send without any administration).
 *
 * @return pair of <tempErrors, permErrors>
 */
std::pair<int, int> Mailer::lastErrors() const
{
    return std::pair<int,int>(tempErrors, permErrors);
}

/**
 * Returns the currently set SMTP-port to use for connection to the server
 * @return smtp-port the server should listen on
 */
int Mailer::getSmtpPort() const
{
    return smtpPort;
}

/**
 * Sets the SMTP-Port the next connection should use for connecting to the server
 * @param value server-port
 */
void Mailer::setSmtpPort(int value)
{
    smtpPort = value;
}

/**
 * Returns the number of milliseconds to timeout a connectionattempt to the mailserver
 * @return timeout
 */
int Mailer::getSmtpTimeout() const
{
    return smtpTimeout;
}

/**
 * Sets the timeout in milliseconds for new established SMTP-sessions
 * @param value timeout in milliseconds
 */
void Mailer::setSmtpTimeout(int value)
{
    if (value < -1 ) return;
    smtpTimeout = value;
}

/**
 * Sets the AUTH-method to use when connecting to the SMTP-server
 * @param method    method to use
 */
void Mailer::setAUTHMethod(Mailer::SMTP_Auth_Method method)
{
    authMethodToUse = method;
}

/**
 * Sets the password to use for AUTH on the SMTP-server
 * @param value password in plantext
 */
void Mailer::setPassword(const QString &value)
{
    password = value;
}

/**
 * Sets the username to use for AUTH on the SMTP-server
 * @param value username in plaintext
 */
void Mailer::setUsername(const QString &value)
{
    username = value;
}

/**
 * Sets the kind of socket-encryption to use
 * @param value encryption to use
 */
void Mailer::setEncryptionUsed(const ENCRYPTION &value)
{
    encryptionUsed = value;
}
