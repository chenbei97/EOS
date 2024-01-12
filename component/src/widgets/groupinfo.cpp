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
    resize(DefaultWindowSize.width(),DefaultWindowSize.height()-100);
    connect(btn,&PushButton::clicked,this,&GroupInfo::onClick);
    grouptype->setEnabled(false); // 只能外部去更改
    grouptype->setCurrentIndex(0);
    cbtn->hide();
    setWindowFlags(windowFlags()& ~Qt::WindowContextHelpButtonHint);
}

QVariantMap GroupInfo::groupInfo() const
{
    QVariantMap m;

    m[HoleGroupNameField] = groupName();
    m[HoleGroupColorField] = groupColor();
    m[HoleMedicineField] = groupMedicine();
    m[HoleDoseField] = groupDose();
    m[HoleDoseUnitField] = groupDoseUnit();

    return m;
}

void GroupInfo::setGroupInfo(const QVariantMap &m)
{
    auto color = m[HoleGroupColorField].toString();
    auto group = m[HoleGroupNameField].toString();
    auto medicine = m[HoleMedicineField].toString();
    auto dose = m[HoleDoseField].toString();
    auto unit = m[HoleDoseUnitField].toString();

    setGroupColor(color);
    setGroupName(group);
    setGroupMedicine(medicine);
    setGroupDose(dose);
    setGroupDoseUnit(unit);
    allGroups = m[HoleAllGroupsField].value<QSet<QString>>();//目的是每次confirm检测当前的分组是否已经被使用过
}

void GroupInfo::initObjects()
{
    grouptype = new ComboBox;
    medicine = new LineEdit;
    dose = new LineEdit;
    doseunit = new ComboBox;
    btn = new PushButton(tr("Confirm"));
    cbtn = new ColorButton;
    cbtn->setObjectName("groupInfoColorButton");

    grouptype->addItems(GroupTypeFields);
    doseunit->addItems(GroupDoseUnitFields);
    dose->setValidator(new QDoubleValidator);

    auto lay = new QVBoxLayout;
    auto formlay = new QFormLayout;
    formlay->addRow(tr("Experiment Group: "),grouptype);
    formlay->addRow(tr("Experiment Medicine: "),medicine);
    formlay->addRow(tr("Experiment Dose: "),dose);
    formlay->addRow(tr("Dose Unit: "),doseunit);

    auto blay = new QHBoxLayout;
    blay->addStretch();
    blay->addWidget(cbtn);
    blay->addWidget(btn);

    lay->addLayout(formlay);
    lay->addLayout(blay);
    lay->addStretch();
    setLayout(lay);
}

QString GroupInfo::groupName() const
{
    return grouptype->currentText();
}

QColor GroupInfo::groupColor() const
{
    return grouptype->itemData(0,Qt::BackgroundRole).toString();
    //return cbtn->color();
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

void GroupInfo::setGroupName(const QString &name)
{
    grouptype->setCurrentText(name);
}

void GroupInfo::setGroupColor(const QColor &color)
{
    LOG<<"group color ="<<color;
    // wellpattern_holeinfo默认是白色,首次打开对话框会把白色传递给这里
    // 这个白色并不是从分组那里来的所以不要传递给grouptype
    if (color != WhiteColor) {
        // cbtn->setColor(color);
        grouptype->setItemData(0,color,Qt::BackgroundRole);
        // 颜色还是要设置
        grouptype->setStyleSheet(tr("QComboBox:!editable{background:%1}").arg(color.name()));
    }
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
    if (grouptype->currentIndex() != -1) {
        if (allGroups.contains(grouptype->currentText())) { // 重复分组的情况提示是否继续
            int ret = QMessageBox::information(this,InformationChinese,
                     tr("The current group has already been used. Do you still want to continue?"),
                     QMessageBox::Yes | QMessageBox::Cancel);
            if (ret == QMessageBox::Yes) {
                accept();
                emit groupSetted(grouptype->currentText());
            }
        } else {
            accept();
            emit groupSetted(grouptype->currentText());
        }
    } else {
        QMessageBox::warning(this,WarningChinese,"Group Not Set!");
        reject();
    }
}
