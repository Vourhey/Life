#include <QApplication>
#include <QTextCodec>
#include "mainwindow.h"

int main(int argc, char **argv)
{
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    QApplication app(argc, argv);
    MainWindow mw;
    mw.show();
    return app.exec();
}
