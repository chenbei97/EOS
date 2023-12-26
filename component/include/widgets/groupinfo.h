/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-17 13:27:57
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-17 13:28:37
 * @FilePath: \EOS\component\include\widgets\groupinfo.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_GROUPINFO_H
#define EOSI_GROUPINFO_H

#include "label.h"
#include "lineedit.h"
#include "combobox.h"
#include "colorbutton.h"

class COMPONENT_IMEXPORT GroupInfo: public QDialog
{
    Q_OBJECT
public:
    explicit GroupInfo(QWidget*parent= nullptr);

    QString groupName() const;
    QColor groupColor() const;
    QString groupMedicine() const;
    QString groupDose() const;
    QString groupDoseUnit() const;

    QVariantMap groupInfo() const;
    void setGroupInfo(const QVariantMap& m);

    void setGroupName(const QString& name);
    void setGroupColor(const QColor& color);
    void setGroupMedicine(const QString& mdc);
    void setGroupDose(const QString& d);
    void setGroupDoseUnit(const QString& unit);

private:
    void initObjects();
    void onClick();
    ComboBox * grouptype;
    LineEdit * medicine;
    LineEdit * dose;
    ComboBox * doseunit;
    PushButton * btn;
    ColorButton * cbtn;

signals:
    void groupSetted(const QString& text);
};

#endif //EOSI_GROUPINFO_H
