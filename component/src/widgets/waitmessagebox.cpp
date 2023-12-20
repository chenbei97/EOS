/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-12-13 13:01:49
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-13 13:03:50
 * @FilePath: \EOS\component\src\widgets\waitmessagebox.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "waitmessagebox.h"

WaitMessageBox::WaitMessageBox(QWidget *parent): QMessageBox(parent)
{
    setWindowModality(Qt::NonModal);
    setStandardButtons(QMessageBox::NoButton);
    setWindowFlags(windowFlags()& ~Qt::WindowCloseButtonHint&~Qt::WindowContextHelpButtonHint);
    waitText = WaitMessageBoxMsg;
    timer.setInterval(0);
    connect(&timer,&QTimer::timeout,this,&WaitMessageBox::updateText);
    resize(DefaultWindowSize*2);
}

void WaitMessageBox::updateText()
{
    auto s = elapsedTimer.elapsed() / 1000.0;
    setText(QString("%1 [%2 s]").arg(waitText).arg(s));
}

void WaitMessageBox::setWaitText(const QString& text)
{
    waitText = text;
    setText(text);
}

void WaitMessageBox::wait()
{
    elapsedTimer.restart();
    timer.start();
    exec();
    //show();
    timer.stop();
}

void WaitMessageBox::wait(unsigned count)
{
    if (count == 0)
        elapsedTimer.restart();
    timer.start();
    exec();
    timer.stop();
}
