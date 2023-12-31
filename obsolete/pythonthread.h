/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-18 10:42:11
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-14 15:56:26
 * @FilePath: \EOS\obsolete\pythonthread.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOSI_PYTHONTHREAD_H
#define EOSI_PYTHONTHREAD_H

#include "window.h"
#include "Python.h"

class INTERFACE_IMEXPORT PythonThread : public QThread
{
    Q_OBJECT
public:
    explicit PythonThread(const QString&path,const QString&file,const QString&func,QObject *parent = nullptr);
    void setFlag(bool enable = false);
    ~PythonThread();
    void run() override;
private:
    QString prefixPath;
    QString fileName;
    QString funcName;
    bool flag = true;
};
#endif //EOSI_PYTHONTHREAD_H
