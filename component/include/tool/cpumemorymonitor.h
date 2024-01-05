/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2024-01-04 17:15:36
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2024-01-04 17:29:51
 * @FilePath: \EOS\component\include\tool\cpumemorymonitor.h
 * @Copyright (c) 2024 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_CPUMEMORYMONITOR_H
#define EOS_CPUMEMORYMONITOR_H

#include "tool.h"
#include "qprocess.h"
#include "qtimer.h"

class COMPONENT_IMEXPORT CpuMemoryMonitor: public QObject
{
    Q_OBJECT
public:
    static CpuMemoryMonitor& instance();
public slots:
    void start(int interval);//开始启动服务
    void stop();//停止服务
private:
    explicit CpuMemoryMonitor(QObject*parent = nullptr);
    ~CpuMemoryMonitor();
private slots:
    void getCPU();          //获取cpu
    void getMemory();       //获取内存
    void readData();        //读取数据
    void setData();         //设置数据
private:
    quint64 cpuPercent;     //CPU百分比
    quint64 memoryPercent;  //内存百分比
    quint64 memoryAll;      //所有内存
    quint64 memoryUse;      //已用内存
    quint64 memoryFree;     //空闲内存

    QTimer *timerCPU;       //定时器获取CPU信息
    QTimer *timerMemory;    //定时器获取内存信息
    QProcess *process;      //执行命令行
private:
    quint64 totalNew, idleNew, totalOld, idleOld;
signals:
    void monitorValue(quint64 cpuPercent, quint64 memoryPercent,
                      quint64 memoryAll, quint64 memoryUse,quint64 memoryFree);
};
#define CpuMemoryMonitorPointer (&CpuMemoryMonitor::instance())
#endif //EOS_CPUMEMORYMONITOR_H
