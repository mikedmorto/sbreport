#include "sbdata.h"

using namespace SB;

SBdata::SBdata()
{

}

const QString SBdata::getLastError()
{
   return this->lastError;
}

bool SBdata::loadFio(const QString &path)
{
    qDebug() << "open file " << path;
    QFile fioFile;
    fioFile.setFileName(path);
    if(!fioFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        this->lastError =  fioFile.errorString();
         qDebug() << "cannot open file" << path << "because of "<< this->lastError;

         return false;
    }
    return true;
}
