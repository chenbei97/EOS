#include "channelboxx.h"

ChannelBoxx::ChannelBoxx(QWidget*parent): GroupBox(parent)
{
    brbtn = new PushButton(BRField);
    phbtn = new PushButton(PHField);
    gfpbtn = new PushButton(GFPField);
    rfpbtn = new PushButton(RFPField);
    dapibtn = new PushButton(DAPIField);

    // objectivesetting,默认是br4x,故objectivebox默认是br4x,故默认brbox是启用,phbox禁用
    brbtn->setEnabled(true);
    phbtn->setEnabled(false);

    auto lay = new QHBoxLayout;

    lay->addWidget(brbtn);
    lay->addWidget(phbtn);
    lay->addWidget(gfpbtn);
    lay->addWidget(rfpbtn);
    lay->addWidget(dapibtn);

    setLayout(lay);
    setTitle(tr("通道"));

//    QButtonGroup * group = new QButtonGroup;
//    group->addButton(brbtn,0);
//    group->addButton(phbtn,1);
//    group->addButton(gfpbtn,2);
//    group->addButton(rfpbtn,3);
//    group->addButton(dapibtn,4);
//    brbtn->setChecked(true);

    connect(brbtn,&PushButton::clicked,this,&ChannelBoxx::clickBr);
    connect(phbtn,&PushButton::clicked,this,&ChannelBoxx::clickPh);
    connect(rfpbtn,&PushButton::clicked,this,&ChannelBoxx::clickRfp);
    connect(gfpbtn,&PushButton::clicked,this,&ChannelBoxx::clickGfp);
    connect(dapibtn,&PushButton::clicked,this,&ChannelBoxx::clickDapi);
    //connect(group,QOverload<int>::of(&QButtonGroup::buttonClicked),this,&ChannelBoxx::channelChanged);
}

void ChannelBoxx::clickBr()
{
    !brbtn->isChecked()?brbtn->setBackGroundColor(Qt::yellow):brbtn->resetBackGroundColor();
    phbtn->resetBackGroundColor();
    rfpbtn->resetBackGroundColor();
    gfpbtn->resetBackGroundColor();
    dapibtn->resetBackGroundColor();
    brbtn->isChecked()? emit channelChanged(0): emit channelChanged(-1);
    if (!brbtn->isChecked()) {// 原来是true,要取消高亮和关灯,高亮已经变成了true,关灯要用取反状态
        QVariantMap m;
        m[CurrentChannelField] = 0;
        m[TurnOffLight] = 1;
        m[BrightField] = -1;
        SocketPointer->exec(TcpFramePool.frame0x0006,assemble0x0006(m));
        if (ParserResult.toBool()) {
            LOG<<"关闭BR";
        }
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
        QVariantMap m;
        m[CurrentChannelField] = 1;
        m[TurnOffLight] = 1;
        m[BrightField] = -1;
        SocketPointer->exec(TcpFramePool.frame0x0006,assemble0x0006(m));
        if (ParserResult.toBool()) {
            LOG<<"关闭PH";
        }
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
        QVariantMap m;
        m[CurrentChannelField] = 2;
        m[TurnOffLight] = 1;
        m[BrightField] = -1;
        SocketPointer->exec(TcpFramePool.frame0x0006,assemble0x0006(m));
        if (ParserResult.toBool()) {
            LOG<<"关闭GFP";
        }
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
        QVariantMap m;
        m[CurrentChannelField] = 3;
        m[TurnOffLight] = 1;
        m[BrightField] = -1;
        SocketPointer->exec(TcpFramePool.frame0x0006,assemble0x0006(m));
        if (ParserResult.toBool()) {
            LOG<<"关闭RFP";
        }
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
        QVariantMap m;
        m[CurrentChannelField] = 4;
        m[TurnOffLight] = 1;
        m[BrightField] = -1;
        SocketPointer->exec(TcpFramePool.frame0x0006,assemble0x0006(m));
        if (ParserResult.toBool()) {
            LOG<<"关闭DAPI";
        }
    }

    //LOG<<channelInfo();
}

void ChannelBoxx::disableChannel(const QString &obj)
{ // obj = br4x,ph4x
    if (obj.contains(ObjectiveBR)) {
        phbtn->setEnabled(false);
        brbtn->setEnabled(true);
    } else if (obj.contains(ObjectivePH)) {
        phbtn->setEnabled(true);
        brbtn->setEnabled(false);
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