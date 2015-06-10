#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

#include "mailerstatus.h"

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = 0);
    ~Dialog();

protected slots:
    void finishedSending(bool empty);
    void mailsBeeingProcessed(int count);
    void errorOccured(int smtpErrorcode, QString errorString);
};

#endif // DIALOG_H
