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
    bool isOK = true;
    if(parser.optionNames().empty()){
        QTextStream(stderr)<< data.getCurrentTime() << "[error] " << "Run without params\n";
        exit(1);
    }
    else{
        if(parser.isSet("f")){
            cfg.setFioPath(parser.value("f"));
            if(!(data.fileExists(cfg.getFioPath()))){
                QTextStream(stderr) << data.getCurrentTime() << "[error] " << "Fio file's doesn't exist\n";
                isOK = false;
            }else{
                QTextStream(stdout) << data.getCurrentTime() << "[info] " << "Reading of fio-file, fio-file path = "
                                    << data.getAbsoluteFilePath(cfg.getFioPath()) << endl;
                //            qDebug()<<"fio-file = " << cfg.getFioPath();
            }
        }else{
            isOK = false;
        }
        if(parser.isSet("t")){
            cfg.setTargetPath(parser.value("t"));
            if(!(data.fileExists(cfg.getTargetPath()))){
                QTextStream(stderr) << data.getCurrentTime() << "[error] " << "Target file's doesn't exist\n";
                isOK = false;
            }else{
                QTextStream(stdout)<< data.getCurrentTime() << "[info] " << "Reading of target-file, target-file path = "
                                   << data.getAbsoluteFilePath(cfg.getTargetPath()) << endl;
                //          qDebug()<<"target-file = " << cfg.getTargetPath();
            }
        }else{
            isOK = false;
        }
        if(parser.isSet("o")){
            cfg.setOutputPath(parser.value("o"));
            if(parser.value("o").isEmpty()){
                QTextStream(stderr)<< data.getCurrentTime() << "[error] " << "Target file is empty\n";
                isOK = false;
            } else{
                QTextStream(stdout)<< data.getCurrentTime() << "[info] " << "Creating of output-file, output-file path  = "
                                   << data.getAbsoluteFilePath(cfg.getOutputPath()) << endl;
                //       qDebug()<<"output-file = " << cfg.getOutputPath();
            }
        }else{
            isOK = false;
        }
        if(parser.isSet("m")){
            cfg.setTargetDate(parser.value("m"));
            if(!cfg.isValidDate()){
                QTextStream(stderr)<< data.getCurrentTime() << "[error] " << parser.value("m") <<" Invalid date time format " << endl;
                isOK = false;
            }else{
                QTextStream(stdout)<< data.getCurrentTime() << "[info] " << "Set time's format, format = " << "yyyy-MM-dd hh:mm:ss " << endl;
            }

        }else{
            isOK = false;
        }
        if(parser.isSet("version")){
            QTextStream(stdout)<< data.getCurrentTime() << "[info] " << "Current version " << a.applicationVersion() << endl;
            //        qDebug()<<a.applicationVersion();
        }
    }
    if(!isOK){
        //         fail params
        QTextStream(stderr)<< data.getCurrentTime() << "[error] " << "Failure params" << endl;
        //        qDebug()<<"Failure params";
        exit(1);
    } else{
        //     processing
        data.processing();
        if(!data.loadFio(cfg.getFioPath())){
            QTextStream(stderr)<< data.getCurrentTime() << "[error] " << "Main error of fioFile = " << data.getLastError();
            //        qDebug() << "Main error = " << data.getLastError();
            exit(1);
        }
        if(!data.loadEvent(cfg.getTargetPath(), cfg.getTargetDate()))
        {
            QTextStream(stderr)<< data.getCurrentTime() << "[error] " << "Main error of targetFile = " << data.getLastError();
            //        qDebug() << "Main error = " << data.getLastError();
            exit(1);
        }
        if(!data.saveOutput(cfg.getOutputPath()))
        {
            QTextStream(stderr)<< data.getCurrentTime() << "[error] " << "Main error of outputFile = " << data.getLastError();
            //        qDebug() << "Main error = " << data.getLastError();
            exit(1);
        }
    }
    exit(0);
    return a.exec();
}
