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

#include <QLocale>
#include <QTranslator>
#include <QFile>
#include <QLibraryInfo>

#ifndef DISABLE_GUI
#include <QMessageBox>
#include <QStyleFactory>
#include <QStyle>
#include <QSplashScreen>
#include <QPainter>
#include <QPen>
#include <QFont>
#include <QPushButton>
#include <QTimer>
#include "sessionapplication.h"
#include "mainwindow.h"
#include "ico.h"
#else
#include "qtsinglecoreapplication.h"
#include <iostream>
#include <stdio.h>
#include "headlessloader.h"
#endif

#include "preferences.h"
#if defined(Q_WS_X11) || defined(Q_WS_MAC)
#include <signal.h>
#include <execinfo.h>
#include "stacktrace.h"
#endif

#include <stdlib.h>
#include "misc.h"
#include "preferences.h"

class UsageDisplay: public QObject {
  Q_OBJECT

public:
  static void displayUsage(char* prg_name) {
    std::cout << qPrintable(tr("Usage:")) << std::endl;
    std::cout << '\t' << prg_name << " --version: " << qPrintable(tr("displays program version")) << std::endl;
#ifndef DISABLE_GUI
    std::cout << '\t' << prg_name << " --no-splash: " << qPrintable(tr("disable splash screen")) << std::endl;
#endif
    std::cout << '\t' << prg_name << " --help: " << qPrintable(tr("displays this help message")) << std::endl;    
    std::cout << '\t' << prg_name << " " << qPrintable(tr("[files or urls]: downloads the torrents passed by the user (optional)")) << std::endl;
  }
};

#include "main.moc"

#if defined(Q_WS_X11) || defined(Q_WS_MAC)
void sigintHandler(int) {
  signal(SIGINT, 0);
  qDebug("Catching SIGINT, exiting cleanly");
  qApp->exit();
}

void sigtermHandler(int)
{
  signal(SIGTERM, 0);
  qDebug("Catching SIGTERM, exiting cleanly");
  qApp->exit();
}

void sigsegvHandler(int) {
  signal(SIGABRT, 0);
  signal(SIGSEGV, 0);
  std::cerr << "\n\n*************************************************************\n";
  std::cerr << "Catching SIGSEGV, please report a bug at http://www.is74.ru provide the following backtrace:\n";
  std::cerr << "qMule version: " << VERSION << std::endl;
  print_stacktrace();
  raise(SIGSEGV);
}

void sigabrtHandler(int) {
  signal(SIGABRT, 0);
  signal(SIGSEGV, 0);
  std::cerr << "\n\n*************************************************************\n";
  std::cerr << "Catching SIGABRT, please report a bug at http://www.is74.ru provide the following backtrace:\n";
  std::cerr << "qMule version: " << VERSION << std::endl;
  print_stacktrace();
  raise(SIGABRT);
}
#endif

// Main
int main(int argc, char *argv[]) {
  // Create Application
  QString uid = misc::getUserIDString();
#ifdef DISABLE_GUI
  QtSingleCoreApplication app("qMule-"+uid, argc, argv);
#else
  SessionApplication app("qMule-"+uid, argc, argv);
#endif

  // Check if qMule is already running for this user
  if (app.isRunning()) {
    qDebug("qMule is already running for this user.");
    //Pass program parameters if any
    QString message;
    for (int a = 1; a < argc; ++a) {
      QString p = QString::fromLocal8Bit(argv[a]);
      if (p.startsWith("--")) continue;
      message += p;
      if (a < argc-1)
        message += "|";
    }
    if (!message.isEmpty()) {
      qDebug("Passing program parameters to running instance...");
      qDebug("Message: %s", qPrintable(message));
      app.sendMessage(message);
    }
    return 0;
  }

  Preferences pref;
#ifndef DISABLE_GUI
  bool no_splash = false;
#endif

  // Load translation
  QString locale = pref.getLocale();
  QTranslator qtTranslator;
  QTranslator translator;

  if (locale.isEmpty()) 
  {
    locale = QLocale::system().name();
    qDebug("locale is empty, use %s", qPrintable(locale));
    pref.setLocale(locale);
    if (locale.isEmpty())
    {
        locale = "en_GB";
    }
  }

  qDebug("Program locale: %s", qPrintable(locale));
  setlocale(LC_CTYPE, ""); // TODO - need attention

  //locale = "ru.ts";

  if (qtTranslator.load(
          QString::fromUtf8("qt_") + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath)
                                                                    )) {
    qDebug("Qt %s locale recognized, using translation.", qPrintable(locale));
  }else{
    qDebug("Qt %s locale unrecognized, using default (en_GB).", qPrintable(locale));
  }
  app.installTranslator(&qtTranslator);
  if (translator.load(QString::fromUtf8(":/lang/qmule_") + locale)) {
    qDebug("%s locale recognized, using translation.", qPrintable(locale));
  }else{
    qDebug("%s locale unrecognized, using default (en_GB).", qPrintable(locale));
  }
  app.installTranslator(&translator);
