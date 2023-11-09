
#include "startpythonx.h"

StartPythonx& StartPythonx::instance()
{
    static StartPythonx instance;
    return instance;
}

StartPythonx::StartPythonx(QObject *parent) : QObject(parent)
{
    SocketInit;
    Py_Initialize();
    if (!Py_IsInitialized()){LOG << "inital python failed!";
    }else LOG << "inital python successful!";
//    Py_SetPythonHome((wchar_t *)"python310");
//    Py_SetPath((wchar_t *)"python310");
//    LOG<<"python home: "<<Py_GetPythonHome();
//    LOG<<"python path: "<<Py_GetPath();
    PyRun_SimpleString("import os,sys");
    PyRun_SimpleString("print(os.getcwd())");
}

void StartPythonx::start(const QString &path, const QString &file, const QString &func)
{
    QString appendPath = QString("sys.path.append('%1')").arg(path);
    PyRun_SimpleString(appendPath.toStdString().c_str());// "sys.path.append('bin')"
    PyRun_SimpleString("sys.path.append('./python310')");

    PyObject* pModule = PyImport_ImportModule(file.toStdString().c_str()); // "test_socket"

    if (!pModule){ LOG<<"cant open python file!";
    } else LOG<<"open python file successful!";

    PyObject* pFunc= PyObject_GetAttrString(pModule,func.toStdString().c_str()); // 调用函数 "test_server"

    if(!pFunc){LOG<<"load tcp load func failed";
    } else LOG<<"load tcp load func successful";

    PyObject_CallFunction(pFunc,Q_NULLPTR);

    SocketPointer->exec(TcpFramePool.frame0x0002,assemble0x0002(QVariantMap()));
    if (ParserResult.toBool()) LOG<<"socket is connect successful!";
    else LOG<<"socket is connect failed!";
    SocketPointer->exec(TcpFramePool.frame0x0003,assemble0x0003(QVariantMap()));
    LOG<<"activate code is "<<ParserResult.toString();
}

void StartPythonx::quit()
{
    Py_Finalize();
}

StartPythonx::~StartPythonx() noexcept
{
    Py_Finalize();
}
