
#include "viewselect.h"

ViewSelect::ViewSelect(QWidget *parent) : GroupBox(parent)
{
    pointMode = new QRadioButton(tr(PointModeField));
    rectMode = new QRadioButton(tr(RectModeField));
    wholeMode = new QRadioButton(tr(WholeModeField));

    auto lay = new QHBoxLayout;
    lay->addWidget(pointMode);
    lay->addWidget(rectMode);
    lay->addWidget(wholeMode);

    setLayout(lay);
    setTitle(tr(ViewSelectBoxTitle));
}