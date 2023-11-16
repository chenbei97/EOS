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
//    SocketInit;
//    process = new QProcess;
//    process->setWorkingDirectory(CURRENT_PATH);
//    LOG<<"python process's workdir is "<<process->workingDirectory();
//
//    QObject::connect(qApp, &QCoreApplication::aboutToQuit, [this]() {
//        process->close();
//        process->waitForFinished();
//        LOG<<"python process is kill? "<<!process->isOpen();
//#ifdef use_python
//        Py_Finalize();
//#endif
//    });
//#ifdef use_python
//    Py_Initialize();
//    //QString pythonHome = "C:\\Users\\Lenovo\\AppData\\Local\\Programs\\Python\\Python310";
//    QString pythonHome = (CURRENT_PATH+"/python310");
//    wchar_t path[128] = {0};
//    charTowchar(pythonHome,path,128);
//    Py_SetPythonHome(path);
//    Py_SetPath(L"C:\\Users\\Lenovo\\Desktop\\EOS\\bin\\python310;" // 不需要显式指定,自动获取目录
//               "C:\\Users\\Lenovo\\Desktop\\EOS\\bin\\python310\\Lib;"
//               "C:\\Users\\\\Lenovo\\Desktop\\EOS\\bin\\python310\\DLLs;"
//               "C:\\Users\\Lenovo\\Desktop\\EOS\\\\bin\\python310\\libs;"
//               "C:\\Users\\Lenovo\\Desktop\\\\EOS\\bin\\python310\\include;");
//    if (!Py_IsInitialized()){LOG << "inital python failed!";
//    }else LOG << "inital python successful!";
//
//    char buf1[128] = {0};
//    wcharTochar(Py_GetPythonHome(),buf1,128);
//    char buf2[256] = {0};
//    wcharTochar(Py_GetPath(),buf2,256);
//    LOG<<"python home: "<<buf1;
//    LOG<<"python path: "<<buf2;
//    //PyRun_SimpleString("import os,sys");
//    //PyRun_SimpleString("sys.path.append('./python310')"); // 也可以不显示添加,因为目录已经指定过了
//    //PyRun_SimpleString("print('os.getcwd(): ',os.getcwd())");
//    //PyRun_SimpleString("print('sys.executable: ',sys.executable)");
//#endif
}

void PythonProcess::start(const QString& path)
{
//    auto env = QProcessEnviro nment::systemEnvironment();
//    env.insert("PYTHON310", CURRENT_PATH+"/python310");
//    process->setProcessEnvironment(env);
//    foreach(auto env,QProcess::systemEnvironment()) {
//        LOG<<"["<<env<<"]";
//    }

//#ifdef use_python
//    LOG<<"python.exe path = "<<CURRENT_PATH+"/python310/python.exe"<<" exec path = "<<path;
//    process->start(CURRENT_PATH+"/python310/python.exe",QStringList()<<path);
//    process->waitForStarted();
//#endif

    // 2. 不使用进程也不使用线程
//    PyObject* pModule = PyImport_ImportModule("test_socket"); // "test_socket"
//    PyObject* pFunc= PyObject_GetAttrString(pModule,"test_server"); // 调用函数 "test_server"
//    //QTimer::singleShot(800,[=]{SocketInit.connectToHost();});
//    //QTimer::singleShot(100,this,[=]{PyObject_CallFunction(pFunc,Q_NULLPTR);});
//    PyObject_CallFunction(pFunc,Q_NULLPTR); // 这种方式python程序会阻塞客户端

//    SocketPointer->exec(TcpFramePool.frame0x0002,assemble0x0002(QVariantMap()));
//    if (ParserResult.toBool()) LOG<<"socket is connect successful!";
//    else LOG<<"socket is connect failed!";
//    SocketPointer->exec(TcpFramePool.frame0x0003,assemble0x0003(QVariantMap()));
//    LOG<<"activate code is "<<ParserResult.toString();
}
