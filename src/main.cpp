#include <QCoreApplication>
#include <QCommandLineParser>
#include "def.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug()<<"HI";
    exit(0);
    return a.exec();
}
