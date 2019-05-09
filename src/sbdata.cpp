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
    //qDebug() << "opening file " << path;
    QFile fioFile;
    fioFile.setFileName(path);
    if(!fioFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        this->lastError = "cannot open file " + path + " because of ";
        this->lastError += fioFile.errorString();
         //qDebug() << this->lastError;

         return false;
    }


    QTextStream in(&fioFile);
    in.setCodec("UTF-8");


    while(!in.atEnd()){
        QString line = in.readLine();
        //qDebug() << "The current string " << line << endl;
        this->fioList.append(line);
    }

    //qDebug() << fioList;
    //qDebug() << "closing the file" + path;
    fioFile.close();

    return true;
}

void SBdata::clearFio()
{
    this->fioList.clear();
}

bool SBdata::loadEvent(const QString &path, const QDate &targetDate)
{
    //qDebug()<<"Load event start";
    this->clearEvent();
    QFile eventFile;

    this->targetDate = targetDate;

    eventFile.setFileName(path);
     if(!eventFile.open(QIODevice::ReadOnly | QIODevice::Text)){
         this->lastError = "cannot open file " + path + " because of ";
         this->lastError += eventFile.errorString();
          //qDebug() << this->lastError;

          return false;
     }
     QTextStream in(&eventFile);
     in.setCodec("UTF-8");
     //qDebug()<<"start cycle of loading";
     // todo : delete this out for debug
     QTextStream out(stdout);
     out.setCodec("UTF-8");



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
            ei.time = QTime::fromString(sl.at(1), "h:mm:ss");
            ei.event = sl.at(5);
            ei.person = sl.at(8);

            // testing of date time
            if(ei.date.isValid() and ei.time.isValid()){
                this->evenVec.append(ei);
            }
        }

     }
     //qDebug()<< "evenVec size == " << evenVec.size();
     //qDebug() <<"end load event file";
    return true;
}
void SBdata::clearEvent()
{
    this->evenVec.clear();
}

void SBdata::processing()
{
    QRegExp regEnter("^([0-9]*|парковка):Вход.*$");
    QRegExp regExit ("^([0-9]*|парковка):Выход.*$");


    //qDebug() << "start";
    int lastDay = this->targetDate.daysInMonth();
    this->vOut.clear();
    //qDebug() << "last day: " << lastDay;
    for(int i = 0; i < fioList.size(); i++)
    {
        //qDebug() << "person " << fioList.at(i);
        OutputData outD;

        outD.person = fioList.at(i);

        for( int k = 1 ; k <= lastDay; k++){

            QVector<EventInfo> personEV;
            for(int j = 0; j < evenVec.size(); j++)
            {
                // qDebug()<<"A - "<< evenVec.at(j).person;
                // qDebug()<<"B - "<< fioList.at(i);

                if(evenVec.at(j).person == fioList.at(i)
                        and evenVec.at(j).date.day() == k ){
                // qDebug()<<"OKKKK";

                   // if(!(evenVec.at(j).event.isEmpty())) {
                         personEV.append(evenVec.at(j));
                   // }

                }

            }
            //if(personEV.size() != 0) {
               // qDebug() << QString("Day %1 event size %2").arg(k).arg(personEV.size());
            //}
            EventInfo firstEnter;
            EventInfo lastExit;


            //два множества событий всех входов и всех выходов


            for(int l = 0; l < personEV.size(); l ++) {
//                qDebug()<<"EVENT" << personEV.at(l).event << " - "<< personEV.at(l).date << "="<< personEV.at(l).time;


                if(personEV.at(l).event.contains(regEnter)) {
                    if(firstEnter.isNull){
                        firstEnter = personEV.at(l);
                        firstEnter.isNull = false;
                        continue;
                    }

                    if(firstEnter.time > personEV.at(l).time){
                        firstEnter = personEV.at(l);
                        continue;
                    }
                    continue;
                }


                if(personEV.at(l).event.contains(regExit)){
                    if(lastExit.isNull){
                        lastExit = personEV.at(l);
                        lastExit.isNull = false;
                        continue;
                    }

                    if(lastExit.time < personEV.at(l).time){
                        lastExit = personEV.at(l);
                        continue;
                    }
                    continue;
                }
            }
//            qDebug() << "firstEnter " << firstEnter.time.toString(FORMAT_TIME);
//            qDebug() << "lastExit" << lastExit.time.toString(FORMAT_TIME);

             outD.vFirstEnter.append(firstEnter.time);
             outD.vLastExit.append(lastExit.time);

             // analytic first input, last output
        }
        vOut.append(outD);
    }


    //qDebug() << "end";
    //qDebug() << vOut.size();
}

bool SBdata::saveOutput(const QString savePath)
{
    //если на выводе -, то отслеживать по условию первого входа, последнего выхода, заменять на ноль
    QFile fo;
    QString fname = savePath;
    fo.setFileName(fname);
    if(!fo.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text)){
       this->lastError = "cannot open file " + fname + " because of ";
       this->lastError += fo.errorString();
       return false;
    }

    for(int i = 0; i < vOut.size(); i++){
        QString ret;

        ret.append(vOut.at(i).person);
        for(int j = 0; j < vOut.at(i).vFirstEnter.size(); j++){
            ret.append(",");
             ret.append(vOut.at(i).vFirstEnter.at(j).toString("hh:mm:ss"));
             ret.append("-");
             ret.append(vOut.at(i).vLastExit.at(j).toString("hh:mm:ss"));

        }
    ret.append("\n");
    fo.write(ret.toUtf8());
    }
    fo.close();
    return true;

}

QString SBdata::getCurrentTime()
{
    QString strCurretTime = QDateTime::currentDateTime().toString("yyyy-MM-ddThh:mm:ss ");
    return strCurretTime;
}
