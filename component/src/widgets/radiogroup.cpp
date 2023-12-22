/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-12-20 10:22:49
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-20 10:26:14
 * @FilePath: \EOS\component\src\widgets\radiogrou.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "radiogroup.h"

void RadioGroup::setChecked(int id, bool checked)
{
    if (id < 1 || id > radioNum) return;
    group->button(id)->setChecked(checked);
}

void RadioGroup::setEnabled(int id)
{ // 指定id设为使能,其它禁用
    if (id < 1 || id > radioNum) return;
    auto buttons = group->buttons();
    for(auto button: buttons) {
        if (group->id(button) != id) {
            button->setEnabled(false);
        } else button->setEnabled(true);
    }
}

void RadioGroup::setEnabled(int id, bool enabled)
{ // 对指定id的按钮设置不影响其它
    if (id < 1 || id > radioNum) return;
    group->button(id)->setEnabled(enabled);
}

QString RadioGroup::checkedText() const
{
    return group->checkedButton()->text();
}

RadioButton* RadioGroup::checkedButton() const
{
    return static_cast<RadioButton*>(group->checkedButton());
}

RadioButton* RadioGroup::button(int id) const
{
    if (id < 1 || id > radioNum) {
        LOG<<"button id"<<id<<"is nullptr!";
        return nullptr;
    }
    return static_cast<RadioButton*>(group->button(id));
}

unsigned RadioGroup::checkedID() const
{
//    unsigned  int id = 1;
//    auto buttons = group->buttons();
//    for(int i = 1; i <= buttons.count(); ++i){
//        if (buttons[i]->isChecked()){
//            id = i;
//            break;
//        }
//    }
//    return id;
    return group->id(group->checkedButton());
}

void RadioGroup::emitSignals(int id)
{
    if (id < 1 || id > radioNum) return;
    group->buttonClicked(id);
}

void RadioGroup::setText(int id, const QString &text)
{
    if (id < 1 || id > radioNum) return;
    group->button(id)->setText(text);
}

void RadioGroup::setText(const QStringList &texts)
{
    auto buttons = group->buttons();
    //LOG<<texts<<buttons.count();
    if (texts.count() >= buttons.count()) {
        for(int i = 0; i < buttons.count(); ++i){
            buttons[i]->setText(texts[i]);
        }
    } else { // texts不够
        for(int i = 0; i < texts.count(); ++i){
            buttons[i]->setText(texts[i]);
        }
    }
}

RadioGroup::RadioGroup(unsigned num, Qt::Orientation orientation,QWidget *parent) : QWidget(parent)
{
    QLayout * lay = nullptr;
    orientation == Qt::Horizontal?
     lay = new QHBoxLayout: lay = new QVBoxLayout;
    group = new QButtonGroup;
    if (num < 2)
        radioNum = 2; // 至少2个才有互斥的意义
    else radioNum = num;

    for(int i = 1; i <= radioNum; ++i){
        auto btn = new RadioButton;
        group->addButton(btn,i); // 注意下标从1开始
        lay->addWidget(btn);
    }
    setLayout(lay);
    setChecked(1,true);
    connect(group,QOverload<int>::of(&QButtonGroup::buttonClicked),this,&RadioGroup::radioClicked);
}

RadioGroup::RadioGroup(QWidget *parent) : QWidget(parent)
{
    auto lay = new QHBoxLayout;
    group = new QButtonGroup;
    radioNum = 2; // 至少2个才有互斥的意义

    for(int i = 1; i <= radioNum; ++i){
        auto btn = new RadioButton;
        group->addButton(btn,i); // 注意下标从1开始
        lay->addWidget(btn);
    }
    setLayout(lay);
    setChecked(1,true);
    connect(group,QOverload<int>::of(&QButtonGroup::buttonClicked),this,&RadioGroup::radioClicked);
}