/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2024-01-05 11:38:39
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2024-01-05 15:15:39
 * @FilePath: \EOS\component\include\widgets\disksizetable.h
 * @Copyright (c) 2024 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_DISKSIZETABLE_H
#define EOS_DISKSIZETABLE_H

#include "widgets.h"

class QProcess;

class COMPONENT_IMEXPORT DiskSizeTable: public QTableWidget
{
    Q_OBJECT
public:
    explicit DiskSizeTable(QWidget*parent= nullptr);
//    QSize sizeHint() const override;
//    QSize minimumSizeHint() const override;
private:
    void insertSize(const QString &name, const QString &use, const QString &free, const QString &all, int percent);
#ifdef Q_OS_WIN
    Q_INVOKABLE void load();
#elif defined(Q_OS_UNIX) && !defined(Q_OS_WASM)
    QProcess *process;
    void readData();
    void checkSize(const QString &result, const QString &name);
signals:
    void sdcardReceive(const QString &sdcardName);
    void udiskReceive(const QString &udiskName);
#endif
};

#endif //EOS_DISKSIZETABLE_H
