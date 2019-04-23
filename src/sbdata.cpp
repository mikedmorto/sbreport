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
        qDebug() << "The current string " << line << endl;
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

bool SBdata::loadEvent(const QString &path, const QDate &targetDate)
{
    qDebug()<<"Load event start";
    this->clearEvent();
    QFile eventFile;

    this->targetDate = targetDate;

    eventFile.setFileName(path);
     if(!eventFile.open(QIODevice::ReadOnly | QIODevice::Text)){
         this->lastError = "cannot open file " + path + " because of ";
         this->lastError += eventFile.errorString();
          qDebug() << this->lastError;

          return false;
     }
     QTextStream in(&eventFile);
     in.setCodec("UTF-8");
     qDebug()<<"start cycle of loading";
     // todo : delete this out for debug
     QTextStream out(stdout);
     out.setCodec("UTF-8");
     ///////


     while(!in.atEnd()){
         QString line = in.readLine();
//         qDebug() << "The current string " << line;
         QStringList sl = line.split('\t');

//         qDebug() << sl;
        if(sl.size() < 6 and sl.size() > 10)
            continue;

        //qDebug() << sl.at(5);
        // out<< sl.at(0) << endl;
        QDate tdate = QDate::fromString(sl.at(0),"dd.MM.yyyy");

        if( !((tdate.year() == targetDate.year()) && (tdate.month() == targetDate.month())) )
             continue;

        if(sl.at(5).contains(QRegExp("^(([0-9]*|парковка):Вход.*|([0-9]*|парковка):Выход.*)$"))){
//        if(sl.at(5).contains(QRegExp(QString::fromUtf8("^.*\:Вход.*$")))){
//            qDebug() << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
            EventInfo ei;
//            qDebug()<<sl.at(0);
            ei.date = QDate::fromString(sl.at(0),"dd.MM.yyyy");
            ei.time = QTime::fromString(sl.at(1), "hh:mm:ss");
            ei.event = sl.at(5);
            ei.person = sl.at(8);
            this->evenVec.append(ei);
        }

     }
     qDebug()<< "evenVec size == " << evenVec.size();
     qDebug() <<"end load event file";

}
void SBdata::clearEvent()
{
    this->evenVec.clear();
}

void SBdata::processing()
{
    qDebug() << "start";
    int lastDay = this->targetDate.daysInMonth();
    qDebug() << "last day: " << lastDay;
    for(int i = 0; i < fioList.size(); i++)
    {
        qDebug() << "person " << fioList.at(i);

        for( int k = 1 ; k <= lastDay; k++){

            QVector<EventInfo> personEV;
            for(int j = 0; j < evenVec.size(); j++)
            {
                // qDebug()<<"A - "<< evenVec.at(j).person;
//                qDebug()<<"B - "<< fioList.at(i);

                if(evenVec.at(j).person == fioList.at(i)
                        and evenVec.at(j).date.day() == k ){
//                    qDebug()<<"OKKKK";
                    personEV.append(evenVec.at(j));

                }
            }
            qDebug() << QString("Day %1 event size %2").arg(k).arg(personEV.size());

        }

    }

    qDebug() << "end";

}
