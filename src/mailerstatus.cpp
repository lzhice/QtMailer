#include "mailerstatus.h"

/**
  * @class MailerStatus
  *
  * @brief Gives a little dialog window showing the progress of a Mailer class.
  *
  * When connected to a Mailer object it shows the progress of the processing.
  * It can cancel the session and start a retry if there are mails left in the
  * queue (due to temporary errors). If there are error sending it shows the
  * latest error occured.
  */

/**
 * Constructor
 * @param mailer        Mailer-object to use to send the mails
 * @param closeOnFinish true closes the window after successfull finish
 * @param parent        Qt-parent
 */
MailerStatus::MailerStatus(Mailer *mailer, bool closeOnFinish, QWidget *parent) :
    QDialog(parent), mailer{mailer}, closeOnFinish{closeOnFinish}
{
    setAttribute(Qt::WA_DeleteOnClose);
    setModal(true);
    QGridLayout* layout = new QGridLayout(this);
    this->setLayout(layout);
    label = new QLabel(this);

    progressbar = new QProgressBar(this);
    ok      = new QPushButton(MAILERSTATUS_OKBUTTONTEXT,     this);
    retry   = new QPushButton(MAILERSTATUS_RETRYBUTTONTEXT,  this);
    cancel  = new QPushButton(MAILERSTATUS_CANCELBUTTONTEXT, this);

    layout->addWidget(label         ,0,0,1,3);
    layout->addWidget(progressbar   ,1,0,1,3);
    layout->addWidget(ok            ,2,0);
    layout->addWidget(retry         ,2,1);
    layout->addWidget(cancel        ,2,2);


    connect( mailer,
             SIGNAL(mailsHaveBeenProcessedTillNow(int)),
             progressbar,
             SLOT(setValue(int))
           );
    connect( cancel,
             SIGNAL(clicked()),
             mailer,
             SLOT(cancelSending())
           );
    connect( mailer,
             SIGNAL(finishedSending(bool)),
             this,
             SLOT(mailerFinished())
           );
    connect( mailer,
             SIGNAL(errorSendingMails(int,QString)),
             this,
             SLOT(errorReceived(int, QString))
             );

    connect( ok,
             SIGNAL(clicked()),
             this,
             SLOT(close())
             );
    connect( retry,
             SIGNAL(clicked()),
             this,
             SLOT(startSending())
             );

    initalStatus();

    this->show();
}


/**
 * Sets the inital status for the widgets
 */
void MailerStatus::initalStatus()
{
    ok->setEnabled(false);
    retry->setEnabled(false);
    cancel->setEnabled(true);
    progressbar->setMinimum(0);
    progressbar->setMaximum(mailer->sizeOfQueue());
    progressbar->setValue(0);
    label->setText(MAILERSTATUS_LABELSTDTEXT1 + QString::number(mailer->sizeOfQueue()) +
                   MAILERSTATUS_LABELSTDTEXT2);
}


/**
 * Will be called after the mailer finished sending MailerStatus
 *
 * If the mailqueue of the mailer is empty the retry-button will be disabled, otherwise it is enabled to
 * allow the user to resend the remaining mails.
 * It the mails were sent sucessfully and closeOnFinish is set the widget will be closed.
 */
void MailerStatus::mailerFinished()
{
    cancel->setEnabled(false);
    ok->setEnabled(true);
    retry->setEnabled( (mailer->sizeOfQueue() > 0) ? true : false );

    int tempErrors = mailer->lastErrors().first;
    int permErrors = mailer->lastErrors().second;
    if (closeOnFinish && tempErrors == 0 && permErrors == 0){
        this->close();
        return;
    }

    label->setText(MAILERSTATUS_FINISHEDTEXT +
                   (tempErrors  == 0 ?
                        QString()
                      :
                        QString("\n") + QString::number(tempErrors) +
                        QString(MAILERSTATUS_MAILSWITHTEMPERROR)) +
                   (permErrors == 0 ?
                        QString()
                      :
                        QString("\n") + QString::number(permErrors) +
                        QString(MAILERSTATUS_MAILSWITHPERMERROR)) +
                   (mailer->sizeOfQueue()       == 0 ?
                        QString()
                      :
                        QString("\n") + QString::number(mailer->sizeOfQueue()) +
                        QString(MAILERSTATUS_MAILSLEFTINQUEUE))
                   );
}


/**
 * Starts the mailer to send all mails beeing kept in the mailqueue.
 */
void MailerStatus::startSending()
{
    initalStatus();
    mailer->sendAllMails();
}


/**
 * Slot that will be called if the mailer has any errors.
 *
 * @param errorCode the received errorcode (0 for connection errors, 1 for ssl errors or SMTP errorcode)
 * @param text      an errordescription
 */
void MailerStatus::errorReceived(int errorCode, QString text)
{
    if (errorCode == 0){
        label->setText(MAILERSTATUS_COMMONERRORMESSAGE"\n"+text);
    } else {
        label->setText(MAILERSTATUS_SMTPERRORMESSAGE"\n" + QString::number(errorCode)+" " +text);
    }
    ok->setEnabled(true);
    retry->setEnabled( (mailer->sizeOfQueue() > 0) ? true : false );
    cancel->setEnabled(false);
}


/**
 * Overrides the closeEvent to let the widget only be closed if the ok-button is enabled
 * @param event the closing event
 */
void MailerStatus::closeEvent(QCloseEvent *event)
{
    if (ok->isEnabled()) event->accept();
    else                 event->ignore();
}
