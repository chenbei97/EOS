/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-10 15:08:37
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-10 15:39:29
 * @FilePath: \EOS\component\src\widgets\windowinfo.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "windowinfo.h"
#include "windows.h"

static WindowsMemoryInfo getWindowsMemoryInfo()
{
    uint mb = 1024 * 1024;
    MEMORYSTATUSEX memoryInfo;
    memoryInfo.dwLength = sizeof(memoryInfo);
    GlobalMemoryStatusEx(&memoryInfo);

    WindowsMemoryInfo info;

    info.memoryUsageRate = QString::number(memoryInfo.dwMemoryLoad);
    info.totalPhysicalMemory = QString::number(memoryInfo.ullTotalPhys / mb);
    info.freePhysicalMemory = QString::number(memoryInfo.ullAvailPhys / mb);
    info.totalNumberOfSystemSubmissions = QString::number(memoryInfo.ullTotalPageFile / mb);
    info.numberOfSystemSubmissions =  QString::number(memoryInfo.ullTotalPageFile / mb - memoryInfo.ullAvailPageFile / mb);
    info.totalVirtualMemory =  QString::number(memoryInfo.ullTotalVirtual / mb);
    info.freeVirtualMemory =  QString::number(memoryInfo.ullAvailVirtual / mb);
    info.freeExtensionVirtualMemory =  QString::number(memoryInfo.ullAvailExtendedVirtual / mb);

    return info;
}

WindowInfo::WindowInfo(QWidget *parent) : QWidget(parent)
{
    memoryUsageRate = new Label; // 内存使用率
    totalPhysicalMemory = new Label;// 总物理内存
    freePhysicalMemory = new Label; // 可用物理内存
    totalNumberOfSystemSubmissions = new Label; // 系统提交总数
    numberOfSystemSubmissions = new Label;// 系统提交数
    totalVirtualMemory = new Label;// 总虚拟内存
    freeVirtualMemory = new Label;// 可用虚拟内存
    freeExtensionVirtualMemory = new Label;// 可用扩展虚拟内存

    totalVirtualMemory->hide();
    freeVirtualMemory->hide();
    freeExtensionVirtualMemory->hide();

    auto formlay = new QFormLayout;

    formlay->addRow(tr("内存使用率: "),memoryUsageRate);
    formlay->addRow(tr("总物理内存: "),totalPhysicalMemory);
    formlay->addRow(tr("可用物理内存: "),freePhysicalMemory);
    formlay->addRow(tr("系统提交总数: "),totalNumberOfSystemSubmissions);
    formlay->addRow(tr("系统提交数: "),numberOfSystemSubmissions);
//    formlay->addRow(tr("总虚拟内存: "),totalVirtualMemory);
//    formlay->addRow(tr("可用虚拟内存: "),freeVirtualMemory);
//    formlay->addRow(tr("可用扩展虚拟: "),freeExtensionVirtualMemory);
    setLayout(formlay);

    resize(350,200);

    connect(&timer,&QTimer::timeout,this,&WindowInfo::updateInfo);
    timer.start(1000);
}

void WindowInfo::updateInfo()
{
    auto info = getWindowsMemoryInfo();
    memoryUsageRate->setText(info.memoryUsageRate+"%"); // 内存使用率
    totalPhysicalMemory->setText(info.totalPhysicalMemory+"MB");// 总物理内存
    freePhysicalMemory->setText(info.freePhysicalMemory+"MB"); // 可用物理内存
    totalNumberOfSystemSubmissions->setText(info.totalNumberOfSystemSubmissions);// ; // 系统提交总数
    numberOfSystemSubmissions->setText(info.numberOfSystemSubmissions);// 系统提交数
    totalVirtualMemory->setText(info.totalVirtualMemory+"MB");// 总虚拟内存
    freeVirtualMemory->setText(info.freeVirtualMemory+"MB");// 可用虚拟内存
    freeExtensionVirtualMemory->setText(info.freeExtensionVirtualMemory+"MB");// 可用扩展虚拟内存
}