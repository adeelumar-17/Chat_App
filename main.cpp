#include "login.h"
#include "database.h"
#include <QApplication>
#include <QScreen>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "CN_Proj_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    Login w;

   /* // Get the primary screen
    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen) {
        QRect screenGeometry = screen->availableGeometry();
        int width = screenGeometry.width();
        int height = screenGeometry.height();

        // Set the window size to fit the screen
        w.resize(width, height);
    }*/

    w.show();
    Database db;
    db.createUserTable();



    return a.exec();
}
