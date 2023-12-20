
#include "buttongroup.h"

void ButtonGroup::setChecked(int id, bool checked)
{
    if (id < 1 || id > buttonNum) return;
    group->button(id)->setChecked(checked);
}

QString ButtonGroup::checkedText() const
{
    return group->checkedButton()->text();
}

QAbstractButton* ButtonGroup::checkedButton() const
{
    return group->checkedButton();
}

QAbstractButton* ButtonGroup::button(int id) const
{
    return group->button(id);
}

unsigned ButtonGroup::checkedID() const
{
    return group->id(group->checkedButton());
}

void ButtonGroup::emitSignals(int id)
{
    if (id < 1 || id > buttonNum) return;
    group->buttonClicked(id);
}

void ButtonGroup::setText(int id, const QString &text)
{
    if (id < 1 || id > buttonNum) return;
    group->button(id)->setText(text);
}

void ButtonGroup::setText(const QStringList &texts)
{
    auto buttons = group->buttons();
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

ButtonGroup::ButtonType ButtonGroup::type() const
{
    return buttonType;
}

ButtonGroup::ButtonGroup(QObject *parent):QObject(parent)
{
    group = new QButtonGroup;
    buttonNum = 2;
    buttonType = RadioBtn;
    for(int i = 1; i <= buttonNum; ++i){
        auto btn = new RadioButton;
        group->addButton(btn,i); // 注意下标从1开始
    }
    setChecked(1,true);
    connect(group,QOverload<int>::of(&QButtonGroup::buttonClicked),this,&ButtonGroup::buttonClicked);
}

ButtonGroup::ButtonGroup(unsigned num,ButtonType type,QObject *parent):QObject(parent)
{
    group = new QButtonGroup;
    if (num < 2)
        buttonNum = 2; // 至少2个才有互斥的意义
    else buttonNum = num;
    buttonType = type;

    switch (buttonType) {
        case RadioBtn:
            for(int i = 1; i <= buttonNum; ++i){
                auto btn = new RadioButton;
                group->addButton(btn,i); // 注意下标从1开始
            }
            break;
        case PushBtn:
            for(int i = 1; i <= buttonNum; ++i){
                auto btn = new PushButton;
                group->addButton(btn,i); // 注意下标从1开始
            }
            break;
    }

    setChecked(1,true);
    connect(group,QOverload<int>::of(&QButtonGroup::buttonClicked),this,&ButtonGroup::buttonClicked);
}