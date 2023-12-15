#include "channelboxx.h"

ChannelBoxx::ChannelBoxx(QWidget*parent): GroupBox(parent)
{
    brbtn = new PushButton(BRField);
    phbtn = new PushButton(PHField);
    gfpbtn = new PushButton(GFPField);
    rfpbtn = new PushButton(RFPField);
    dapibtn = new PushButton(DAPIField);
    canvas = new ChannelPicture;
#ifdef use_channelnotifier
    notifier = new ChannelNotifier;
#endif

    // objectivesetting,默认是br4x,故objectivebox默认是br4x,故默认brbox是启用,phbox禁用
    brbtn->setEnabled(true);
    phbtn->setEnabled(false);

    auto lay = new QHBoxLayout;

    lay->addWidget(brbtn);
    lay->addWidget(phbtn);
    lay->addWidget(gfpbtn);
    lay->addWidget(rfpbtn);
    lay->addWidget(dapibtn);

    auto mainlay = new QVBoxLayout;
    mainlay->addLayout(lay);
    mainlay->addWidget(canvas);
    setLayout(mainlay);
    setTitle(tr(ChannelBoxTitle));

#ifdef use_channelnotifier
    QButtonGroup * group = new QButtonGroup;
    group->addButton(brbtn,0);
    group->addButton(phbtn,1);
    group->addButton(gfpbtn,2);
    group->addButton(rfpbtn,3);
    group->addButton(dapibtn,4);
    brbtn->setChecked(true);

    brbtn->setID(0);
    phbtn->setID(1);
    gfpbtn->setID(2);
    rfpbtn->setID(3);
    dapibtn->setID(4);

    notifier->addToList(brbtn);
    notifier->addToList(phbtn);
    notifier->addToList(gfpbtn);
    notifier->addToList(rfpbtn);
    notifier->addToList(dapibtn);
    connect(group,QOverload<int>::of(&QButtonGroup::buttonClicked),this,&ChannelBoxx::toggleChannel);
    connect(notifier,&ChannelNotifier::channelChanged,this,&ChannelBoxx::channelChanged);
    connect(notifier,&ChannelNotifier::channelClosed,this,&ChannelBoxx::channelClosed);
#else
    connect(brbtn,&PushButton::clicked,this,&ChannelBoxx::clickBr);
    connect(phbtn,&PushButton::clicked,this,&ChannelBoxx::clickPh);
    connect(rfpbtn,&PushButton::clicked,this,&ChannelBoxx::clickRfp);
    connect(gfpbtn,&PushButton::clicked,this,&ChannelBoxx::clickGfp);
    connect(dapibtn,&PushButton::clicked,this,&ChannelBoxx::clickDapi);
#endif
}

#ifdef use_channelnotifier
void ChannelBoxx::toggleChannel(int option)
{ // 使用通知者模式减少大量的重复代码,把clickxxx函数的代码写在notify函数内
    switch (option)
    {
        case 0:
            notifier->notify(brbtn);
            break;
        case 1:
            notifier->notify(phbtn);
            break;
        case 2:
            notifier->notify(gfpbtn);
            break;
        case 3:
            notifier->notify(rfpbtn);
            break;
        case 4:
            notifier->notify(dapibtn);
            break;
    }
}
#else

void ChannelBoxx::clickBr()
{
    !brbtn->isChecked()?brbtn->setBackGroundColor(Qt::yellow):brbtn->resetBackGroundColor();
    phbtn->resetBackGroundColor();
    rfpbtn->resetBackGroundColor();
    gfpbtn->resetBackGroundColor();
    dapibtn->resetBackGroundColor();
    brbtn->isChecked()? emit channelChanged(0): emit channelChanged(-1);
    if (!brbtn->isChecked()) {// 原来是true,要取消高亮和关灯,高亮已经变成了true,关灯要用取反状态
        emit channelClosed(0); // 把下方代码转移到preview_tcp,方便维护
//        QVariantMap m;
//        m[CurrentChannelField] = 0;
//        m[TurnOffLight] = 1;
//        m[BrightField] = -1;
//        SocketPointer->exec(TcpFramePool.toggleChannelEvent,assembleToggleChannelEvent(m),true);
//        if (ParserResult.toBool()) {
//            LOG<<"close br channel";
//        }
    }
}

void ChannelBoxx::clickPh()
{
    brbtn->resetBackGroundColor();
    !phbtn->isChecked()?phbtn->setBackGroundColor(Qt::yellow):phbtn->resetBackGroundColor();
    rfpbtn->resetBackGroundColor();
    gfpbtn->resetBackGroundColor();
    dapibtn->resetBackGroundColor();
    phbtn->isChecked()? emit channelChanged(1): emit channelChanged(-1);
    if (!phbtn->isChecked()) {
        emit channelClosed(1);
//        QVariantMap m;
//        m[CurrentChannelField] = 1;
//        m[TurnOffLight] = 1;
//        m[BrightField] = -1;
//        SocketPointer->exec(TcpFramePool.toggleChannelEvent,assembleToggleChannelEvent(m),true);
//        if (ParserResult.toBool()) {
//            LOG<<"close ph channel";
//        }
    }
}

