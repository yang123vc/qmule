/*
 * Bittorrent Client using Qt4 and libtorrent.
 * Copyright (C) 2006  Christophe Dumez
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * In addition, as a special exception, the copyright holders give permission to
 * link this program with the OpenSSL project's "OpenSSL" library (or with
 * modified versions of it that use the same license as the "OpenSSL" library),
 * and distribute the linked executables. You must obey the GNU General Public
 * License in all respects for all of the code used other than "OpenSSL".  If you
 * modify file(s), you may extend this exception to your version of the file(s),
 * but you are not obligated to do so. If you do not wish to do so, delete this
 * exception statement from your version.
 *
 * Contact : chris@qbittorrent.org
 */

#ifndef DOWNLOADTHREAD_H
#define DOWNLOADTHREAD_H

#include <QNetworkReply>
#include <QObject>
#include <QHash>
#include <QSslError>

QT_BEGIN_NAMESPACE
class QNetworkAccessManager;
QT_END_NAMESPACE

class DownloadThread : public QObject {
  Q_OBJECT

public:
  DownloadThread(QObject* parent = 0);
  QNetworkReply* downloadUrl(const QString &url);
  void downloadTorrentUrl(const QString &url);
  //void setProxy(QString IP, int port, QString username, QString password);

signals:
  void downloadFinished(const QString &url, const QString &file_path);
  void downloadFailure(const QString &url, const QString &reason);

private slots:
  void processDlFinished(QNetworkReply* reply);
  void checkDownloadSize(qint64 bytesReceived, qint64 bytesTotal);
#ifndef QT_NO_OPENSSL
  void ignoreSslErrors(QNetworkReply*,const QList<QSslError>&);
#endif

private:
  QString errorCodeToString(QNetworkReply::NetworkError status);
  void applyProxySettings();
#ifndef DISABLE_GUI
#ifdef RSS_ENABLE
  void loadCookies(const QString &host_name, QString url);
#endif
#endif

private:
  QNetworkAccessManager m_networkManager;
  QHash<QString, QString> m_redirectMapping;

};

#endif
