#ifndef EOSI_DOCKWIDGET_H
#define EOSI_DOCKWIDGET_H

#include "widgets.h"

class COMPONENT_IMEXPORT DockWidget: public QDockWidget
{
    Q_OBJECT
public:
    explicit DockWidget(QWidget*parent= nullptr);
    explicit DockWidget(const QString&title,QWidget*parent= nullptr);
    void setWindowSize(int w, int h);
    void setWindowPosition(int x,int y);
    void setWindowPosition(const QPoint& pos);
    void setRetracement(bool enable);
    bool event(QEvent* event) override;
    void closeEvent(QCloseEvent* event) override;
private:
    bool mRetracement = false;
    QSize mWinSize;
    QPoint mWinPos;
};
#endif //EOSI_DOCKWIDGET_H
