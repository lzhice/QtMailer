#ifndef MAILER_H
#define MAILER_H

#include <QObject>
#include <QList>
#include <QString>
#include <deque>
#include <QStringList>
#include <utility>
#include <QSslSocket>
#include <QHostInfo>
#include <QEventLoop>
#include <QSslError>

#include "mail.h"

#define SMTPPORT 25
#define SMTPTIMEOUT 30000

#define ERROR_UNENCCONNECTIONNOTPOSSIBLE    "Could not connect to server"
#define ERROR_ENCCONNECTIONNOTPOSSIBLE      "Could not connect to server encrypted"

class Mailer : public QObject
{
    Q_OBJECT

    /// Defines the different states of the SMTP connection
    enum SMTP_States {
        Disconnected,
        Connected,
        EHLOsent,
        MAILFROMsent,
        TOsent,
        DATAsent,
        CONTENTsent,
        QUITsent,
        RSETsent,
        AUTH
    };

    /// Defines the different states of the SMTP-login
    enum SMTP_Login_State {
        PRELOGIN,
        AUTHLOGINsent,
        USERNAMEsent,
        PASSWORDsent
    };

    /// Defines if the STARTTLS is already send to the server (when using STARTTLS)
    enum STARTTLSstate{
       preSTARTTLS,
       postSTARTTLS
    };

public:
    /// Defines if the commnuication with the server should be encrypted or not
    enum ENCRYPTION{
        UNENCRYPTED,
        STARTTLS,
        SSL
    };

    /// Defines if the server needs authentication
    enum SMTP_Auth_Method {
        LOGIN,
        NO_Auth
    };

    explicit Mailer(const QString &server, QObject *parent = 0);

    int                     sizeOfQueue() const;
    bool                    sendAllMails();
    void                    enqueueMail(const Mail& mail);
    QString                 getServer() const;
    void                    setServer(const QString &value);
    bool                    isBusy();
    void                    waitForProcessing();
    std::pair<int,int>      lastErrors() const;
    int                     getSmtpPort() const;
    void                    setSmtpPort(int value);
    int                     getSmtpTimeout() const;
    void                    setSmtpTimeout(int value);
    void                    setAUTHMethod(SMTP_Auth_Method);
    void                    setPassword(const QString &value);
    void                    setUsername(const QString &value);
    void                    setEncryptionUsed(const ENCRYPTION &value);
    void					ignoreSelfSignedCertificates(bool ignore = true);

protected:
    QString             server;
    QSslSocket*         socket{nullptr};
    QTextStream         socketStream;
    bool                isConnected{false};
    SMTP_States         currentState{Disconnected};
    std::deque<Mail>    mailqueue;
    Mail*               processedMail{nullptr};
    int                 recepientsSent{0};
    int                 mailsProcessed{0};
    int                 mailsToSend{0};
    int                 tempErrors{0};
    int                 permErrors{0};
    int                 smtpPort{SMTPPORT};
    int                 smtpTimeout{SMTPTIMEOUT};
    SMTP_Auth_Method    authMethodToUse{NO_Auth};
    ENCRYPTION          encryptionUsed{UNENCRYPTED};
    SMTP_Login_State    loginState{PRELOGIN};
    STARTTLSstate       startTLSstate{preSTARTTLS};
    QString             username;
    QString             password;
    bool				ignoreSelfSigned{false};

    bool                connectToServer();
    void                disconnectFromServer();
    void                sendAUTHLOGIN();
    void                sendSTARTTLS();
    void                sendEHLO();
    void                sendMAILFROM();
    void                sendTO();
    void                sendDATA();
    void                sendMessagecontent();
    void                sendQUIT();
    void                sendRSET();
    void                sendNextMailOrQuit();
    void                mailProcessed();
    QString             pureMailaddressFromAddressstring(const QString &addressstring);
    bool                validPureMailaddress(const QString& address);
    bool                validDecoratedAddress(const QString& address);

signals:
    void finishedSending(bool queueEmpty);
    void errorSendingMails(int smtpErrorcode, QString smtpErrorstring);
    void mailsHaveBeenProcessedTillNow(int numberOfMailsProcessed);

public slots:
    void     cancelSending();

protected slots:
    void    dataReadyForReading();
    void    errorReceived(QAbstractSocket::SocketError);
    void    sslErrorsReceived(QList<QSslError>);

public slots:

};

#endif // MAILER_H
