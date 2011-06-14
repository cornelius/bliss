#include "mainwindow.h"

#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <KDE/KLocale>

static const char description[] =
  I18N_NOOP("Managing todos the happy way");

static const char version[] = "0.1";

int main(int argc, char **argv)
{
  KAboutData about("bliss", 0, ki18n("Bliss"), version, ki18n(description),
                   KAboutData::License_GPL, ki18n("(C) 2011 Cornelius Schumacher"), KLocalizedString(), 0, "schumacher@kde.org");
  about.addAuthor( ki18n("Cornelius Schumacher"), KLocalizedString(), "schumacher@kde.org" );
  KCmdLineArgs::init(argc, argv, &about);

  KCmdLineOptions options;
  options.add("+[URL]", ki18n( "Document to open" ));
  KCmdLineArgs::addCmdLineOptions(options);
  KApplication app;

  if ( app.isSessionRestored() ) {
    RESTORE(MainWindow);
  } else {
    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
    if (args->count() == 0) {
      MainWindow *widget = new MainWindow;
      widget->show();
      widget->readData();
    } else {
      for ( int i = 0; i < args->count(); i++ ) {
        MainWindow *widget = new MainWindow;
        widget->show();
        widget->readData( args->arg( i ) );
      }
    }
    args->clear();
  }

  return app.exec();
}
