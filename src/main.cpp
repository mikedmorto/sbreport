#include <QCoreApplication>
#include <QCommandLineParser>
#include "def.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    for(int i = 0; i < argc; i ++)
    {
        qDebug()<< i << " " << argv[i];
    }
    qDebug()<<"HI";
    exit(0);
    return a.exec();
}
