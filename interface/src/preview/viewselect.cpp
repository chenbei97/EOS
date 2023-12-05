
#include "viewselect.h"

ViewSelect::ViewSelect(QWidget *parent) : GroupBox(parent)
{
    pointMode = new QRadioButton(tr(PointModeField));
    rectMode = new QRadioButton(tr(RectModeField));
    wholeMode = new QRadioButton(tr(WholeModeField));
    pointMode->setChecked(true);

    auto lay = new QHBoxLayout;
    lay->addWidget(pointMode);
    lay->addWidget(rectMode);
    lay->addWidget(wholeMode);

    setLayout(lay);
    setTitle(tr(ViewSelectBoxTitle));

    auto group = new QButtonGroup;
    group->addButton(pointMode,0);
    group->addButton(rectMode,1);
    group->addButton(wholeMode,2);

    connect(group,QOverload<int>::of(&QButtonGroup::buttonClicked),this, &ViewSelect::modeSelected);
}