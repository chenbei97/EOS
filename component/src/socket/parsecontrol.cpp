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
    connect(parser,&Parse::parseResult,this,&ParserControl::async);

    connect(this,&ParserControl::parse,this,[&]{loop.exec();}); // 开始解析就进入时间循环等待同步
    connect(parser,&Parse::parseResult,&loop,&EventLoop::quit);

    parsethread.start();
}

void ParserControl::async(QCString f,QCVariant d)
{/*
 * 异步信号过来想要直接拿到结果必须使用同步机制
 * 外部的parse信号进来后就启用同步loop.exec
 * 直到parseResult出现后loop.quit,就可以拦截信号实现快照结果,方便外部直接调用result()函数
 * 快照结果不等于拦截信号,异步信号依然可以发出去为外部使用,也就是外部可以使用同步也可以使用异步机制,2种方法兼容
 * */
    fram=f;res=d;
    //LOG<<"frame = "<<fram<<" d = "<<res; // 从结果打印是正确的
    parseresult[fram]=d;
    emit parseResult(f,d); // 外部使用这个信号纯异步写法也可以,绑定好这个信号就行
}

QVariant ParserControl::result()
{
    auto r = res;
    res = QVariant();//取走后要清除上次的内容防止下次被使用
    //LOG<<r;
    return r;
}

QString ParserControl::frame()
{
    auto f = fram;
    fram.clear();
    return f;
}

QVariantMap ParserControl::response()
{
    auto d = parseresult;
    d[fram].clear();
    fram.clear();
    res = QVariant();
    return d;
}

ParserControl::~ParserControl()
{
    parsethread.quit();
    parsethread.wait();
}