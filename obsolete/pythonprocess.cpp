/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-09 15:04:36
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-09 16:36:25
 * @FilePath: \EOS\interface\src\mainwindow\pythonprocess.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "pythonprocess.h"

PythonProcess::PythonProcess(QObject*parent): QObject(parent)
{
    //SocketInit; // 进程的话不要在这就连接否则会阻塞,先启动进程再连
    process = new QProcess;
    process->setWorkingDirectory(CURRENT_PATH);
    LOG<<"python process's workdir is "<<process->workingDirectory();

    QObject::connect(qApp, &QCoreApplication::aboutToQuit, [this]() {
        process->close();
        process->waitForFinished();
        LOG<<"python process is kill? "<<!process->isOpen();
        Py_Finalize();
    });
    Py_Initialize();
    //QString pythonHome = "C:\\Users\\Lenovo\\AppData\\Local\\Programs\\Python\\Python310";
    QString pythonHome = (CURRENT_PATH+"\\python310");
    wchar_t home[128] = {0};
    charTowchar(pythonHome,home,128);
    Py_SetPythonHome(home);
    char buf0[512] = {0};
    wcharTochar(Py_GetPath(),buf0,512);
    LOG<<"python path: "<<buf0; // 打印出来的路径不对,但是不影响后续的连接
//    auto pythonPath = CURRENT_PATH+"\\"+"python310;"+
//            CURRENT_PATH+"\\"+"python310;"+
//            CURRENT_PATH+"\\"+"python310\\Lib;"+
//            CURRENT_PATH+"\\"+"python310\\DLLs;"+
//            CURRENT_PATH+"\\"+"python310\\libs;"+
//            CURRENT_PATH+"\\"+"python310\\include;";
//    wchar_t path[512] = {0};
//    charTowchar(pythonPath,path,512);// 似乎不需要显式指定路径也可以,自动获取目录
//    Py_SetPath(path);
    if (!Py_IsInitialized()){LOG << "inital python failed!";
    }else LOG << "inital python successful!";

    char buf1[128] = {0};
    wcharTochar(Py_GetPythonHome(),buf1,128);
    char buf2[512] = {0};
    wcharTochar(Py_GetPath(),buf2,512);
    LOG<<"python home: "<<buf1;
    LOG<<"python path: "<<buf2;
//    PyRun_SimpleString("import os,sys");
//    PyRun_SimpleString("sys.path.append('./python310')"); // 也可以不显示添加,因为目录已经指定过了
//    PyRun_SimpleString("print('os.getcwd(): ',os.getcwd())");
//    PyRun_SimpleString("print('sys.executable: ',sys.executable)");
}

void PythonProcess::start(const QString& path)
{
    // 这段代码注释掉,好像不能给电脑插环境变量
//    auto env = QProcessEnvironment::systemEnvironment();
//    env.insert("PYTHON310", CURRENT_PATH+"/python310");
//    process->setProcessEnvironment(env);
//    foreach(auto env,QProcess::systemEnvironment()) {
//        LOG<<"["<<env<<"]";
//    }

    // 1. 使用进程
    LOG<<"python.exe path = "<<CURRENT_PATH+"\\python310\\python.exe"<<" exec path = "<<path; // 必须指定使用python310内的解释器
    process->start(CURRENT_PATH+"\\python310\\python.exe",QStringList()<<path);
//    process->start("C:\\Users\\22987\\AppData\\Local\\Programs\\Python\\Python310\\python.exe",
//                   QStringList()<<path);
    process->waitForStarted();
    SocketInit; // 进程启动后再连接

    // 2. 不使用进程也不使用线程 直接调函数
//    PyObject* pModule = PyImport_ImportModule("test_socket"); // "test_socket"
//    PyObject* pFunc= PyObject_GetAttrString(pModule,"test_server"); // 调用函数 "test_server"
//    //QTimer::singleShot(800,[=]{SocketInit.connectToHost();});
//    //QTimer::singleShot(100,this,[=]{PyObject_CallFunction(pFunc,Q_NULLPTR);});
//    PyObject_CallFunction(pFunc,Q_NULLPTR); // 这种方式python程序会阻塞客户端,因为python先启动然后阻塞等待

    SocketPointer->exec(TcpFramePool.askConnectedStateEvent,assembleAskConnectedStateEvent(QVariantMap()),true);
    if (ParserResult.toBool()) LOG<<"socket is connect successful!";
    else LOG<<"socket is connect failed!";
    SocketPointer->exec(TcpFramePool.askActivateCodeEvent,assembleAskActivateCodeEvent(QVariantMap()),true);
    LOG<<"activate code is "<<ParserResult.toString();
}
