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

#ifndef MLOG_H
#define MLOG_H

#include <QObject>
#include <QFile>
#include <QString>
#include <QtDebug>
#include <QTextStream>

#include <QDate>
#include <QTime>
#define DEFDEBUGLEVEL 3
#define DELEMITER " | "

class MLog : public QObject
{
    Q_OBJECT
public:
    explicit MLog(QObject *parent = 0);
    enum LogLevel{logNone=0,logAlert,logInfo,logDebug};

    bool setFile(const QString & fname);
    void setLevel(int level);
    bool isLog();
    void closeFile();
    QString getSessionID()const;
signals:

public slots:
    void slotPut(const QString & owner,MLog::LogLevel level,const QString & text);
private:
    int loglevel;
    QString sessionId;
    QFile logfile;
    bool isFile;

};

#endif // MLOG_H
