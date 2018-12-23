#ifndef SBDATA_H
#define SBDATA_H
#include "QStringList"
#include "QFile"
#include <QTextStream>
#include "def.h"

namespace SB {

class SBdata
{
private:
    QStringList fioList;
    QString lastError;

public:
    SBdata();
    const QString getLastError();
    bool loadFio(const QString &path);
    void clearFio();

};
}

#endif // SBDATA_H
