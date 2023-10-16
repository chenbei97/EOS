/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 14:17:22
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-16 15:09:26
 * @FilePath: \EOS\component\include\widgets\widgets.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_WIDGETS_H
#define EOSI_WIDGETS_H

// (1) 头文件

#include "qcombobox.h"
#include "qlineedit.h"
#include "qtextedit.h"
#include "qlabel.h"
#include "qgroupbox.h"
#include "qpushbutton.h"
#include "qsplitter.h"
#include "qspinbox.h"
#include "qprogressbar.h"

#include "qvalidator.h"
#include "qevent.h"
// (2) 常量定义
#define ProgressBarMinHeight 40
#define ProgressBarStyle ("QProgressBar{border:2px solid grey;border-radius:5px;text-align:center;}"\
                                    "QProgressBar::chunk{background-color: #CD96CD;width: 10px;margin: 0.5px;}")

// (3) 导出定义
#if defined(COMPONENT_LIBRARY)
#  define COMPONENT_IMEXPORT Q_DECL_EXPORT
#else
#  define COMPONENT_IMEXPORT Q_DECL_IMPORT
#endif


#endif //EOSI_WIDGETS_H