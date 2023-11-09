
#ifndef EOS_MAIN_CORE_PY_STARTPYTHONX_H
#define EOS_MAIN_CORE_PY_STARTPYTHONX_H

#include "window.h"
#include "Python.h"

class INTERFACE_IMEXPORT StartPythonx: public QObject
{
Q_OBJECT
public:
    static StartPythonx& instance();
    void start(const QString &path, const QString &file, const QString &func);
    void quit();
private:
    explicit StartPythonx(QObject* parent= nullptr);
    ~StartPythonx();
};
#define StartPythonxPointer (&StartPythonx::instance())

#endif //EOS_MAIN_CORE_PY_STARTPYTHONX_H
