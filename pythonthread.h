#ifndef EOSI_PYTHONTHREAD_H
#define EOSI_PYTHONTHREAD_H

#include "public.h"
#include "Python.h"

class PythonThread : public QThread
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

class StartPython: public QObject
{
    Q_OBJECT
public:
    static StartPython& instance();
    void start(const QString &path, const QString &file, const QString &func);
private:
    explicit StartPython(QObject*parent= nullptr);
    ~StartPython();
    PythonThread * thread;
};
#define  StartPythonPointer (&StartPython::instance())
#endif //EOSI_PYTHONTHREAD_H
