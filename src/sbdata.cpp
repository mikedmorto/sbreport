#include "sbdata.h"

using namespace SB;

SBdata::SBdata()
{

}

const QString SBdata::getLastError(){
   return this->lastError;
}

bool SBdata::loadFio(const QString &path)
{
    this->fioList.clear();
    qDebug() << "opening file " << path;
    QFile fioFile;
    fioFile.setFileName(path);
    if(!fioFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        this->lastError = "cannot open file " + path + " because of ";
        this->lastError += fioFile.errorString();
         qDebug() << this->lastError;

         return false;
    }
    QTextStream in(&fioFile);
    in.setCodec("UTF-8");

    while(!in.atEnd()){
        QString line = in.readLine();
        this->fioList.append(line);
    }
    qDebug() << fioList;
    qDebug() << "closing the file" + path;
    fioFile.close();
    return true;
}

void SBdata::clearFio()
{
    this->fioList.clear();
}
