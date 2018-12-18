#include <QCoreApplication>
#include <QCommandLineParser>
#include "def.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    a.setApplicationName(APP_NAME);
    a.setApplicationVersion(APP_VER);
    a.setOrganizationName(CORP_NAME);

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addOption({{"v","version"}, QCoreApplication::tr("Show version")});

    parser.process(a);
    if(parser.isSet("version"))
    {
        qDebug()<<a.applicationVersion();
    }




    exit(0);
    return a.exec();
}
