#ifndef SBDATA_H
#define SBDATA_H
#include "QStringList"
#include "QFile"
#include <QTextStream>
#include "def.h"
#include <QDate>
#include <QTime>
#include <QDateTime>
#include <QVector>
#include <QRegExp>
#include "def.h"
#include <QProcess>
#include <QFileInfo>

namespace SB {
struct EventInfo
{
    QDate date;
    QTime time;
    QString event;
    QString person;
    bool isNull;
    EventInfo(){
        isNull = true;
    }
};
struct OutputData
{
    QString person;
    QVector<QTime> vFirstEnter;
    QVector<QTime> vLastExit;
};

class SBdata
{
private:
    QStringList fioList;
    QString lastError;
    QVector<EventInfo> evenVec;
    QVector<OutputData> vOut;
    QDate targetDate;


public:
    SBdata();
    const QString getLastError();
    bool loadFio(const QString &path);
    void clearFio();
    bool loadEvent(const QString &path, const QDate &targetDate);
    void clearEvent();
    bool fileExists(QString path);
    void processing();   
    bool saveOutput(const QString savePath);
    QString getCurrentTime();
    QString getAbsoluteFilePath(QString fileName);
};
}

#endif // SBDATA_H
