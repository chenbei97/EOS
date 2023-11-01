/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-30 16:03:47
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-01 16:15:40
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
    setTitle(tr("实验"));
    timeInfo();
}

void TimeBox::disableChannel(const QString &obj)
{ // obj = br4x,ph4x
    if (obj.contains(ObjectiveBR)) {
        phbox->setEnabled(false);
        brbox->setEnabled(true);
    } else if (obj.contains(ObjectivePH)) {
        phbox->setEnabled(true);
        brbox->setEnabled(false);
    } else {
        // 可能是无镜头 none 对通道的勾选没有影响
    }
}

TimeInfo TimeBox::timeInfo() const
{
    TimeInfo info;

    info[TotalTimeField] = QString::number(totalTime());
    info[DurationTimeField] = QString::number(durationTime());
    info[IsScheduleField] = isSchedule();
    info[StartTimeField] = startTime().toString(DefaultDateTimeFormat);
    QString channels = QString("%1,%2,%3,%4,%5").arg(brbox->isChecked()).arg(phbox->isChecked())
            .arg(gfpbox->isChecked()).arg(rfpbox->isChecked()).arg(dapibox->isChecked());
    info[ChannelField] = channels;

    return info;
}

void TimeBox::refreshInfo()
{
    // 1. 检查时间信息设置是否正确
    if (!checkTime()) return;

    // 2. 预计的总拍照次数 = 总时长/间隔
    auto count = totalTime() / durationTime(); // 向下取整
    QDateTime datetime;
    if (isSchedule())
        datetime = datetimeedit->dateTime();
    else datetime = QDateTime::currentDateTime(); // 立即扫描是基于当前时间进行计算
    auto end = datetime.addSecs(totalTime()).toString(DefaultDateTimeFormat);
    tipinfo->setText(tr("预计结束时间: %1  总扫描次数: %2").arg(end).arg(count));
}

void TimeBox::toggleScanType(bool isSchedule)
{// 切换计划执行或立即执行
    isSchedule? datetimeedit->setEnabled(true):datetimeedit->setEnabled(false);
    refreshInfo(); // 如果是立即开始要更新
}

void TimeBox::updateDateTimeEdit()
{
    //if (!isSchedule()) return;//禁用状态也还是给更新
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
    scantype = new CheckBox(tr("计划执行?"),true);
    datetimeedit = new QDateTimeEdit(QDateTime::currentDateTime());
    tipinfo = new Label(tr("预计结束时间: %1  总扫描次数: %2").arg(0).arg(0));

    totaltime = new SpinBox;
    durationtime = new SpinBox;
    totalunit = new ComboBox(TimeBoxTimeUnitFields);
    durationunit = new ComboBox(TimeBoxTimeUnitFields);

    brbox = new CheckBox(BRField);
    phbox = new CheckBox(PHField);
    gfpbox = new CheckBox(GFPField);
    rfpbox = new CheckBox(RFPField);
    dapibox = new CheckBox(DAPIField);
}

void TimeBox::initAttributes()
{
    datetimeedit->setMinimumDateTime(QDateTime::currentDateTime());

    totaltime->setRange(1,LONG_MAX);
    totaltime->setSuffix(HoursFieldSuffix);
    totaltime->setValue(1.0);

    durationtime->setRange(1,LONG_MAX);
    durationtime->setSuffix(HoursFieldSuffix);
    durationtime->setValue(1.0);

    // objectivesetting,默认是br4x,故objectivebox默认是br4x,故默认brbox是启用,phbox禁用
    brbox->setEnabled(true);
    phbox->setEnabled(false);
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

    auto boxlay = new QHBoxLayout;
    boxlay->addWidget(new Label(tr("实验通道: ")));
    boxlay->addWidget(brbox);
    boxlay->addWidget(phbox);
    boxlay->addWidget(gfpbox);
    boxlay->addWidget(rfpbox);
    boxlay->addWidget(dapibox);
    boxlay->addStretch();
    boxlay->setSpacing(10);

    auto lay = new QVBoxLayout;
    lay->addLayout(totallay);
    lay->addLayout(durlay);
    lay->addLayout(boxlay);
    lay->addWidget(scantype);
    lay->addWidget(datetimeedit);
    lay->addWidget(tipinfo);

    setLayout(lay);
}

bool TimeBox::isSchedule() const
{
    return scantype->isChecked();
}

bool TimeBox::checkTime() const
{
    // 1. 取实验时长转为秒
    auto total = totalTime();

    // 2. 取实验间隔转为秒
    auto duration = durationTime();

    // 3. 间隔时间不能小于拍图时间，这个拍图时间是后端给定的，例如30mins
    if (duration<TimeBoxPhotoTimeLimit) {
        tipinfo->setText(TimeBoxPhotoTimeWaring);
        return false;
    }

    // 4.总实验时长要大于实验间隔时间,不然不能完成实验
    if (total<duration) {
        tipinfo->setText(TimeBoxTotalBeyondDurationWaring);
        return false;
    }
    return true;
}

long long TimeBox::totalTime() const
{
    long long total = 0;
    if (totalunit->currentText() == HourField)
        total = totaltime->value() * 3600;
    else
        total = totaltime->value() * 60;

    return total;
}

long long TimeBox::durationTime() const
{
    long long duration = 0;
    if (durationunit->currentText()==HourField)
        duration = durationtime->value() * 3600;
    else
        duration = durationtime->value()  * 60;

    return duration;
}

QDateTime TimeBox::startTime() const
{
    if (!isSchedule()) return QDateTime();

    auto datetime = datetimeedit->dateTime();
    if (datetime < QDateTime::currentDateTime()) {
        return QDateTime::currentDateTime(); // 有可能用户设置时间后又做了一些别的事导致这个时间过时了
    }
    return datetime;
}