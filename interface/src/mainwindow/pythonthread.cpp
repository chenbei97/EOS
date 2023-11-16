/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-18 10:42:11
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-18 10:42:59
 * @FilePath: \EOS\interface\include\pythonthread.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "pythonthread.h"

void PythonThread::run()
{
//#ifdef use_python
//    if (!flag) {
//        quit();
//        wait();
//        Py_Finalize();
//    }
//    Py_Initialize();
//    if (!Py_IsInitialized()){LOG << "inital python failed!";
//    }else LOG << "inital python successful!";
//    QString python_path = (CURRENT_PATH+"/python310/");
//    wchar_t path[128] = {0};
//    charTowchar(python_path,path,128);
//    Py_SetPythonHome(path);
//    Py_SetPath(path);
//    char buf1[128] = {0};
//    wcharTochar(Py_GetPythonHome(),buf1,128);
//    char buf2[128] = {0};
//    wcharTochar(Py_GetPath(),buf2,128);
//    LOG<<"python home: "<<buf1;
//    LOG<<"python path: "<<buf2;
//    PyRun_SimpleString("import os,sys");
//    PyRun_SimpleString("print(os.getcwd())");
//    QString appendPath = QString("sys.path.append('%1')").arg(prefixPath);
//    PyRun_SimpleString(appendPath.toStdString().c_str());// "sys.path.append('bin')"
//    PyRun_SimpleString("sys.path.append('./python310')");
//
//    PyObject* pModule = PyImport_ImportModule(fileName.toStdString().c_str()); // "test_socket"
//
//    if (!pModule){ LOG<<"cant open python file!";
//    } else LOG<<"open python file successful!";
//
//    PyObject* pFunc= PyObject_GetAttrString(pModule,funcName.toStdString().c_str()); // 调用函数 "test_server"
//
//    if(!pFunc){LOG<<"load tcp load func failed";
//    } else LOG<<"load tcp load func successful";
//
//    PyObject_CallFunction(pFunc,Q_NULLPTR);
//#endif
}

PythonThread::PythonThread(const QString &path, const QString &file, const QString &func, QObject *parent)
: prefixPath(path),fileName(file),funcName(func),flag(true),QThread(parent)
{

}

void PythonThread::setFlag(bool enable)
{
    flag = enable;
}


PythonThread::~ PythonThread()
{
//#ifdef use_python
//    Py_Finalize();
//#endif
}