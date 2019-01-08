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
namespace SB {
struct EventInfo
{
    QDate date;
    QTime time;
    QString event;
    QString person;
};

class SBdata
{
private:
    QStringList fioList;
    QString lastError;
    QVector<EventInfo> evenVec;

public:
    SBdata();
    const QString getLastError();
    bool loadFio(const QString &path);
    void clearFio();
    bool loadEvent(const QString &path);
    void clearEvent();

};
}

#endif // SBDATA_H
