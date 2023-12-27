#include "channelbox.h"

ChannelBox::ChannelBox(QWidget*parent): GroupBox(parent)
{
    brchannel = new PushButton(BRField);
    phchannel = new PushButton(PHField);
    gfpchannel = new PushButton(GFPField);
    rfpchannel = new PushButton(RFPField);
    dapichannel = new PushButton(DAPIField);
    canvas = new ChannelPicture;
#ifdef use_channelnotifier
    notifier = new ChannelNotifier;
#endif

    // objectivesetting,默认是br4x,故objectivebox默认是br4x,故默认brbox是启用,phbox禁用
    brchannel->setEnabled(true);
    phchannel->setEnabled(false);

    auto lay = new QHBoxLayout;

    lay->addWidget(brchannel);
    lay->addWidget(phchannel);
    lay->addWidget(gfpchannel);
    lay->addWidget(rfpchannel);
    lay->addWidget(dapichannel);

    auto mainlay = new QVBoxLayout;
    mainlay->addLayout(lay);
    mainlay->addWidget(canvas);
    canvas->hide(); // 取消缩略图
    setLayout(mainlay);
    setTitle(tr(ChannelBoxTitle));

#ifdef use_channelnotifier
    QButtonGroup * group = new QButtonGroup;
    group->addButton(brchannel,0);
    group->addButton(phchannel,1);
    group->addButton(gfpchannel,2);
    group->addButton(rfpchannel,3);
    group->addButton(dapichannel,4);
    brchannel->setChecked(true);

    brchannel->setID(0);
    phchannel->setID(1);
    gfpchannel->setID(2);
    rfpchannel->setID(3);
    dapichannel->setID(4);

    notifier->addToList(brchannel);
    notifier->addToList(phchannel);
    notifier->addToList(gfpchannel);
    notifier->addToList(rfpchannel);
    notifier->addToList(dapichannel);
    connect(group,QOverload<int>::of(&QButtonGroup::buttonClicked),this,QOverload<int>::of(&ChannelBox::toggleChannel));
    connect(notifier,&ChannelNotifier::channelChanged,this,&ChannelBox::channelChanged);
    connect(notifier,&ChannelNotifier::channelClosed,this,&ChannelBox::channelClosed);
#else
    connect(brchannel,&PushButton::clicked,this,&ChannelBoxx::clickBr);
    connect(phchannel,&PushButton::clicked,this,&ChannelBoxx::clickPh);
    connect(rfpchannel,&PushButton::clicked,this,&ChannelBoxx::clickRfp);
    connect(gfpchannel,&PushButton::clicked,this,&ChannelBoxx::clickGfp);
    connect(dapichannel,&PushButton::clicked,this,&ChannelBoxx::clickDapi);
#endif
}

#ifdef use_channelnotifier
void ChannelBox::toggleChannel(int option)
{ // 使用通知者模式减少大量的重复代码,把clickxxx函数的代码写在notify函数内
    switch (option)
    {
        case 0:
            notifier->notify(brchannel);
            break;
        case 1:
            notifier->notify(phchannel);
            break;
        case 2:
            notifier->notify(gfpchannel);
            break;
        case 3:
            notifier->notify(rfpchannel);
            break;
        case 4:
            notifier->notify(dapichannel);
            break;
    }
}
#else

