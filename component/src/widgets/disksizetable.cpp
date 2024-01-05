/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2024-01-05 11:40:04
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2024-01-05 15:15:47
 * @FilePath: \EOS\component\src\widgets\disksizetable.cpp
 * @Copyright (c) 2024 by ${chenbei}, All Rights Reserved. 
 */

#include "disksizetable.h"
#include "utilities.h"

#ifdef Q_OS_WIN
#include "windows.h"
#endif

DiskSizeTable::DiskSizeTable(QWidget *parent) : QTableWidget(parent)
{
    QStringList headText = {"Disk","UsedSpace","FreeSpace","TotalSpace","Percent"} ;
    this->clear();
    this->setColumnCount(5);
    this->setHorizontalHeaderLabels(headText);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSelectionMode(QAbstractItemView::NoSelection);
    this->verticalHeader()->setVisible(true);
    this->horizontalHeader()->setStretchLastSection(true);
    this->verticalHeader()->setStretchLastSection(true); // 延展好看,否则下方有一些空白空间
    //this->verticalHeader()->setDefaultSectionSize(60);

#ifdef Q_OS_WIN
    QMetaObject::invokeMethod(this, "load", Qt::QueuedConnection);
#elif defined(Q_OS_UNIX) && !defined(Q_OS_WASM)
    process = new QProcess(this);
    connect(process, SIGNAL(readyRead()), this, SLOT(readData()));
    process->start("df", QStringList() << "-h");
#endif
    setContentsMargins(QMargins(0,0,0,0));
}

//QSize DiskSizeTable::sizeHint() const
//{
//    return QSize(200, 150);
//}
//
//QSize DiskSizeTable::minimumSizeHint() const
//{
//    return QSize(160, 120);
//}

void DiskSizeTable::insertSize(const QString &name, const QString &use,
                               const QString &free, const QString &all,int percent)
{
    int row = this->rowCount();
    this->insertRow(row);
    QTableWidgetItem *itemname = new QTableWidgetItem(name);
    itemname->setTextAlignment(Qt::AlignCenter);
    QTableWidgetItem *itemuse = new QTableWidgetItem(use);
    itemuse->setTextAlignment(Qt::AlignCenter);
    QTableWidgetItem *itemfree = new QTableWidgetItem(free);
    itemfree->setTextAlignment(Qt::AlignCenter);
    QTableWidgetItem *itemall = new QTableWidgetItem(all);
    itemall->setTextAlignment(Qt::AlignCenter);

    this->setItem(row, 0, itemname);
    this->setItem(row, 1, itemuse);
    this->setItem(row, 2, itemfree);
    this->setItem(row, 3, itemall);

    QProgressBar *bar = new QProgressBar;
    bar->setRange(0, 100);
    bar->setValue(percent);
    this->setCellWidget(row, 4, bar);
    LOG<<"valid driver:"<<name<<use<<free<<all<<percent<<"%"<<row<<rowCount();
}

#ifdef Q_OS_WIN
void DiskSizeTable::load()
{
    //清空原有数据
    int row = this->rowCount();
    for (int i = 0; i < row; ++i) {
        this->removeRow(0);
    }
    QFileInfoList list = QDir::drives();
    LOG<<"all drivers: "<<list;
    for (auto dir:  list) {
        auto dirName = dir.absolutePath().toUtf8(); // toLocal也可以,不需要额外转(LPCSTR)了
        if (dir.absolutePath().contains("w",Qt::CaseInsensitive)
            ||dir.absolutePath().contains("x",Qt::CaseInsensitive)
            || dir.absolutePath().contains("y",Qt::CaseInsensitive)
            || dir.absolutePath().contains("z",Qt::CaseInsensitive)) {
            LOG<<"check str: skip driver"<<dirName;
            continue;
        }
        auto DType = GetDriveType(dirName); // 似乎此函数计算网络磁盘时就会很慢
        LOG<<"dirver: "<<dirName<<"type: "<<DType;
        if (DType == DRIVE_NO_ROOT_DIR) { // 网络磁盘
            LOG<<"check type: skip driver"<<dirName<<DType;
            continue;
        }

        if (DType == DRIVE_FIXED ) { // 硬盘或者U盘,|| DType == DRIVE_REMOVABLE U盘不打印
            ULARGE_INTEGER liFreeBytesAvailable, liTotalBytes, liTotalFreeBytes;
            if (GetDiskFreeSpaceExA(dirName,&liFreeBytesAvailable, &liTotalBytes, &liTotalFreeBytes)) {
                QString use = QString::number((double)(liTotalBytes.QuadPart - liTotalFreeBytes.QuadPart) / GB, 'f', 1);
                use += "G";
                QString free = QString::number((double) liTotalFreeBytes.QuadPart / GB, 'f', 1);
                free += "G";
                QString all = QString::number((double) liTotalBytes.QuadPart / GB, 'f', 1);
                all += "G";
                int percent = 100 - ((double)liTotalFreeBytes.QuadPart / liTotalBytes.QuadPart) * 100;
                insertSize(dirName, use, free, all, percent);
            } else {
                LOG<<"get disk free space ex failed";
            }
        }
    }
}
#endif

#if defined(Q_OS_UNIX) && !defined(Q_OS_WASM)
void DiskSizeTable::checkSize(const QString &result, const QString &name)
{
    QString dev, use, free, all;
    int percent = 0;
    QStringList list = result.split(" ");
    int index = 0;

    for (int i = 0; i < list.count(); ++i) {
        QString s = list.at(i).trimmed();
        if (s.isEmpty()) {
            continue;
        }

        index++;
        if (index == 1) {
            dev = s;
        } else if (index == 2) {
            all = s;
        } else if (index == 3) {
            use = s;
        } else if (index == 4) {
            free = s;
        } else if (index == 5) {
            percent = s.left(s.length() - 1).toInt();
            break;
        }
    }

    if (name.length() > 0) {
        dev = name;
    }

    insertSize(dev, use, free, all, percent);
}

void DiskSizeTable::readData()
{
    while (!process->atEnd()) {
        QString result = QLatin1String(process->readLine());
#ifdef __arm__
        if (result.startsWith("/dev/root")) {
            checkSize(result, "本地存储");
        } else if (result.startsWith("/dev/mmcblk")) {
            checkSize(result, "本地存储");
        } else if (result.startsWith("/dev/mmcblk1p")) {
            checkSize(result, "SD卡");
            QStringList list = result.split(" ");
            Q_EMIT sdcardReceive(list.at(0));
        } else if (result.startsWith("/dev/sd")) {
            checkSize(result, "U盘");
            QStringList list = result.split(" ");
            Q_EMIT udiskReceive(list.at(0));
        }
#else
        if (result.startsWith("/dev/sd")) {
            checkSize(result, "");
            QStringList list = result.split(" ");
            Q_EMIT udiskReceive(list.at(0));
        }
#endif
    }
}
#endif