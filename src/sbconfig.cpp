#include "sbconfig.h"

using namespace SB;

SBconfig::SBconfig()
{
    this->isOK = false;
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

bool SBconfig::isValid()
{
    this->validate();
    return this->isOK;
}

void SBconfig::validate()
{
    bool ok = true;
    if(this->fioPath.isEmpty()){
        ok = false;
    }

    if(this->outputPath.isEmpty()){
        ok = false;
    }

    if(this->targetPath.isEmpty()){
        ok = false;
    }
    this->isOK = ok;
}

