#ifndef MAILERSTATUS_H
#define MAILERSTATUS_H

#include <QDialog>
#include <QPushButton>
#include <QProgressBar>
#include <QLabel>
#include <QGridLayout>
#include <QButtonGroup>
#include <QCloseEvent>

#include "mailer.h"
#include "mailerstatusStrings.h"

class MailerStatus : public QDialog
{
    Q_OBJECT

public:
    explicit MailerStatus(  Mailer* mailer,
                            bool closeOnFinish = true,
                            QWidget *parent = 0
                          );

protected:
    Mailer*         mailer{nullptr};
    QProgressBar*   progressbar;
    QLabel*         label{nullptr};
    QPushButton*    ok{nullptr};
    QPushButton*    retry{nullptr};
    QPushButton*    cancel{nullptr};
    bool            closeOnFinish{true};

protected slots:
    void    mailerFinished();
    void    errorReceived(int, QString);

signals:

public slots:
    void closeEvent(QCloseEvent *event);
    void initalStatus();
    void startSending();

};

#endif // MAILERSTATUS_H
