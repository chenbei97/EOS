/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-16 13:47:10
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-16 15:13:35
 * @FilePath: \EOS\component\include\tool\inireadwrite.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_MAIN_CORE_PY_INIREADWRITE_H
#define EOS_MAIN_CORE_PY_INIREADWRITE_H

#include "tool.h"

class COMPONENT_IMEXPORT IniReadWrite : public QObject
{
    Q_OBJECT
public:
    enum IniVariables {ActivateCode,ObjectiveLocation};
    explicit IniReadWrite(QCString filename,QObject*parent = nullptr);

    void printIni() const;
    QString readIni(IniVariables key) const;
    void writeIni(IniVariables key, QCVariant val);
    void writeIni(const IniVariable& v);

    const QMap<IniVariables,QString> IniVariableMap = {
        {IniVariables::ActivateCode, "system/activate_code"},
        {IniVariables::ObjectiveLocation, "system/objective_location"},
    };

private:
    QSettings * mSettings;
private:
    void setValue(QCString key, QCVariant value);
    QVariant value(QCString key, QCVariant defaultValue = QVariant()) const;
    QStringList  allKeys() const;

    void setAtomicSyncRequired(bool enable);

    void setIniCodec(QTextCodec *codec);
    void setIniCodec(const char *codecName);
};



#endif //EOS_MAIN_CORE_PY_INIREADWRITE_H
