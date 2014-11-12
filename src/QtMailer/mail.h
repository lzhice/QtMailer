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
    explicit Mail(const QStringList& toRecepients,
                  const QStringList& ccRecepients,
                  const QStringList& bccRecepients,
                  const QString& sender,
                  const QString& subject,
                  const QString& body,
                  const QList<QFileInfo>& attachments,
                  QObject *parent = 0);
    explicit Mail(const QStringList& toRecepients,
                  const QStringList& ccRecepients,
                  const QStringList& bccRecepients,
                  const QString& sender,
                  const QString& subject,
                  const QString& body,
                  const QFileInfo& attachment,
                  QObject *parent = 0);
    explicit Mail(const QStringList& toRecepients,
                  const QStringList& ccRecepients,
                  const QStringList& bccRecepients,
                  const QString& sender,
                  const QString& subject,
                  const QString& body,
                  QObject *parent = 0);
    explicit Mail(const QStringList& toRecepients,
                  const QString& sender,
                  const QString& subject,
                  const QString& body,
                  const QList<QFileInfo>& attachments,
                  QObject *parent = 0);
    explicit Mail(const QStringList& toRecepients,
                  const QString& sender,
                  const QString& subject,
                  const QString& body,
                  const QFileInfo& attachments,
                  QObject *parent = 0);
    explicit Mail(const QStringList& toRecepients,
                  const QString& sender,
                  const QString& subject,
                  const QString& body,
                  QObject *parent = 0);
    explicit Mail(const QString& toRecepient,
                  const QString& sender,
                  const QString& subject,
                  const QString& body,
                  QObject *parent = 0);
    explicit Mail(const QString& toRecepient,
                  const QString& sender,
                  const QString& subject,
                  const QString& body,
                  const QFileInfo& attachment,
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
    QString         recepientHeaderLineFromStringList(const QString& header, const QStringList& addresses) const;

signals:

public slots:

};

#endif // MAIL_H
