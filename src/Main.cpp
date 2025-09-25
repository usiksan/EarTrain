/*
  Project "Ear train"
  Author
    Sibilev Alexander S.

  Web
    www.saliLab.com
    www.saliLab.ru

  Description
    Program entry point
*/

#include "CsConfig.h"
#include "score/CsComposition.h"
#include "score/CsPlayer.h"
#include "score/CsEarTrainer.h"
#include "score/CsParser.h"
#include "midiKeyboard/CsMidiKeyboardLinux.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QLocale>
#include <QTranslator>
#include <QFontDatabase>

int main(int argc, char *argv[])
  {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
  QGuiApplication app(argc, argv);

  //Setups for settings
  QCoreApplication::setOrganizationName(CS_ORGANIZATION_NAME);
  QCoreApplication::setOrganizationDomain(CS_ORGANIZATION_DOMAIN);
  QCoreApplication::setApplicationName(CS_APPLICATION_NAME);

  //Утилиты
  qmlRegisterType<CsComposition>        ( "CsBook", 1, 0, "CsComposition" );
  qmlRegisterType<CsEarTrainer>         ( "CsBook", 1, 0, "CsEarTrainer" );

  CsParser::init();

  QTranslator translator;
  const QStringList uiLanguages = QLocale::system().uiLanguages();
  for (const QString &locale : uiLanguages) {
    const QString baseName = "SaliSongBook_" + QLocale(locale).name();
    if (translator.load(":/i18n/" + baseName)) {
      app.installTranslator(&translator);
      break;
      }
    }

  //In Android default font has not musical symbols,
  // so we append in application musical font
  int id = QFontDatabase::addApplicationFont(":/ttf/NotoMusic-Regular.ttf");
  QString fontFamily = QFontDatabase::applicationFontFamilies(id).at(0);


  //=============================================================================
  //        Midi keyboard
#ifdef Q_OS_ANDROID
  CsMidiKeyboard *kbd = new CsMidiKeyboardAndroid();
#else
#ifdef Q_OS_LINUX
  CsMidiKeyboard *kbd = new CsMidiKeyboardLinux();
#endif
#endif
  kbd->init();
  kbd->connect( CsEarTrainer::instance(), &CsPlayer::pianoKeySet, CsComposition::instance(), &CsComposition::pianoKeySet );

  //Line player
  kbd->connect( kbd, &CsMidiKeyboard::tick, CsEarTrainer::instance(), &CsEarTrainer::tick, Qt::QueuedConnection );
  kbd->connect( kbd, &CsMidiKeyboard::midiNote, CsEarTrainer::instance(), &CsEarTrainer::kbdNote, Qt::QueuedConnection );
  kbd->connect( CsEarTrainer::instance(), &CsEarTrainer::playNote, kbd, &CsMidiKeyboard::playNote );
  kbd->connect( CsEarTrainer::instance(), &CsEarTrainer::playVoice, kbd, &CsMidiKeyboard::playVoice );
  kbd->connect( CsEarTrainer::instance(), &CsEarTrainer::playTempo, kbd, &CsMidiKeyboard::playTempo );
  kbd->connect( CsEarTrainer::instance(), &CsEarTrainer::cellTick, CsComposition::instance(), &CsComposition::cellTick );

  //Загрузить визуальные настройки
  CsEarTrainer::instance()->loadConfig();

  QQmlApplicationEngine engine;

  engine.rootContext()->setContextProperty( "csComposition", CsComposition::instance() );
  engine.rootContext()->setContextProperty( "csPlayer", CsEarTrainer::instance() );
  engine.rootContext()->setContextProperty( "csMusicFont", fontFamily );

  const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
  QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                   &app, [url](QObject *obj, const QUrl &objUrl) {
    if (!obj && url == objUrl)
      QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
  engine.load(url);

  int res = app.exec();

  return res;
  }