#ifndef DISABLE_GUI
  if (locale.startsWith("ar")) {
    qDebug("Right to Left mode");
    app.setLayoutDirection(Qt::RightToLeft);
  } else {
    app.setLayoutDirection(Qt::LeftToRight);
  }
#endif
  app.setApplicationName(QString::fromUtf8("qMule"));

  // Check for executable parameters
  if (argc > 1) {
    if (QString::fromLocal8Bit(argv[1]) == QString::fromUtf8("--version")) {
      std::cout << "qMule " << VERSION << '\n';
      return 0;
    }
    if (QString::fromLocal8Bit(argv[1]) == QString::fromUtf8("--help")) {
      UsageDisplay::displayUsage(argv[0]);
      return 0;
    }

    for (int i=1; i<argc; ++i) {
#ifndef DISABLE_GUI
      if (QString::fromLocal8Bit(argv[i]) == QString::fromUtf8("--no-splash")) {
        no_splash = true;
      } else {
#endif
        if (QString::fromLocal8Bit(argv[i]).startsWith("--webui-port=")) {
          QStringList parts = QString::fromLocal8Bit(argv[i]).split("=");
          if (parts.size() == 2) {
            bool ok = false;
            int new_port = parts.last().toInt(&ok);
            if (ok && new_port > 0 && new_port <= 65535) {
              Preferences().setWebUiPort(new_port);
            }
          }
        }
#ifndef DISABLE_GUI
      }
#endif
    }
  }

#ifndef DISABLE_GUI
  if (pref.isSlashScreenDisabled()) {
    no_splash = true;
  }
  QSplashScreen *splash = 0;
  if (!no_splash) {
    QPixmap splash_img(":/Icons/skin/Logo.png");
    QPainter painter(&splash_img);
    QString prog_name = "eMule - qMule";
    QString version = "v0.01";
    painter.setPen(QPen(Qt::black));
    painter.setFont(QFont("Arial", 22, QFont::Black));
    painter.drawText(175 - painter.fontMetrics().width(prog_name) / 2, 220, prog_name);
    painter.drawText(175 - painter.fontMetrics().width(version) / 2, 220 + painter.fontMetrics().height(), version);
    
    splash = new QSplashScreen(splash_img, Qt::WindowStaysOnTopHint);
    QTimer::singleShot(1500, splash, SLOT(deleteLater()));
    splash->show();
    app.processEvents();
  }
#endif
  // Set environment variable
  if (qputenv("QMULE", QByteArray(VERSION))) {
    std::cerr << "Couldn't set environment variable...\n";
  }

#ifndef DISABLE_GUI
  app.setStyleSheet("QStatusBar::item { border-width: 0; }");
#endif

#ifndef DISABLE_GUI
  app.setQuitOnLastWindowClosed(false);
#endif
#if defined(Q_WS_X11) || defined(Q_WS_MAC)
  signal(SIGABRT, sigabrtHandler);
  signal(SIGTERM, sigtermHandler);
  signal(SIGINT, sigintHandler);
  signal(SIGSEGV, sigsegvHandler);
#endif
  // Read torrents given on command line
  QStringList torrentCmdLine = app.arguments();
  // Remove first argument (program name)
  torrentCmdLine.removeFirst();
#ifndef QT_NO_DEBUG_OUTPUT
  foreach (const QString &argument, torrentCmdLine) {
    qDebug() << "Command line argument:" << argument;
  }
#endif

#ifndef DISABLE_GUI
  MainWindow window(0, torrentCmdLine);
  QObject::connect(&app, SIGNAL(messageReceived(const QString&)),
                   &window, SLOT(processParams(const QString&)));
  app.setActivationWindow(&window);
#else
  // Load Headless class
  HeadlessLoader loader(torrentCmdLine);
  QObject::connect(&app, SIGNAL(messageReceived(const QString&)),
                   &loader, SLOT(processParams(const QString&)));
#endif

  LOGGER_INIT(LOG_ALL)

  int ret = app.exec();
  qDebug("Application has exited");
  return ret;
}


