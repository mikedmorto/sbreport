#include "sbconfig.h"

SBconfig::SBconfig()
{
}

void SBconfig::setTargetPath(const QString &_targetPath)
{
    this->targetPath = _targetPath;

}
void SBconfig::setOutputPath(const QString &_outputPath)
{
    this->outputPath = _outputPath;

}
void SBconfig::setFioPath(const QString &_fioPath)
{
    this->fioPath = _fioPath;

}

QString SBconfig::getTargetPath() const
{
    return this->targetPath;

}

QString SBconfig::getOutputPath() const
{
    return this->outputPath;
}

QString SBconfig::getFioPath() const
{
    return this->fioPath;
}

