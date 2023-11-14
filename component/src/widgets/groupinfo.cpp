/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-17 13:28:08
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-17 13:28:29
 * @FilePath: \EOS\component\src\widgets\groupinfo.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "groupinfo.h"

GroupInfo::GroupInfo(QWidget *parent) : QDialog(parent)
{
    initObjects();
    resize(GroupInfoSize);
    connect(btn,&PushButton::clicked,this,&GroupInfo::onClick);
}

QVariantMap GroupInfo::groupInfo() const
{
    QVariantMap m;

    m[HoleExperTypeField] = groupType();
    m[HoleGroupNameField] = groupName();
    m[HoleGroupColorField] = groupColor();
    m[HoleMedicineField] = groupMedicine();
    m[HoleDoseField] = groupDose();
    m[HoleDoseUnitField] = groupDoseUnit();

    return m;
}

void GroupInfo::setGroupInfo(const QVariantMap &m)
{

    auto type = m[HoleExperTypeField].toString();
    auto color = m[HoleGroupColorField].toString();
    auto group = m[HoleGroupNameField].toString();
    auto medicine = m[HoleMedicineField].toString();
    auto dose = m[HoleDoseField].toString();
    auto unit = m[HoleDoseUnitField].toString();

    setGroupType(type);
    setGroupColor(color);
    setGroupName(group);
    setGroupMedicine(medicine);
    setGroupDose(dose);
    setGroupDoseUnit(unit);
}

QString GroupInfo::groupType() const
{
    return grouptype->currentText();
}

QString GroupInfo::groupName() const
{
    return groupname->text().simplified();
}

QColor GroupInfo::groupColor() const
{
    return cbtn->color();
}

QString GroupInfo::groupMedicine() const
{
    return medicine->text().simplified();
}

QString GroupInfo::groupDose() const
{
    return dose->text();
}

QString GroupInfo::groupDoseUnit() const
{
    return doseunit->currentText();
}

void GroupInfo::setGroupType(const QString &type)
{
    grouptype->setCurrentText(type);
}

void GroupInfo::setGroupName(const QString &name)
{
    groupname->setText(name);
}

void GroupInfo::setGroupColor(const QColor &color)
{
    cbtn->setColor(color);
}

void GroupInfo::setGroupMedicine(const QString &mdc)
{
    medicine->setText(mdc);
}

void GroupInfo::setGroupDose(const QString &d)
{
    dose->setText(d);
}

void GroupInfo::setGroupDoseUnit(const QString &unit)
{
    doseunit->setCurrentText(unit);
}

void GroupInfo::onClick()
{
   emit accept();
}

void GroupInfo::initObjects()
{
    grouptype = new ComboBox;
    groupname = new LineEdit;
    medicine = new LineEdit;
    dose = new LineEdit;
    doseunit = new ComboBox;
    btn = new PushButton(tr("Confirm"));
    cbtn = new ColorButton;

    grouptype->addItems(GroupTypeFields);
    doseunit->addItems(GroupDoseUnitFields);
    dose->setValidator(new QDoubleValidator);

    auto lay = new QVBoxLayout;
    auto formlay = new QFormLayout;
    formlay->addRow(tr("Experiment Type: "),grouptype);
    formlay->addRow(tr("Experiment Group: "),groupname);
    formlay->addRow(tr("Experiment Medicine: "),medicine);
    formlay->addRow(tr("Experiment Dose: "),dose);
    formlay->addRow(tr("Dose Unit: "),doseunit);

    auto blay = new QHBoxLayout;
    blay->addStretch();
    blay->addWidget(cbtn);
    blay->addWidget(btn);

    lay->addLayout(formlay);
    lay->addLayout(blay);
    setLayout(lay);
}
