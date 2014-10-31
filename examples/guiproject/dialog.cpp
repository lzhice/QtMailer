#include "dialog.h"
#include "../../src/QtMailer/mail.h"
#include "../../src/QtMailer/mailer.h"
#include "QList"
#include "QFileInfo"
#include <QEventLoop>
#include <QApplication>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
{
    // To send mails over an open SMTP-server is easy

    // First create an Mailer-object on the heap (it MUST be on the heap).
    Mailer* mailer = new Mailer("mail.example.com", this);

    // next connect the SIGNALS you need
    connect(mailer, SIGNAL(finishedSending(bool)),              this, SLOT(finishedSending(bool)) ) ;
    connect(mailer, SIGNAL(mailsHaveBeenProcessedTillNow(int)), this, SLOT(mailsBeeingProcessed(int)));
    connect(mailer, SIGNAL(errorSendingMails(int,QString)),     this, SLOT(errorOccured(int,QString)));

    // The easiest way to create a pure mail without atttachments
    mailer->enqueueMail(
                        Mail(
                             "Your Name <you@example.com>",         // Receipient
                             "My Name <me@example.com>",            // Sender
                             "My Subject",                          // Subject
                             "And the mailcontent I'd like to send" // Content
                            )
                        );

    // If you'd like to enque a mail with an attachnemt
    mailer->enqueueMail(
                        Mail(
                            "you@example.com",                      // Receipient
                            "me@example.com",                       // Sender
                            "Now with Attachment",                  // Subject
                            "Here is the attachment you wanted",    // Content
                             QFileInfo("File.txt")                  // Attachment
                            )
                        );

    // To get a statusdialog which shows the progress of sending and lets us
    // resend mails if there are errors we create one on the heap
    MailerStatus* status = new MailerStatus(
                                             mailer,    // pointer to the Mailer-object
                                             false,     // if true the dialog gets closed after sucessfull sending
                                             this       // a parent as known from Qt
                                             );

    // And than... Just send all this stuff...
    mailer->sendAllMails();


    // You can even halt the program till the mailer has finished the work to do.
    mailer->waitForProcessing();


    // Next we want so send mails over SSL-encrypted TCP-connection with a
    // Login using a username and password (STARTTLS is not supported atm,
    // and PLAIN as AUTH-Method isn't supported neither).
    mailer->setEncryptionUsed(Mailer::ENCRYPTION::SSL);
    mailer->setAUTHMethod(Mailer::SMTP_Auth_Method::LOGIN);
    mailer->setSmtpPort(465);
    mailer->setUsername("myUsername");
    mailer->setPassword("myReallyVerySecretPassword");

    // And again enqueue a mail..
    mailer->enqueueMail(
                        Mail(
                             "you@example.com",                     // Receipient
                             "me@example.com",                      // Sender
                             "My encrypted mail",                   // Subject
                             "And the mailcontent I'd like to send" // Content
                            )
                        );

    // and if we start sending using the status dialog, the dialog will be
    // refreshed as needed
    status->startSending();


    // OK, but we can also send mails over encrypted connections using STARTTLS.
    // We still use LOGIN als AUTH-Method.
    mailer->setEncryptionUsed(Mailer::ENCRYPTION::STARTTLS);
    mailer->setAUTHMethod(Mailer::SMTP_Auth_Method::LOGIN);
    mailer->setServer("smtp.example.com");  // we can connect an other server.
    mailer->setSmtpPort(587);               // and we want an other port to use.

    // And again enqueue a mail..
    mailer->enqueueMail(
                        Mail(
                             "Your Name <you@example.com>",         // Receipient
                             "My Name <me@example.com>",            // Sender
                             "Encrypted using STARTLS",             // Subject
                             "Strangers in the night."              // Content
                            )
                        );

    // and again we kick the shit out
    status->startSending();
}

Dialog::~Dialog()
{

}

void Dialog::finishedSending(bool empty)
{
    qDebug() << QString("Mailer has finished. Mailqueue is ").append(empty ? "" : "not").append("empty");
}

void Dialog::mailsBeeingProcessed(int count)
{
    qDebug() << QString::number(count).append(" mail").append(count == 1 ? "" : "s").append(" have been processed till now.");
}

void Dialog::errorOccured(int smtpFehlercode, QString fehlerText)
{
    qDebug() << QString("During transmission ").
                         append(smtpFehlercode < 2 ? "" : "SMTP-").
                         append("Error ").
                         append(smtpFehlercode < 2 ? QString() : QString::number(smtpFehlercode)+" ").
                         append("\"" + fehlerText + "\" ").
                         append("happened.");
}
