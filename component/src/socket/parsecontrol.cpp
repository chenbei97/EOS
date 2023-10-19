/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-18 08:42:29
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-18 08:43:30
 * @FilePath: \EOS\component\src\socket\parsecontrol.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "parsecontrol.h"

ParserControl& ParserControl::instance()
{
    static ParserControl p;
    return p;
}

ParserControl::ParserControl(QObject *parent) : QObject(parent)
{
    parser = new Parse;
    parser->moveToThread(&parsethread);

    // 解析信号传递给解析类,触发信号和同步信号,这样信号回来后这里result()就可以拿到同步后的结果
    connect(this,&ParserControl::parse,parser,&Parse::parse);
    connect(parser,&Parse::parseResult,this,[&](auto f,auto d){
        fram=f;res=d;
        //LOG<<"frame = "<<fram<<" d = "<<res;
        emit parseResult(f,d);
    });

    connect(this,&ParserControl::parse,this,[&]{loop.exec();}); // 开始解析就进入时间循环等待同步
    connect(this,&ParserControl::parseResult,&loop,&EventLoop::quit);

    parsethread.start();
}

QVariant ParserControl::result() const
{
    return res;
}

QString ParserControl::frame() const
{
    return fram;
}

ParserControl::~ParserControl()
{
    parsethread.quit();
    parsethread.wait();
}