/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-30 16:03:47
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-30 16:06:48
 * @FilePath: \EOS\interface\src\preview\timebox.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "timebox.h"

TimeBox::TimeBox(QWidget *parent) : GroupBox(parent)
{
    initObjects();
    initAttributes();
    initConnections();
    initLayout();
    setTitle(tr("时间"));
}

void TimeBox::refreshInfo()
{
    // 1. 取实验时长转为秒
    long long total = 0;
    if (totalunit->currentText() == HourField)
        total = totaltime->value() * 3600;
    else
        total = totaltime->value() * 60;

    // 2. 取实验间隔转为秒
    long long duration = 0;
    if (durationunit->currentText()==HourField)
        duration = durationtime->value() * 3600;
    else
        duration = durationtime->value()  * 60;

    // 3. 间隔时间不能小于拍图时间，这个拍图时间是后端给定的，例如30mins
    if (duration<TimeBoxPhotoTimeLimit) {
        tipinfo->setText(TimeBoxPhotoTimeWaring);
        return;
    }

    // 4.总实验时长要大于实验间隔时间,不然不能完成实验
    if (total<duration) {
        tipinfo->setText(TimeBoxTotalBeyondDurationWaring);
        return;
    }

    // 5. 预计的总拍照次数 = 总时长/间隔
    auto count = total / duration; // 向下取整
    QDateTime datetime;
    if (scantype->isChecked())
        datetime = datetimeedit->dateTime();
    else datetime = QDateTime::currentDateTime(); // 立即扫描是基于当前时间进行计算
    auto end = datetime.addSecs(total).toString(DateTimeDefaultFormat);
    tipinfo->setText(tr("预计结束时间: %1  总扫描次数: %2").arg(end).arg(count));
}

void TimeBox::toggleScanType(bool isSchedule)
{// 切换计划执行或立即执行
    isSchedule? datetimeedit->setEnabled(true):datetimeedit->setEnabled(false);
    refreshInfo(); // 如果是立即开始要更新
}

void TimeBox::updateDateTimeEdit()
{
    if (!scantype->isChecked()) return;
    datetimeedit->setMinimumDateTime(QDateTime::currentDateTime());
}

void TimeBox::updateTotalTimeUnit(const QString &unit)
{
    QString suffix = unit==HourField?HoursFieldSuffix:MinutesFieldSuffix;
    totaltime->setSuffix(suffix);
    refreshInfo();
}

void TimeBox::updateDurationTimeUnit(const QString& unit)
{
    QString suffix = unit==HourField?HoursFieldSuffix:MinutesFieldSuffix;
    durationtime->setSuffix(suffix);
    refreshInfo();
}

void TimeBox::initObjects()
{
    scantype = new CheckBox(tr("计划执行?"));
    datetimeedit = new QDateTimeEdit(QDateTime::currentDateTime());
    tipinfo = new Label(tr("预计结束时间: %1  总扫描次数: %2").arg(0).arg(0));

    totaltime = new SpinBox;
    durationtime = new SpinBox;
    totalunit = new ComboBox(TimeBoxTimeUnitFields);
    durationunit = new ComboBox(TimeBoxTimeUnitFields);
}

void TimeBox::initAttributes()
{
    scantype->setChecked(true);
    datetimeedit->setMinimumDateTime(QDateTime::currentDateTime());

    totaltime->setRange(1,LONG_MAX);
    totaltime->setSuffix(HoursFieldSuffix);
    totaltime->setValue(1.0);

    durationtime->setRange(1,LONG_MAX);
    durationtime->setSuffix(HoursFieldSuffix);
    durationtime->setValue(1.0);
}

void TimeBox::initConnections()
{
    connect(scantype,&CheckBox::checkedChanged,this,&TimeBox::toggleScanType);
    connect(datetimeedit,&QDateTimeEdit::dateTimeChanged,this,&TimeBox::refreshInfo);
    connect(totaltime,QOverload<double>::of(&SpinBox::valueChanged),this,&TimeBox::refreshInfo);
    connect(durationtime,QOverload<double>::of(&SpinBox::valueChanged),this,&TimeBox::refreshInfo);
    connect(totalunit,QOverload<const QString&>::of(&ComboBox::currentIndexChanged),this,&TimeBox::updateTotalTimeUnit);
    connect(durationunit,QOverload<const QString&>::of(&ComboBox::currentIndexChanged),this,&TimeBox::updateDurationTimeUnit);
    connect(&timer,&QTimer::timeout,this,&TimeBox::updateDateTimeEdit);
    timer.start(1000); // 每秒更新1次
}

void TimeBox::initLayout()
{
    auto totallay = new QHBoxLayout;
    totallay->addWidget(new Label(tr("实验时长: ")));
    totallay->addWidget(totaltime);
    totallay->addWidget(totalunit);
    totallay->addStretch();

    auto durlay = new QHBoxLayout;
    durlay->addWidget(new Label(tr("实验间隔: ")));
    durlay->addWidget(durationtime);
    durlay->addWidget(durationunit);
    durlay->addStretch();

    auto lay = new QVBoxLayout;
    lay->addLayout(totallay);
    lay->addLayout(durlay);
    lay->addWidget(scantype);
    lay->addWidget(datetimeedit);
    lay->addWidget(tipinfo);

    setLayout(lay);
}