void ChannelBoxx::clickGfp()
{
    brbtn->resetBackGroundColor();
    phbtn->resetBackGroundColor();
    !gfpbtn->isChecked()?gfpbtn->setBackGroundColor(Qt::yellow):gfpbtn->resetBackGroundColor();
    rfpbtn->resetBackGroundColor();
    dapibtn->resetBackGroundColor();
    gfpbtn->isChecked()? emit channelChanged(2): emit channelChanged(-1);
    if (!gfpbtn->isChecked()) {
        emit channelClosed(2);
//        QVariantMap m;
//        m[CurrentChannelField] = 2;
//        m[TurnOffLight] = 1;
//        m[BrightField] = -1;
//        SocketPointer->exec(TcpFramePool.toggleChannelEvent,assembleToggleChannelEvent(m),true);
//        if (ParserResult.toBool()) {
//            LOG<<"close gfp channel";
//        }
    }
}

void ChannelBoxx::clickRfp()
{
    brbtn->resetBackGroundColor();
    phbtn->resetBackGroundColor();
    gfpbtn->resetBackGroundColor();
    !rfpbtn->isChecked()?rfpbtn->setBackGroundColor(Qt::yellow):rfpbtn->resetBackGroundColor();
    dapibtn->resetBackGroundColor();
    rfpbtn->isChecked()? emit channelChanged(3): emit channelChanged(-1);
    if (!rfpbtn->isChecked()) {
        emit channelClosed(3);
//        QVariantMap m;
//        m[CurrentChannelField] = 3;
//        m[TurnOffLight] = 1;
//        m[BrightField] = -1;
//        SocketPointer->exec(TcpFramePool.toggleChannelEvent,assembleToggleChannelEvent(m),true);
//        if (ParserResult.toBool()) {
//            LOG<<"close rfp channel";
//        }
    }
}

void ChannelBoxx::clickDapi()
{
    brbtn->resetBackGroundColor();
    phbtn->resetBackGroundColor();
    rfpbtn->resetBackGroundColor();
    gfpbtn->resetBackGroundColor();
    !dapibtn->isChecked()?dapibtn->setBackGroundColor(Qt::yellow):dapibtn->resetBackGroundColor();
    dapibtn->isChecked()? emit channelChanged(4): emit channelChanged(-1);

    if (!dapibtn->isChecked()) {
        emit channelClosed(4);
//        QVariantMap m;
//        m[CurrentChannelField] = 4;
//        m[TurnOffLight] = 1;
//        m[BrightField] = -1;
//        SocketPointer->exec(TcpFramePool.toggleChannelEvent,assembleToggleChannelEvent(m),true);
//        if (ParserResult.toBool()) {
//            LOG<<"close dapi channel";
//        }
    }
}
#endif

void ChannelBoxx::takePhoto(const QImage &img, const QString &channel)
{
    LOG<<"current channel is "<<channel;
    canvas->setImage(ChannelFields.indexOf(channel),img);
}

void ChannelBoxx::disableChannel(const QString &obj)
{ // obj = br10x, ph4x是兼容的老式字符串
    //LOG<<"objective = "<<obj;
    if (obj.contains(ObjectiveBR) || obj.contains(NAField)) {
        phbtn->setEnabled(false);
        phbtn->resetBackGroundColor();
        brbtn->setEnabled(true);
        // BR物镜-荧光通道使能要改回来
        gfpbtn->setEnabled(true);
        rfpbtn->setEnabled(true);
        dapibtn->setEnabled(true);
    } else if (obj.contains(ObjectivePH)) {
        phbtn->setEnabled(true);
        brbtn->setEnabled(false);
        brbtn->resetBackGroundColor();
        // PH物镜-新增荧光通道也不能选
        gfpbtn->setEnabled(false);
        rfpbtn->setEnabled(false);
        dapibtn->setEnabled(false);
    } else {
        // 可能是无镜头 none 对通道的勾选没有影响
    }
}

ChannelInfo ChannelBoxx::channelInfo() const
{
    ChannelInfo m;

    if (brbtn->isChecked())
        m[CurrentChannelField] = BRField; // 转换为idx无需在这里,这里需要实际字符串
    else if (phbtn->isChecked())
        m[CurrentChannelField] = PHField;
    else if (gfpbtn->isChecked())
        m[CurrentChannelField] = GFPField;
    else if (rfpbtn->isChecked())
        m[CurrentChannelField] = RFPField;
    else if (dapibtn->isChecked())
        m[CurrentChannelField] = DAPIField;

    return m;
}