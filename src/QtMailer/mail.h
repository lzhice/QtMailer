#ifndef MAIL_H
#define MAIL_H

#include <QObject>
#include <QList>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QRegExp>
#include <QDebug>
#include <QMimeType>
#include <QMimeDatabase>
#include <QFileInfo>
#include <utility>

#define MAXLINESIZE 78
#define BOUNDARY    "mXysXimXplXebXouXndXarXy"

class Mail : public QObject
{
    Q_OBJECT
public:
    explicit Mail(QStringList toRecepients,
                  QStringList ccRecepients,
                  QStringList bccRecepients,
                  QString sender,
                  QString subject,
                  QString body,
                  QList<QFileInfo> attachments,
                  QObject *parent = 0);
    explicit Mail(QStringList toRecepients,
                  QStringList ccRecepients,
                  QStringList bccRecepients,
                  QString sender,
                  QString subject,
                  QString body,
                  QFileInfo attachment,
                  QObject *parent = 0);
    explicit Mail(QStringList toRecepients,
                  QStringList ccRecepients,
                  QStringList bccRecepients,
                  QString sender,
                  QString subject,
                  QString body,
                  QObject *parent = 0);
    explicit Mail(QStringList toRecepients,
                  QString sender,
                  QString subject,
                  QString body,
                  QList<QFileInfo> attachments,
                  QObject *parent = 0);
    explicit Mail(QStringList toRecepients,
                  QString sender,
                  QString subject,
                  QString body,
                  QFileInfo attachments,
                  QObject *parent = 0);
    explicit Mail(QStringList toRecepients,
                  QString sender,
                  QString subject,
                  QString body,
                  QObject *parent = 0);
    explicit Mail(QString toRecepient,
                  QString sender,
                  QString subject,
                  QString body,
                  QObject *parent = 0);
    explicit Mail(QString toRecepient,
                  QString sender,
                  QString subject,
                  QString body,
                  QFileInfo attachment,
                  QObject *parent = 0);
    Mail(const Mail& other);

    QString             plaintextMail() const;
    QString             getSender() const;
    QStringList         getAllRecepients() const;
    QStringList         getToRecepients() const;
    QStringList         getCcRecepients() const;
    QStringList         getBccRecepients() const;
    std::pair<int,int>  lastErrors() const;

protected:
    QStringList         toRecepients;
    QStringList         ccRecepients;
    QStringList         bccRecepients;
    QString             sender;
    QString             subject;
    QString             body;
    QList<QFileInfo>    attachments;

    QString         generateBase64FromFile(const QFileInfo&) const;
    QString         foldString(const QString& original) const;
    QString         mimetypeForFile(const QFileInfo& file) const;
    QString         recepientHeaderLineFromStringList(QString header, QStringList addresses) const;

signals:

public slots:

};

#endif // MAIL_H
