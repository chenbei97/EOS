
#ifndef EOS_CHANNELBOXX_H
#define EOS_CHANNELBOXX_H

#include "window.h"

class INTERFACE_IMEXPORT ChannelBoxx: public GroupBox
{
    Q_OBJECT
public:
    explicit ChannelBoxx(QWidget*parent= nullptr);
    ChannelInfo channelInfo() const;
    void disableChannel(const QString& obj);
private:
    PushButton * brbtn;
    PushButton * phbtn;
    PushButton * gfpbtn;
    PushButton * rfpbtn;
    PushButton * dapibtn;
private:
    void clickBr();
    void clickPh();
    void clickGfp();
    void clickRfp();
    void clickDapi();
signals:
    void channelChanged(int option);
};

#endif //EOS_CHANNELBOXX_H
