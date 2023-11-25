/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 14:53:07
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-16 15:10:37
 * @FilePath: \EOS\component\include\datastructure\eventloop.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_EVENTLOOP_H
#define EOSI_EVENTLOOP_H

#include "datastructure.h"

class COMPONENT_IMEXPORT EventLoop : public QObject
{
    Q_OBJECT
private:
    QEventLoop loop;
public:
    explicit EventLoop(QObject *parent = nullptr);
    bool isRunning() const;
public slots:
    void quit();
    void exec();
signals:
    void started();
};

#endif //EOSI_EVENTLOOP_H
