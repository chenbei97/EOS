/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-19 15:49:13
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-19 15:52:39
 * @FilePath: \EOS\test\socketpython.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "socketpython.h"

void SocketPython::run()
{
//#ifdef use_python
//    Py_Initialize();
//    if (!Py_IsInitialized()){LOG << "inital python failed!";
//    }else LOG << "inital python successful!";
//    PyRun_SimpleString("import os,sys");
//    PyRun_SimpleString("print(os.getcwd())");
//    PyRun_SimpleString("sys.path.append('../test')");
//
//    PyObject* pModule = PyImport_ImportModule("test_socket");
//
//    if (!pModule){ LOG<<"cant open python file!";
//    } else LOG<<"open python file successful!";
//
//    PyObject* pFunc= PyObject_GetAttrString(pModule,"test_server"); // 调用函数
//
//    if(!pFunc){LOG<<"load tcp load func failed";
//    } else LOG<<"load tcp load func successful";
//
//    PyObject_CallFunction(pFunc,Q_NULLPTR);
//#endif
}

SocketPython::SocketPython(QObject*parent):QThread (parent)
{

}

SocketPython :: ~SocketPython()
{
//#ifdef use_python
//    Py_Finalize();
//#endif
}

