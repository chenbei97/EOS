/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-26 10:06:06
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-16 16:24:04
 * @FilePath: \EOS\component\src\widgets\dockwidget.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "dockwidget.h"

DockWidget::DockWidget(QWidget*parent): QDockWidget(parent)
{
    mWinSize = DefaultWindowSize;
    auto x = (UserScreenWidth-mWinSize.width())/2;
    auto y = (UserScreenHeight-mWinSize.height())/2;
    mWinPos = {x,y};
    mRetracement = false;
}

DockWidget::DockWidget(const QString&title,QWidget *parent) : QDockWidget(title,parent)
{
    mWinSize = DefaultWindowSize;
    auto x = (UserScreenWidth-mWinSize.width())/2;
    auto y = (UserScreenHeight-mWinSize.height())/2;
    mWinPos = {x,y};
    mRetracement = false;
}

bool DockWidget::event(QEvent* event)
{
    if(event->type() == QEvent::ZOrderChange)
    {
        if(isFloating())	//判断是否变为浮动窗口
        {
            resize(mWinSize);	//改变初始化大小
            auto x = (UserScreenWidth-mWinSize.width())/2;
            auto y = (UserScreenHeight-mWinSize.height())/2;
            mWinPos = {x,y};
            move(mWinPos);
            show();
        }
    }
    return QDockWidget::event(event);
}

void DockWidget::closeEvent(QCloseEvent* event)
{ // 如果显示浮动小窗口,恢复原位才需要设置mRetracement=true
    if (mRetracement) { // 这个要写在外头,否则关闭时会出现问题
        if (isFloating()) {
            setFloating(false);    //设置浮动标志为false
            //创建一个关闭浮动窗口的事件，发送给事件队列
            QEvent *e = new QEvent(QEvent::ZOrderChange);
            QApplication::sendEvent(this, e);
            event->ignore();    //最后忽略事件，不关闭窗口
        }
    }
    //setFloating(false);//可能是造成递归了
    return QDockWidget::closeEvent(event);
}

void DockWidget::setRetracement(bool enable)
{
    mRetracement = enable;
}

void DockWidget::setWindowPosition(int x, int y)
{
    mWinPos = {x,y};
    QApplication::sendEvent(this,new QEvent(QEvent::ZOrderChange));
}

void DockWidget::setWindowPosition(const QPoint &pos)
{
    mWinPos = pos;
    QApplication::sendEvent(this,new QEvent(QEvent::ZOrderChange));
}

void DockWidget::setWindowSize(int w, int h)
{
    mWinSize = {w,h};
    QApplication::sendEvent(this,new QEvent(QEvent::ZOrderChange));
}

