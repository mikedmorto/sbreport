/*
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *    2010 year ToriSakuraTeam
 *    Author: Mikhail Khromov aka MikeDM (Mike D'Morto)
 *    E-Mail: mikedmorto@gmail.com
 */

#include "mlog.h"

MLog::MLog(QObject *parent) :
    QObject(parent)
{
    // generate session id
    // служит для парсинга лога, позволяет выделить сессию.
//    this->sessionId=QString::number(QDate::currentDate().toJulianDay())+"_"+QString(QTime::currentTime().toString())+
//                    "_"+QString::number(QTime::currentTime().msec());
    this->sessionId = QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch());
    // устанавливаем индикатор состояния лога об отсутствии файла лога
    this->isFile=false;
    // устанавливаем уровень протоколирования по умолчанию из def.h
    this->loglevel=DEFDEBUGLEVEL;
}



bool MLog::isLog(){
// возвращаем состояние протоколирования, пишем события в файл или нет.
    return this->isFile;
}

void MLog::setLevel(int level){
// устанавливаем уровень протоколирования.
// -1вывести сообщение в любом случае.
// 0 вывод в лог отсутствует
// 1 вывод в лог только серьезных сообщений требующих немедленного вмешательства или сообщения о критических ошибках
// 2 вывод в лог сообщений работы модулей
// 3 вывод в лог всех событий
    this->loglevel=level;
}

bool MLog::setFile(const QString & fname){
// открываем файл лога на запись и дополнение
    if(this->isFile){
        this->logfile.close();
        this->isFile=false;
    }
    if(fname.isEmpty()){
        return false;
    }
    this->logfile.setFileName(fname);
    if(!this->logfile.open(QFile::WriteOnly | QFile::Append)){
          return false;
      }else{
          // переключаем индикатор существования потока в лог файл
          this->isFile=true;
          return true;
      }
      return false;
  }

void MLog::slotPut(const QString & owner, MLog::LogLevel level, const QString & text){
    if( ( level <= this->loglevel and this->loglevel != 0 ) or this->loglevel == -1){
        // пишем в лог событие если его уровень равен уровню записи или является критичнее текущего уровня.
        QString levelText;
        switch (level) {
        case MLog::logAlert:
            levelText = "ALERT";
            break;
        case MLog::logInfo:
            levelText = "INFO ";
            break;
        case MLog::logDebug:
            levelText = "DEBUG";
            break;
        default:
            break;
        }

        QString outputText(QDateTime::currentDateTime().toString("yyyy.MM.dd - HH:mm:ss:zzz")+" "+
                           this->sessionId+DELEMITER+levelText+DELEMITER
                           +owner+DELEMITER+text);
        if(!this->isFile){
            // файл не открыт, пишем в стандартный вывод
            printf("%s\n",outputText.toStdString().c_str());
        }else{
            // пишем в лог файл
            QTextStream gg;
            gg.setDevice(&this->logfile);
            gg<<outputText<<"\n";
            if(gg.status() != QTextStream::Ok){
                // ошибка записи в файл лога, перекрываем запись в файл лога
                this->isFile=false;
                this->logfile.close();
                this->slotPut("LOG",MLog::logAlert,"ERROR write to log file");
            }
        }

    }
}

void MLog::closeFile(){
    // закрываем поток вывода лога в файл, таким образом лог будет идти на стандартный вывод
    if(this->isFile){
        this->logfile.close();
        this->isFile=false;
    }
}

QString MLog::getSessionID() const
{
    return this->sessionId;
}