void ChannelBoxx::clickBr()
{
    !brchannel->isChecked()?brchannel->setBackGroundColor(Qt::yellow):brchannel->resetBackGroundColor();
    phchannel->resetBackGroundColor();
    rfpchannel->resetBackGroundColor();
    gfpchannel->resetBackGroundColor();
    dapichannel->resetBackGroundColor();
    brchannel->isChecked()? emit channelChanged(0): emit channelChanged(-1);
    if (!brchannel->isChecked()) {// 原来是true,要取消高亮和关灯,高亮已经变成了true,关灯要用取反状态
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
    brchannel->resetBackGroundColor();
    !phchannel->isChecked()?phchannel->setBackGroundColor(Qt::yellow):phchannel->resetBackGroundColor();
    rfpchannel->resetBackGroundColor();
    gfpchannel->resetBackGroundColor();
    dapichannel->resetBackGroundColor();
    phchannel->isChecked()? emit channelChanged(1): emit channelChanged(-1);
    if (!phchannel->isChecked()) {
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
    brchannel->resetBackGroundColor();
    phchannel->resetBackGroundColor();
    !gfpchannel->isChecked()?gfpchannel->setBackGroundColor(Qt::yellow):gfpchannel->resetBackGroundColor();
    rfpchannel->resetBackGroundColor();
    dapichannel->resetBackGroundColor();
    gfpchannel->isChecked()? emit channelChanged(2): emit channelChanged(-1);
    if (!gfpchannel->isChecked()) {
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
    brchannel->resetBackGroundColor();
    phchannel->resetBackGroundColor();
    gfpchannel->resetBackGroundColor();
    !rfpchannel->isChecked()?rfpchannel->setBackGroundColor(Qt::yellow):rfpchannel->resetBackGroundColor();
    dapichannel->resetBackGroundColor();
    rfpchannel->isChecked()? emit channelChanged(3): emit channelChanged(-1);
    if (!rfpchannel->isChecked()) {
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
    brchannel->resetBackGroundColor();
    phchannel->resetBackGroundColor();
    rfpchannel->resetBackGroundColor();
    gfpchannel->resetBackGroundColor();
    !dapichannel->isChecked()?dapichannel->setBackGroundColor(Qt::yellow):dapichannel->resetBackGroundColor();
    dapichannel->isChecked()? emit channelChanged(4): emit channelChanged(-1);

    if (!dapichannel->isChecked()) {
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

void ChannelBox::takePhoto(const QImage &img, const QString &channel)
{
    LOG<<"current channel is "<<channel;
    canvas->setImage(ChannelFields.indexOf(channel),img);
}

void ChannelBox::disableChannel(const QString &obj)
{ // obj = br10x, ph4x是兼容的老式字符串,物镜变了
    //LOG<<"objective = "<<obj;
    if (obj.contains(ObjectiveBR) || obj.contains(NAField)) { // 如果是普通物镜或者带NA的物镜那么相差通道不可选择
        phchannel->setEnabled(false);
        phchannel->resetBackGroundColor();
        brchannel->setEnabled(true);
        // BR物镜-荧光通道使能要改回来
        gfpchannel->setEnabled(true);
        rfpchannel->setEnabled(true);
        dapichannel->setEnabled(true);
    } else if (obj.contains(ObjectivePH)) { // 如果是相差物镜只能使用相差通道
        phchannel->setEnabled(true);
        brchannel->setEnabled(false);
        brchannel->resetBackGroundColor();
        // PH物镜-新增荧光通道也不能选
        gfpchannel->setEnabled(false);
        rfpchannel->setEnabled(false);
        dapichannel->setEnabled(false);
    } else {
        // 可能是无镜头 none 对通道的勾选没有影响
    }
}

void ChannelBox::openDefaultChannel()
{ // 用于预览时电机过来就应该开灯
#ifdef use_channelnotifier
    if (phchannel->isEnabled()) { // ph使能说明不能使用明场镜头
        toggleChannel(1); 
    } else if (brchannel->isEnabled()) {
        toggleChannel(0);
    }
#else
    if (phchannel->isEnabled()) { // ph使能说明不能使用明场镜头
        phchannel->click();
    } else if (brchannel->isEnabled()) {
        brchannel->click();
    }
#endif
}

ChannelInfo ChannelBox::channelInfo() const
{
    ChannelInfo m;

    if (brchannel->isChecked())
        m[CurrentChannelField] = BRField; // 转换为idx无需在这里,这里需要实际字符串
    else if (phchannel->isChecked())
        m[CurrentChannelField] = PHField;
    else if (gfpchannel->isChecked())
        m[CurrentChannelField] = GFPField;
    else if (rfpchannel->isChecked())
        m[CurrentChannelField] = RFPField;
    else if (dapichannel->isChecked())
        m[CurrentChannelField] = DAPIField;

    return m;
}

QString ChannelBox::currentChannel() const
{
    return channelInfo()[CurrentChannelField];
}