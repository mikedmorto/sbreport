#ifndef SBCONFIG_H
#define SBCONFIG_H
#include <QString>
#include <QDate>

class SBconfig
{
public:
    SBconfig();

    void setTargetPath(const QString& _targetPath);
    void setOutputPath(const QString& _outputPath);
    void setFioPath(const QString& _fioPath);
    void setTargetDate(const QString& _targetDate);

    QString getTargetPath()const;
    QString getOutputPath()const;
    QString getFioPath()const;
    QDate getTargetDate()const;

    bool isValidDate();

private:
    QString targetPath;
    QString outputPath;
    QString fioPath;
    QDate targetDate;
};

#endif // SBCONFIG_H
