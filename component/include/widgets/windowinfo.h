/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-10 15:08:26
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-10 15:39:43
 * @FilePath: \EOS\component\include\widgets\windowinfo.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_MAIN_CORE_PY_WINDOWINFO_H
#define EOS_MAIN_CORE_PY_WINDOWINFO_H

#include "widgets.h"
#include "utilities.h"
#include "label.h"
#include "qtimer.h"

class COMPONENT_IMEXPORT WindowInfo : public QWidget
{
    Q_OBJECT
public:
    explicit WindowInfo(QWidget*parent= nullptr);
private:
    Label * memoryUsageRate; // 内存使用率
    Label * totalPhysicalMemory;// 总物理内存
    Label * freePhysicalMemory; // 可用物理内存
    Label * totalNumberOfSystemSubmissions; // 系统提交总数
    Label * numberOfSystemSubmissions;// 系统提交数
    Label * totalVirtualMemory;// 总虚拟内存
    Label * freeVirtualMemory;// 可用虚拟内存
    Label * freeExtensionVirtualMemory;// 可用扩展虚拟内存
    QTimer timer;
    void updateInfo();
};
#endif //EOS_MAIN_CORE_PY_WINDOWINFO_H
