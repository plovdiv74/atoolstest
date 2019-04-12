/*****************************************************************************
* Copyright 2015-2019 Alexander Barthel alex@littlenavmap.org
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include "metartest.h"
#include "geotest.h"
#include "updatetest.h"
#include "dtmtest.h"
#include "versiontest.h"
#include "magdectest.h"
#include "flightplantest.h"
#include "scenerycfgtest.h"
#include "stringtest.h"
#include "onlinetest.h"
#include "perftest.h"

#include <QString>
#include <QTimer>
#include <QtTest/QtTest>

/*
 * Macros that allow to create and add test classes for command line selection.
 */
#define RUNTESTEXT(name) \
  if(parser->isSet( # name) || parser->isSet("RunAll")) \
  { \
    name tst; \
    runtest(tst, messages, otherOptions); \
  }

#define RUNTEST(name) \
  { \
    name tst; \
    QTest::qExec(&tst, argCount, argVector); \
  }

#define DEFINETEST(name) \
  { \
    addOption(parser, # name); \
  };

// Forward declarations
void test();
void runtest(QObject& testObject, QVector<std::pair<int, QString> >& messages, const QStringList& options);
void addOption(QCommandLineParser *parser, const QString& shortOpt);

static int argCount = 0;
static char **argVector = nullptr;
static QCommandLineParser *parser = nullptr;
static QStringList otherOptions;

// Use own streams since Qt directs all output to stderr per default
QDebug& qStdOut();
QDebug& qStdErr();
void initIo();
void deinitIo();

// =====================================================================================
// main
// =====================================================================================
int main(int argc, char *argv[])
{
  Q_INIT_RESOURCE(atools);

  argCount = argc;
  argVector = argv;

  initIo();

  QCoreApplication app(argc, argv);
  QCoreApplication::setApplicationName("atoolstest");
  QCoreApplication::setOrganizationName("ABarthel");
  QCoreApplication::setOrganizationDomain("littlenavmap.org");
  QCoreApplication::setApplicationVersion("0.9.1.develop"); // VERSION_NUMBER

  qStdOut() << "Starting tests ..." << endl;
  qStdOut() << "LANG=" << QProcessEnvironment::systemEnvironment().value("LANG") << endl;
  qStdOut() << "LANGUAGE=" << QProcessEnvironment::systemEnvironment().value("LANGUAGE") << endl;

  // Command line reading
  parser = new QCommandLineParser();
  parser->setApplicationDescription("atools unit tests.");
  parser->addHelpOption();
  parser->addVersionOption();

  QCommandLineOption allOpt("RunAll", QObject::tr("Run all test classes."));
  parser->addOption(allOpt);

  DEFINETEST(OnlineTest);
  DEFINETEST(SceneryCfgTest);
  DEFINETEST(MagdecTest);
  // DEFINETEST(UpdateTest);
  DEFINETEST(StringTest);
  DEFINETEST(PerfTest);
  DEFINETEST(VersionTest);
  DEFINETEST(GeoTest);
  DEFINETEST(FlightplanTest);
  DEFINETEST(MetarTest);
  DEFINETEST(DtmTest);

  parser->parse(QCoreApplication::arguments());
  otherOptions.append(QCoreApplication::arguments().first());
  otherOptions.append(parser->unknownOptionNames());

  if(parser->optionNames().isEmpty() || parser->isSet("h"))
    // Display help and exit
    parser->showHelp();
  else if(parser->isSet("v"))
    // Display version and exit
    parser->showVersion();
  else
  {
    // Start test execution in event queue
    QTimer::singleShot(0, test);
    return app.exec();
  }
}

void test()
{
  // status |= QTest::qExec(&tc, argc, argv);
  QVector<std::pair<int, QString> > messages;

  RUNTESTEXT(OnlineTest);
  RUNTESTEXT(SceneryCfgTest);
  RUNTESTEXT(MagdecTest);
  // RUNTESTEXT(UpdateTest);
  RUNTESTEXT(StringTest);
  RUNTESTEXT(PerfTest);
  RUNTESTEXT(VersionTest);
  RUNTESTEXT(GeoTest);
  RUNTESTEXT(FlightplanTest);
  RUNTESTEXT(MetarTest);
  RUNTESTEXT(DtmTest);

  bool failed = false;
  for(const std::pair<int, QString>& msg : messages)
  {
    if(msg.first > 0)
    {
      qStdErr() << msg.second << "FAILED" << msg.first << endl;
      failed = true;
    }
    else
      qStdOut() << msg.second << "Success" << endl;
  }

  qStdOut() << "exit" << static_cast<int>(failed);

  deinitIo();
  QApplication::exit(failed);
}

void runtest(QObject& testObject, QVector<std::pair<int, QString> >& messages, const QStringList& options)
{
  int result = QTest::qExec(&testObject, options);
  messages.append(std::make_pair(result, QString(testObject.metaObject()->className())));
}

void addOption(QCommandLineParser *cmdParser, const QString& shortOpt)
{
  cmdParser->addOption(QCommandLineOption(shortOpt, QString("Run the %1 test class.").arg(shortOpt)));
}

// =============================================================
// Own IO to avoid all redirected to stderr and have functions of QDebug
static QDebug *qDbgStdOut;
static QDebug *qDbgStdErr;
static QFile stdOutFile;
static QFile stdErrFile;

void initIo()
{
  stdOutFile.open(stdout, QIODevice::WriteOnly | QIODevice::Text);
  stdErrFile.open(stderr, QIODevice::WriteOnly | QIODevice::Text);

  qDbgStdOut = new QDebug(&stdOutFile);
  qDbgStdErr = new QDebug(&stdErrFile);
}

void deinitIo()
{
  delete qDbgStdOut;
  qDbgStdOut = nullptr;
  delete qDbgStdErr;
  qDbgStdErr = nullptr;
}

QDebug& qStdOut()
{
  return *qDbgStdOut;

}

QDebug& qStdErr()
{
  return *qDbgStdErr;
}
