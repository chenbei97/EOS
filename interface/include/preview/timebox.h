/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-30 16:03:32
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-30 16:06:56
 * @FilePath: \EOS\interface\include\preview\timebox.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOSI_TIMEBOX_H
#define EOSI_TIMEBOX_H

#include "window.h"

#define use_timerthread 1
class INTERFACE_IMEXPORT TimeBox: public GroupBox
{
    Q_OBJECT
public:
    explicit TimeBox(QWidget*parent= nullptr);
    ~TimeBox();
    void importExperConfig(bool is_schedule,long long total,long long duration,
                           const QStringList&channels,const QString& objective);
    bool checkTime() const;
    bool isSchedule() const;
    long long totalTime() const;
    long long durationTime() const;
    QDateTime startTime() const;
    TimeInfo timeInfo() const;
public slots:
    void disableChannel(const QString &obj);
private:
    void initObjects();
    void initAttributes();
    void initConnections();
    void initLayout();
    void toggleScanType(bool isSchedule);//切换计划执行或立即执行
    void refreshInfo();//dutation/total/start时间更新时刷新提示信息
    void updateDateTimeEdit();
    void updateTotalTimeUnit(const QString& unit);
    void updateDurationTimeUnit(const QString& unit);
private:
    CheckBox * scantype;
    QDateTimeEdit * datetimeedit;
    SpinBox * totaltime;
    SpinBox * durationtime;
    ComboBox * totalunit;
    ComboBox * durationunit;
    CheckBox * brbox;
    CheckBox * phbox;
    CheckBox * gfpbox;
    CheckBox * rfpbox;
    CheckBox * dapibox;
    Label * tipinfo;
#ifdef use_timerthread // 定时更新datetimeedit
    TimerBroadCastThread timerthread;
#else
    QTimer timer;
#endif
signals:

};

#endif //EOSI_TIMEBOX_H
