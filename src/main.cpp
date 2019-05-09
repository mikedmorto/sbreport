#include <QCoreApplication>
#include <QCommandLineParser>
#include <QTextStream>
#include "def.h"
#include "sbconfig.h"
#include "sbdata.h"


using namespace std;

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

    QCommandLineOption target_month(QStringList() << "m" << "target-month",
                                      QCoreApplication::translate("main", "Target month format MM.yyyy like 12.2012"),
                                      QCoreApplication::translate("main", "month"));

    parser.addOption(fio_fileOption);
    parser.addOption(target_fileOption);
    parser.addOption(output_fileOption);
    parser.addOption(target_month);
    parser.process(a);

    SBconfig cfg;
    SB::SBdata data;
    if(parser.isSet("f"))
    {
        cfg.setFioPath(parser.value("f"));
         QTextStream(stdout) << data.getCurrentTime() << "fio-file = " << cfg.getFioPath() << endl;
        //qDebug()<<"fio-file = " << cfg.getFioPath();
    }
    if(parser.isSet("t"))
    {
        cfg.setTargetPath(parser.value("t"));
         QTextStream(stdout)<< data.getCurrentTime() << "target-file = " << cfg.getTargetPath() << endl;
        //qDebug()<<"target-file = " << cfg.getTargetPath();
    }
    if(parser.isSet("o"))
    {
        cfg.setOutputPath(parser.value("o"));
         QTextStream(stdout)<< data.getCurrentTime() << "output-file = " << cfg.getOutputPath() << endl;
        //qDebug()<<"output-file = " << cfg.getOutputPath();
    }
    if(parser.isSet("m"))
    {
        cfg.setTargetDate(parser.value("m"));

        if(!cfg.isValidDate()){
            QTextStream(stdout)<< data.getCurrentTime() << parser.value("m") <<" - invalid date time format " << endl;
            QTextStream(stderr)<< data.getCurrentTime() << parser.value("m") <<" - invalid date time format " << endl;
            exit(1);
        }
    }
    if(parser.isSet("version"))
    {
        QTextStream(stdout)<< data.getCurrentTime() << "Current version " << a.applicationVersion() << endl;
        //qDebug()<<a.applicationVersion();
        exit(0);
    }


    // compare keys
    bool isOK = true;
    if(parser.isSet("f")){
        if(!parser.isSet("t"))
                isOK = false;
        // others
        if(!parser.isSet("m"))
            isOK = false;
    }

    if(!isOK){
        // fail params
        QTextStream(stdout)<< data.getCurrentTime() << "Failure params" << endl;
        QTextStream(stderr)<< data.getCurrentTime() << "Failure params" << endl;
        //qDebug()<<"Failure params";
        exit(1);
    }




    if(!data.loadFio(cfg.getFioPath())){
        QTextStream(stdout)<< data.getCurrentTime() << "Main error = " << data.getLastError();
        QTextStream(stderr)<< data.getCurrentTime() << "Main error = " << data.getLastError();
        //qDebug() << "Main error = " << data.getLastError();
        exit(1);
    }

    if(!data.loadEvent(cfg.getTargetPath(), cfg.getTargetDate()))
    {
        QTextStream(stdout)<< data.getCurrentTime() << "Main error = " << data.getLastError();
        QTextStream(stderr)<< data.getCurrentTime() << "Main error = " << data.getLastError();
        //qDebug() << "Main error = " << data.getLastError();
        exit(1);
    }

    // processing

    data.processing();

    if(!data.saveOutput(cfg.getOutputPath()))
    {
        QTextStream(stdout)<< data.getCurrentTime() << "Main error = " << data.getLastError();
        QTextStream(stderr)<< data.getCurrentTime() << "Main error = " << data.getLastError();
        //qDebug() << "Main error = " << data.getLastError();
        exit(1);
    }



    exit(0);
    return a.exec();
}
