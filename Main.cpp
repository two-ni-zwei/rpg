#include "MainWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    const QApplication app(argc, argv);
    QGuiApplication::setWindowIcon(QIcon(QString(RPG_ASSET_DIR) + "/rpg.ico"));
    MainWindow win;
    win.show();
    return app.exec();
}
