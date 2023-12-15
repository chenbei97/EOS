/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-12-15 08:50:11
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-15 09:10:20
 * @FilePath: \EOS\interface\src\mainwindow\pythoncall.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "pythoncall.h"

void PythonCall::start(const QString &path, const QString &file, const QString &func)
{
    QString appendPath = QString("sys.path.append('%1')").arg(path); // 必须要把../test加入路径否则不认识
    PyRun_SimpleString(appendPath.toStdString().c_str());// "sys.path.append('bin')"
    //PyRun_SimpleString("sys.path.append('./python310')");

    PyObject* pModule = PyImport_ImportModule(file.toStdString().c_str()); // "test_socket"

    if (!pModule){ LOG<<"cant open python file!";
    } else LOG<<"open python file successful!";

    PyObject* pFunc= PyObject_GetAttrString(pModule,func.toStdString().c_str()); // 调用函数 "test_server"

    if(!pFunc){LOG<<"load tcp load func failed";
    } else LOG<<"load tcp load func successful";

    PyObject_CallFunction(pFunc,Q_NULLPTR); // 非线程也非进程方式,会阻塞
    SocketInit;
    SocketPointer->exec(TcpFramePool.askConnectedStateEvent,assembleAskConnectedStateEvent(QVariantMap()));
    if (ParserResult.toBool()) LOG<<"socket is connect successful!";
    else LOG<<"socket is connect failed!";
    SocketPointer->exec(TcpFramePool.askActivateCodeEvent,assembleAskActivateCodeEvent(QVariantMap()));
    LOG<<"activate code is "<<ParserResult.toString();
}

PythonCall::~PythonCall() noexcept
{
    Py_Finalize();
}

PythonCall& PythonCall::instance()
{
    static PythonCall instance;
    return instance;
}

PythonCall::PythonCall(QObject *parent)
{
    Py_Initialize();
    if (!Py_IsInitialized()){LOG << "inital python failed!";
    }else LOG << "inital python successful!";
    wchar_t home[128] = {0};
    QString pythonHome = (CURRENT_PATH+"\\python310");
    charTowchar(pythonHome,home,128);
    Py_SetPythonHome(home);
    char buf1[128] = {0};
    wcharTochar(Py_GetPythonHome(),buf1,128);
    char buf2[512] = {0};
    wcharTochar(Py_GetPath(),buf2,512);
    LOG<<"python home: "<<buf1;
    LOG<<"python path: "<<buf2;
    PyRun_SimpleString("import os,sys");
    PyRun_SimpleString("print(os.getcwd())");
}