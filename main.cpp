#include "mainwindow.h"

#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	QGuiApplication::setWindowIcon(QIcon(QString(RPG_ASSET_DIR) + "/rpg.ico"));
    MainWindow w;
    w.show();
    return a.exec();
}
