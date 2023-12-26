/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-12-13 13:01:20
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-13 13:03:58
 * @FilePath: \EOS\component\include\widgets\waitmessagebox.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_WAITMESSAGEBOX_H
#define EOS_WAITMESSAGEBOX_H

#include "widgets.h"
//#include "qevent.h"
#define WaitMessageBoxMsg QObject::tr("please wait processing...")

class COMPONENT_IMEXPORT WaitMessageBox: public QMessageBox
{
    Q_OBJECT
public:
    explicit WaitMessageBox(QWidget*parent= nullptr);
    explicit WaitMessageBox(const QString& text,QWidget*parent= nullptr);
    void wait();
    void wait(unsigned count);
    void setWaitText(const QString& text);
    void showEvent(QShowEvent *e) override;
private:
    void updateText();
private:
    QString waitText;
    QElapsedTimer elapsedTimer;
    QTimer timer;
};

#endif //EOS_WAITMESSAGEBOX_H
