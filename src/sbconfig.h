#ifndef SBCONFIG_H
#define SBCONFIG_H
#include <QString>

class SBconfig
{
public:
    SBconfig();

    void setTargetPath(const QString& _targetPath);
    void setOutputPath(const QString& _outputPath);
    void setFioPath(const QString& _fioPath);

    QString getTargetPath()const;
    QString getOutputPath()const;
    QString getFioPath()const;


private:
    QString targetPath;
    QString outputPath;
    QString fioPath;
};

#endif // SBCONFIG_H
