#include <QCoreApplication>
#include <QCommandLineParser>
#include <QTextStream>
#include "def.h"
#include "sbconfig.h"
#include "sbdata.h"
#include "mlog.h"


using namespace std;

int main(int argc, char *argv[])
{
    MLog log;
    QCoreApplication a(argc, argv);
    a.setApplicationName(APP_NAME);
    a.setApplicationVersion(GIT_VERSION);
    a.setOrganizationName(CORP_NAME);

    log.slotPut("MAIN", MLog::logInfo, QString("Start application %1 %2").arg(APP_NAME).arg(GIT_VERSION));

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
    if(parser.isSet("version"))
    {
        QTextStream(stdout)<<"application " << a.applicationName() << " version " << a.applicationVersion() << endl;
        exit(0);
    }

    if(parser.isSet("f"))
    {
        cfg.setFioPath(parser.value("f"));
        log.slotPut("MAIN", MLog::logDebug, QString("Param fio-file = \"%1\"").arg(cfg.getFioPath()));
    }
    if(parser.isSet("t"))
    {
        cfg.setTargetPath(parser.value("t"));
        log.slotPut("MAIN", MLog::logDebug, QString("Param target-file = \"%1\"").arg(cfg.getTargetPath()));
    }
    if(parser.isSet("o"))
    {
        cfg.setOutputPath(parser.value("o"));
        log.slotPut("MAIN", MLog::logDebug, QString("Param output-file = \"%1\"").arg(cfg.getOutputPath()));
    }
    if(parser.isSet("m"))
    {
        cfg.setTargetDate(parser.value("m"));
        log.slotPut("MAIN", MLog::logDebug, QString("Param date = \"%1\"").arg(cfg.getTargetDate().toString("MM.yyyy")));
        if(!cfg.isValidDate()){
            log.slotPut("MAIN", MLog::logAlert, QString("invalid date time format. Exit."));
            exit(1);
        }
    }


    // testing parameters
    if(!parser.isSet("f")){
        log.slotPut("MAIN", MLog::logAlert, QString("Parameter -f is not set. Exit."));
        exit(1);
    }
    if(!parser.isSet("t")){
        log.slotPut("MAIN", MLog::logAlert, QString("Parameter -t is not set. Exit."));
        exit(1);
    }
    if(!parser.isSet("o")){
        log.slotPut("MAIN", MLog::logAlert, QString("Parameter -o is not set. Exit."));
        exit(1);
    }
    if(!parser.isSet("m")){
        log.slotPut("MAIN", MLog::logAlert, QString("Parameter -m is not set. Exit."));
        exit(1);
    }


    log.slotPut("MAIN", MLog::logInfo, QString("Start load fio file"));
    if(!data.loadFio(cfg.getFioPath())){
        log.slotPut("MAIN", MLog::logAlert, QString("Failure load fio file because of %1").arg(data.getLastError()));
        exit(2);
    }
    log.slotPut("MAIN", MLog::logInfo, QString("Stop load fio file"));


    log.slotPut("MAIN", MLog::logInfo, QString("Start load target file"));
    if(!data.loadEvent(cfg.getTargetPath(), cfg.getTargetDate()))
    {
        log.slotPut("MAIN", MLog::logAlert, QString("Failure load target file because of %1").arg(data.getLastError()));
        exit(3);
    }
    log.slotPut("MAIN", MLog::logInfo, QString("Stop load target file"));

    // processing
    log.slotPut("MAIN", MLog::logInfo, QString("Start processing"));
    data.processing();
    log.slotPut("MAIN", MLog::logInfo, QString("Stop processing"));


    log.slotPut("MAIN", MLog::logInfo, QString("Start write output file"));
    if(!data.saveOutput(cfg.getOutputPath()))
    {
        log.slotPut("MAIN", MLog::logAlert, QString("Failure write output file because of %1").arg(data.getLastError()));
        exit(4);
    }
    log.slotPut("MAIN", MLog::logInfo, QString("Stop write output file"));


    log.slotPut("MAIN", MLog::logInfo, QString("Stop application"));
    exit(0);
    return a.exec();
}
