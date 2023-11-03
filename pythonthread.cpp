#include "pythonthread.h"

void PythonThread::run()
{
    Py_Initialize();
    if (!Py_IsInitialized()){LOG << "inital python failed!";
    }else LOG << "inital python successful!";
    PyRun_SimpleString("import os,sys");
    PyRun_SimpleString("print(os.getcwd())");
    QString appendPath = QString("sys.path.append('%1')").arg(prefixPath);
    PyRun_SimpleString(appendPath.toStdString().c_str());// "sys.path.append('bin')"

    PyObject* pModule = PyImport_ImportModule(fileName.toStdString().c_str()); // "test_socket"

    if (!pModule){ LOG<<"cant open python file!";
    } else LOG<<"open python file successful!";

    PyObject* pFunc= PyObject_GetAttrString(pModule,funcName.toStdString().c_str()); // 调用函数 "test_server"

    if(!pFunc){LOG<<"load tcp load func failed";
    } else LOG<<"load tcp load func successful";

    PyObject_CallFunction(pFunc,Q_NULLPTR);
}

PythonThread::PythonThread(const QString &path, const QString &file, const QString &func, QObject *parent)
: prefixPath(path),fileName(file),funcName(func), QThread(parent)
{

}


PythonThread :: ~ PythonThread()
{
    Py_Finalize();
}

StartPython &StartPython::instance()
{
    static StartPython instance;
    return instance;
}

StartPython::StartPython(QObject *parent) : QObject(parent)
{
    thread = nullptr;
    SocketInit;
}

void StartPython::start(const QString &path, const QString &file, const QString &func)
{
    if (thread == nullptr)
        thread = new PythonThread(path,file,func);
    if (!thread->isRunning())
        thread->start();
    SocketPointer->exec(TcpFramePool.frame0x0002,assemble0x0002(QVariantMap()));
    if (ParserResult.toBool()) LOG<<"socket is connect successful!";
    else LOG<<"socket is connect failed!";
    SocketPointer->exec(TcpFramePool.frame0x0003,assemble0x0003(QVariantMap()));
    LOG<<"activate code is "<<ParserResult.toString();
}

StartPython::~StartPython() noexcept
{
    thread->quit();
    thread->wait();
}