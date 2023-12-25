/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-12-25 16:24:55
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-25 17:28:10
 * @FilePath: \EOS\component\src\widgets\pushgroup.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "pushgroup.h"

void PushGroup::setChecked(int id, bool checked)
{
    if (id < 1 || id > btnNum) return;
    auto btn = static_cast<PushButton*>(group->button(id));
    checked?
    btn->setBackGroundColor(Qt::yellow):
    btn->resetBackGroundColor();
}

void PushGroup::setEnabled(int id)
{ // 指定id设为使能,其它禁用
    if (id < 1 || id > btnNum) return;
    auto buttons = group->buttons();
    for(auto button: buttons) {
        if (group->id(button) != id) {
            button->setEnabled(false);
        } else button->setEnabled(true);
    }
}

void PushGroup::setEnabled(int id, bool enabled)
{ // 对指定id的按钮设置不影响其它
    if (id < 1 || id > btnNum) return;
    group->button(id)->setEnabled(enabled);
}

QString PushGroup::checkedText() const
{
    return group->checkedButton()->text();
}

PushButton* PushGroup::checkedButton() const
{
    return static_cast<PushButton*>(group->checkedButton());
}

PushButton* PushGroup::button(int id) const
{
    if (id < 1 || id > btnNum) {
        LOG<<"button id"<<id<<"is nullptr!";
        return nullptr;
    }
    return static_cast<PushButton*>(group->button(id));
}

unsigned PushGroup::checkedID() const
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

void PushGroup::emitSignals(int id)
{
    if (id < 1 || id > btnNum) return;
    group->buttonClicked(id);
}

void PushGroup::setText(int id, const QString &text)
{
    if (id < 1 || id > btnNum) return;
    group->button(id)->setText(text);
}

void PushGroup::setText(const QStringList &texts)
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

PushGroup::PushGroup(unsigned num, Qt::Orientation orientation,QWidget *parent) : QWidget(parent)
{
    QLayout * lay = nullptr;
    orientation == Qt::Horizontal?
            lay = new QHBoxLayout: lay = new QVBoxLayout;
    group = new QButtonGroup;
    if (num < 2)
        btnNum = 2; // 至少2个才有互斥的意义
    else btnNum = num;

    for(int i = 1; i <= btnNum; ++i){
        auto btn = new PushButton;
        group->addButton(btn,i); // 注意下标从1开始
        lay->addWidget(btn);
    }
    setLayout(lay);
    setChecked(1,true);
    connect(group,QOverload<int>::of(&QButtonGroup::buttonClicked),this,&PushGroup::pushClicked);
}

PushGroup::PushGroup(QWidget *parent) : QWidget(parent)
{
    auto lay = new QHBoxLayout;
    group = new QButtonGroup;
    btnNum = 2; // 至少2个才有互斥的意义

    for(int i = 1; i <= btnNum; ++i){
        auto btn = new PushButton;
        group->addButton(btn,i); // 注意下标从1开始
        lay->addWidget(btn);
    }
    setLayout(lay);
    setChecked(1,true);
    connect(group,QOverload<int>::of(&QButtonGroup::buttonClicked),this,&PushGroup::pushClicked);
}

