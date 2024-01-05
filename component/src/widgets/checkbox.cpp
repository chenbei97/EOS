#include "checkbox.h"

CheckBox::CheckBox(QWidget*parent): QWidget(parent)
{
    checkbox = new QCheckBox();
    setChecked(false);

    auto lay = new QVBoxLayout;
    lay->addWidget(checkbox);
    lay->setMargin(0);
    lay->setSpacing(0);
    setLayout(lay);

    connect(checkbox,&QCheckBox::stateChanged,this,&CheckBox::onStateChanged);

    //setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
}

CheckBox::CheckBox(const QString &text, QWidget *parent)
{
    checkbox = new QCheckBox(text);
    setChecked(false);

    auto lay = new QVBoxLayout;
    lay->addWidget(checkbox);
    lay->setMargin(0);
    lay->setSpacing(0);
    setLayout(lay);

    connect(checkbox,&QCheckBox::stateChanged,this,&CheckBox::onStateChanged);
    //setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
}

CheckBox::CheckBox(const QString &text, bool checked, QWidget *parent)
{
    checkbox = new QCheckBox(text);
    setChecked(checked);

    auto lay = new QVBoxLayout;
    lay->addWidget(checkbox);
    lay->setMargin(0);
    lay->setSpacing(0);
    setLayout(lay);

    connect(checkbox,&QCheckBox::stateChanged,this,&CheckBox::onStateChanged);
}

void CheckBox::onStateChanged(int state)
{
    if (state == Qt::Checked) emit checkedChanged(true);
    else if (state == Qt::Unchecked) emit checkedChanged(false);
}

void CheckBox::setText(const QString& text)
{
    checkbox->setText(text);
}

void CheckBox::setChecked(bool checked)
{
    checked?checkbox->setCheckState(Qt::Checked):
    checkbox->setCheckState(Qt::Unchecked);
}

bool CheckBox::isChecked() const
{
    return checkbox->isChecked();
}

QString CheckBox::text() const
{
    return checkbox->text();
}

//QSize CheckBox::sizeHint() const
//{
//    return QSize(30,30);
//}