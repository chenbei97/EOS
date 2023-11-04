#ifndef EOSI_PYTHONTHREAD_H
#define EOSI_PYTHONTHREAD_H

#include "window.h"
#include "Python.h"

class StartPython;
#define StartPythonPointer (&StartPython::instance());

class INTERFACE_IMEXPORT PythonThread : public QThread
{
    Q_OBJECT
public:
    explicit PythonThread(const QString&path,const QString&file,const QString&func,QObject *parent = nullptr);
    ~PythonThread();
    void run() override;
private:
    QString prefixPath;
    QString fileName;
    QString funcName;
};

class INTERFACE_IMEXPORT StartPython: public QObject
{
    Q_OBJECT
public:
    static StartPython& instance();
    void start(const QString &path, const QString &file, const QString &func);
    void quit();
private:
    explicit StartPython(QObject*parent= nullptr);
    ~StartPython();
    PythonThread * thread;
};
#endif //EOSI_PYTHONTHREAD_H
