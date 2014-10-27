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
    explicit Mail(QStringList toReceipients,
                  QStringList ccReceipients,
                  QStringList bccReceipients,
                  QString sender,
                  QString subject,
                  QString body,
                  QList<QFileInfo> attachments,
                  QObject *parent = 0);
    explicit Mail(QStringList toReceipients,
                  QStringList ccReceipients,
                  QStringList bccReceipients,
                  QString sender,
                  QString subject,
                  QString body,
                  QFileInfo attachment,
                  QObject *parent = 0);
    explicit Mail(QStringList toReceipients,
                  QStringList ccReceipients,
                  QStringList bccReceipients,
                  QString sender,
                  QString subject,
                  QString body,
                  QObject *parent = 0);
    explicit Mail(QStringList toReceipients,
                  QString sender,
                  QString subject,
                  QString body,
                  QList<QFileInfo> attachments,
                  QObject *parent = 0);
    explicit Mail(QStringList toReceipients,
                  QString sender,
                  QString subject,
                  QString body,
                  QFileInfo attachments,
                  QObject *parent = 0);
    explicit Mail(QStringList toReceipients,
                  QString sender,
                  QString subject,
                  QString body,
                  QObject *parent = 0);
    explicit Mail(QString toReceipient,
                  QString sender,
                  QString subject,
                  QString body,
                  QObject *parent = 0);
    explicit Mail(QString toReceipient,
                  QString sender,
                  QString subject,
                  QString body,
                  QFileInfo attachment,
                  QObject *parent = 0);
    Mail(const Mail& other);

    QString             plaintextMail() const;
    QString             getSender() const;
    QStringList         getAllReceipients() const;
    QStringList         getToReceipients() const;
    QStringList         getCcReceipients() const;
    QStringList         getBccReceipients() const;
    std::pair<int,int>  lastErrors() const;

protected:
    QStringList         toReceipients;
    QStringList         ccReceipients;
    QStringList         bccReceipients;
    QString             sender;
    QString             subject;
    QString             body;
    QList<QFileInfo>    attachments;

    QString         generateBase64FromFile(const QFileInfo&) const;
    QString         foldString(const QString& original) const;
    QString         mimetypeForFile(const QFileInfo& file) const;
    QString         receipientHeaderLineFromStringList(QString header, QStringList addresses) const;

signals:

public slots:

};

#endif // MAIL_H
