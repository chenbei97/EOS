#pragma once

#include "teatlibnew_global.h"
#include "qquickview.h"
#include "qquickwidgets.h"
#include "qwidget.h"

class TEATLIBNEW_EXPORT teatLibNew : public QWidget
{
    Q_OBJECT
public:
    teatLibNew(QWidget* parent = nullptr);

    QQuickView* GetMainQuickView();

    QQuickWidget* ShowMainView();

    QWidget* ShowView();

private:
    QQuickView mMainView;
    QQuickWidget* mMainWidget;
    QWidget* mWidget;
};
