/*-
 * Copyright (c) 2015, Martin Kropfinger
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

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
    QString         recepientHeaderLineFromStringList(const QString& header,
                                                      const QStringList& addresses) const;

signals:

public slots:

};

#endif // MAIL_H
