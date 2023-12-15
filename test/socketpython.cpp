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
    PyRun_SimpleString("sys.path.append('../test')"); // 必须引入

    PyObject* pModule = PyImport_ImportModule("test_socket");

    if (!pModule){ LOG<<"cant open python file!";
    } else LOG<<"open python file successful!";

    PyObject* pFunc= PyObject_GetAttrString(pModule,"test_server"); // 调用函数

    if(!pFunc){LOG<<"load tcp load func failed";
    } else LOG<<"load tcp load func successful";

    PyObject_CallFunction(pFunc,Q_NULLPTR);
}

SocketPython::SocketPython(QObject*parent):QThread (parent)
{

}

SocketPython :: ~SocketPython()
{
    Py_Finalize();
}

