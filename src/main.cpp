#include <QCoreApplication>
#include <QCommandLineParser>
#include "def.h"
#include "sbconfig.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    a.setApplicationName(APP_NAME);
    a.setApplicationVersion(GIT_VERSION);
    a.setOrganizationName(CORP_NAME);

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addOption({{"v","version"}, QCoreApplication::tr("Show version")});

    QCommandLineOption fio_fileOption(QStringList() << "f" << "fio-file",
                                      QCoreApplication::translate("main", "Path to FIO file <path>."),
                                      QCoreApplication::translate("main", "path"));

    QCommandLineOption target_fileOption(QStringList() << "t" << "target-file",
                                      QCoreApplication::translate("main", "Path to target file <path>."),
                                      QCoreApplication::translate("main", "path"));

    QCommandLineOption output_fileOption(QStringList() << "o" << "output-file",
                                      QCoreApplication::translate("main", "Path to output file <path>."),
                                      QCoreApplication::translate("main", "path"));

    parser.addOption(fio_fileOption);
    parser.addOption(target_fileOption);
    parser.addOption(output_fileOption);

    parser.process(a);
    SBconfig cfg;
    if(parser.isSet("f"))
    {
        cfg.setFioPath(parser.value("f"));
        qDebug()<<"fio-file = " << cfg.getFioPath();
    }
    if(parser.isSet("t"))
    {
        cfg.setTargetPath(parser.value("t"));
        qDebug()<<"target-file = " << cfg.getTargetPath();
    }
    if(parser.isSet("o"))
    {
        cfg.setOutputPath(parser.value("o"));
        qDebug()<<"output-file = " << cfg.getOutputPath();
    }
    if(parser.isSet("version"))
    {
        qDebug()<<a.applicationVersion();
    }


    exit(0);
    return a.exec();
}
