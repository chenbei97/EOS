/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2024-01-04 17:15:47
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2024-01-04 17:30:06
 * @FilePath: \EOS\component\src\tool\cpumemorymonitor.cpp
 * @Copyright (c) 2024 by ${chenbei}, All Rights Reserved. 
 */

#include "cpumemorymonitor.h"

#ifdef Q_OS_WIN
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x502
#endif
#include "windows.h"
#endif

void CpuMemoryMonitor::start(int interval)
{
    this->getCPU();
    this->getMemory();

    if (!timerCPU->isActive()) {
        timerCPU->start(interval);
    }
    if (!timerMemory->isActive()) {
        timerMemory->start(interval + 1000);
    }
}

void CpuMemoryMonitor::stop()
{
    process->close();
    if (timerCPU->isActive()) {
        timerCPU->stop();
    }
    if (timerMemory->isActive()) {
        timerMemory->stop();
    }
}

void CpuMemoryMonitor::getCPU()
{
#ifdef Q_OS_WIN
#if 0
    static FILETIME lastIdleTime;
    static FILETIME lastKernelTime;
    static FILETIME lastUserTime;

    FILETIME newIdleTime;
    FILETIME newKernelTime;
    FILETIME newUserTime;

    //采用GetSystemTimes获取的CPU占用和任务管理器的不一致
    GetSystemTimes(&newIdleTime, &newKernelTime, &newUserTime);

    int offset = 31;
    quint64 a, b;
    quint64 idle, kernel, user;

    a = (lastIdleTime.dwHighDateTime << offset) | lastIdleTime.dwLowDateTime;
    b = (newIdleTime.dwHighDateTime << offset) | newIdleTime.dwLowDateTime;
    idle = b - a;

    a = (lastKernelTime.dwHighDateTime << offset) | lastKernelTime.dwLowDateTime;
    b = (newKernelTime.dwHighDateTime << offset) | newKernelTime.dwLowDateTime;
    kernel = b - a;

    a = (lastUserTime.dwHighDateTime << offset) | lastUserTime.dwLowDateTime;
    b = (newUserTime.dwHighDateTime << offset) | newUserTime.dwLowDateTime;
    user = b - a;

    cpuPercent = float(kernel + user - idle) * 100 / float(kernel + user);

    lastIdleTime = newIdleTime;
    lastKernelTime = newKernelTime;
    lastUserTime = newUserTime;
    this->setData();
#else
    //获取系统版本区分win10
    bool win10 = false;
#if (QT_VERSION >= QT_VERSION_CHECK(5,4,0))
    win10 = (QSysInfo::productVersion().mid(0, 2).toInt() >= 10);
#else
    win10 = (QSysInfo::WindowsVersion >= 192);
#endif

    QString cmd = "\\Processor(_Total)\\% Processor Time";
    if (win10) {
        cmd = "\\Processor Information(_Total)\\% Processor Utility";
    }

    if (process->state() == QProcess::NotRunning) {
        process->start("typeperf", QStringList() << cmd);
    }
#endif

#elif defined(Q_OS_UNIX) && !defined(Q_OS_WASM)
    if (process->state() == QProcess::NotRunning) {
        totalNew = idleNew = 0;
        process->start("cat", QStringList() << "/proc/stat");
    }
#endif
}

void CpuMemoryMonitor::getMemory()
{
#ifdef Q_OS_WIN
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);
    memoryPercent = statex.dwMemoryLoad;
    memoryAll = statex.ullTotalPhys / MB;
    memoryFree = statex.ullAvailPhys / MB;
    memoryUse = memoryAll - memoryFree;
    this->setData();

#elif defined(Q_OS_UNIX) && !defined(Q_OS_WASM)
    if (process->state() == QProcess::NotRunning) {
        process->start("cat", QStringList() << "/proc/meminfo");
    }
#endif
}

void CpuMemoryMonitor::readData()
{
#ifdef Q_OS_WIN
    while (!process->atEnd()) {
        QString s = QLatin1String(process->readLine());
        s = s.split(",").last();
        s.replace("\r", "");
        s.replace("\n", "");
        s.replace("\"", "");
        if (!s.isEmpty() && s.length() < 10) {
            cpuPercent = qRound(s.toFloat());
        }
    }
#elif defined(Q_OS_UNIX) && !defined(Q_OS_WASM)
    while (!process->atEnd()) {
        QString s = QLatin1String(process->readLine());
        if (s.startsWith("cpu")) {
            QStringList list = s.split(" ");
            idleNew = list.at(5).toUInt();
            foreach (QString value, list) {
                totalNew += value.toUInt();
            }

            quint64 total = totalNew - totalOld;
            quint64 idle = idleNew - idleOld;
            cpuPercent = 100 * (total - idle) / total;
            totalOld = totalNew;
            idleOld = idleNew;
            break;
        } else if (s.startsWith("MemTotal")) {
            s.replace(" ", "");
            s = s.split(":").at(1);
            memoryAll = s.left(s.length() - 3).toUInt() / KB;
        } else if (s.startsWith("MemFree")) {
            s.replace(" ", "");
            s = s.split(":").at(1);
            memoryFree = s.left(s.length() - 3).toUInt() / KB;
        } else if (s.startsWith("Buffers")) {
            s.replace(" ", "");
            s = s.split(":").at(1);
            memoryFree += s.left(s.length() - 3).toUInt() / KB;
        } else if (s.startsWith("Cached")) {
            s.replace(" ", "");
            s = s.split(":").at(1);
            memoryFree += s.left(s.length() - 3).toUInt() / KB;
            memoryUse = memoryAll - memoryFree;
            memoryPercent = 100 * memoryUse / memoryAll;
            break;
        }
    }
#endif
    this->setData();
}

void CpuMemoryMonitor::setData()
{
    cpuPercent = (cpuPercent < 0 ? 0 : cpuPercent);
    cpuPercent = (cpuPercent > 100 ? 0 : cpuPercent);
    emit monitorValue(cpuPercent, memoryPercent, memoryAll, memoryUse, memoryFree);
}

CpuMemoryMonitor& CpuMemoryMonitor::instance()
{
    static CpuMemoryMonitor instance;
    return instance;
}

CpuMemoryMonitor::CpuMemoryMonitor(QObject *parent) : QObject(parent)
{
    totalNew = idleNew = totalOld = idleOld = 0;

    cpuPercent = 0;
    memoryPercent = 0;
    memoryAll = 0;
    memoryUse = 0;

    //获取CPU占用情况定时器
    timerCPU = new QTimer(this);
    connect(timerCPU, SIGNAL(timeout()), this, SLOT(getCPU()));

    //获取内存占用情况定时器
    timerMemory = new QTimer(this);
    connect(timerMemory, SIGNAL(timeout()), this, SLOT(getMemory()));

    //执行命令获取
    process = new QProcess(this);
    connect(process, SIGNAL(readyRead()), this, SLOT(readData()));
}

CpuMemoryMonitor::~CpuMemoryMonitor() noexcept
{
    this->stop();
}


