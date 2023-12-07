#pragma once

#include "data_global.h"
#include <QQuickView>
#include <QWidget>
#include <QQuickWidget>

class DATA_EXPORT dataWidget : public QWidget
{
    Q_OBJECT
public:
    dataWidget(QWidget* parent = nullptr);

    QQuickView* GetMainQuickView();

    QQuickWidget* ShowMainView();

    QWidget* ShowView();

private:
    QQuickView mMainView;
    QQuickWidget* mMainWidget;
    QWidget* mWidget;
};
