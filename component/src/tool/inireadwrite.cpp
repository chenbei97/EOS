/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-16 13:47:26
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-16 15:13:46
 * @FilePath: \EOS\component\src\tool\inireadwrite.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "inireadwrite.h"

IniReadWrite::IniReadWrite(QCString filename,QObject*parent):QObject(parent),
 mSettings(new QSettings(filename,QSettings::IniFormat,parent))
{
    setIniCodec("utf-8");
    setAtomicSyncRequired(true);
}

void IniReadWrite::printIni() const
{//IniVariableMap.values作为键值
   foreach(auto key, IniVariableMap.values())
      LOG<<key<<": "<<value(key).toString();
}

QString IniReadWrite::readIni(IniVariables key) const
{
    return value(IniVariableMap[key]).toString();
}

void IniReadWrite::writeIni(IniVariables key,QCVariant value)
{
    setValue(IniVariableMap[key],value);
}

void IniReadWrite::writeIni(const IniVariable &v)
{// IniVariable[IniVariableMap[IniVariables::ActivateCode]] = "123",这样使用
    if (v.isEmpty()) return;
    foreach(auto key, v.keys())
        setValue(key,v[key]);
}

QVariant IniReadWrite::value(QCString key, QCVariant defaultValue) const
{ // key = system/activate_code
    return mSettings->value(key,defaultValue);
}

void IniReadWrite::setValue(QCString key, QCVariant value)
{
    mSettings->setValue(key,value);
    mSettings->sync();
}

QStringList IniReadWrite::allKeys() const
{
    return mSettings->allKeys();
}

void IniReadWrite::setAtomicSyncRequired(bool enable)
{
    mSettings->setAtomicSyncRequired(enable);
}

void IniReadWrite::setIniCodec(const char *codecName)
{
    mSettings->setIniCodec(codecName);
}

void IniReadWrite::setIniCodec(QTextCodec *codec)
{
    mSettings->setIniCodec(codec);
}
