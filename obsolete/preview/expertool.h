/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-14 13:26:28
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-14 14:51:58
 * @FilePath: \EOS\interface\include\preview\expertool.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_EXPERTOOL_H
#define EOS_EXPERTOOL_H

#include "window.h"
#include "timebox.h"
#include "zstackbox.h"
#include "savebox.h"
#include "focusbox.h"

class INTERFACE_IMEXPORT ExperTool: public QWidget
{
    Q_OBJECT
public:
    explicit ExperTool(QWidget*parent= nullptr);
    TimeInfo timeInfo() const;
    ExperToolInfo toolInfo() const;
    FocusInfo focusInfo() const;
    bool isAutoFocus() const;
    QStringList currentSelectedChannels() const;
    void importExperConfig(const QVariantMap& m);
private:
    TimeBox * timebox;
    ZStackBox * zstackbox;
    SaveBox * savebox;
    FocusBox * focusbox;
    QVBoxLayout * lay;
signals:
    void zstackChanged(bool isChecked);// 是否勾选zstack和stitch
    void stitchChanged(bool isChecked);
    void exportFilePath(const QString&path); // 导出文件配置的路径
    void loadExper(); // 启动实验
    void stopExper();
signals:
    void objectiveChanged(const QString& obj);
};

#endif //EOS_EXPERTOOL_H
