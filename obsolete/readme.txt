（1）pythonprocess是以进程方式启动Python
注意的2点：
① 先启动进程后再客户端去连接，否则会阻塞
② start进程的参数必须指定python.exe的全路径，也就是bin/python310/python.exe
（2）startpython和pythonthread类,以线程方式启动Python
其中startpython是单例类,内部持有pythonthread指针
注意的2点: 
① 先启动线程后再客户端去连接，否则会阻塞
② 线程无需指定python.exe,是通过调函数的方式启动
但是需要通过PyImport_ImportModule手动导入模型即"test_socket"
需要通过PyObject_CallFunction手动调用函数即"test_server"
③ 线程方式有个好处,python脚本的print信息可以在c++终端可见,因为这个线程是c++启动的
④ 线程的方式开发时没有问题,但是打包到其他电脑运行会提示错误,套接字不能跨线程通信
这也是为何startpython和pythonthread不使用的原因
如果不需要套接字通信仅仅是启动个脚本,线程/进程方式均可,否则只能使用进程方式
将这2个类归入obsolete不参与编译（这2个类是条件可用的）
（3） pythoncall类,非进程也非线程而是直接通过Python.h的API调用脚本
注意的2点: 
① 适用于调用不带阻塞逻辑的脚本,如果阻塞就会一直等待无法执行C++窗口
② 调用带有相对路径的脚本,必须使用PyRun_SimpleString执行路径添加,否则不认识
QString appendPath = QString("sys.path.append('%1')").arg(path); // 必须要把../test加入路径否则不认识
PyRun_SimpleString(appendPath.toStdString().c_str());// "sys.path.append('../test')"
将这个类归入obsolete不参与编译（这个类是条件可用的）
（4）主程序main.cpp这样去使用（1）~（3）的类:
    1) 进程方式
    auto process = new PythonProcess;
    process->start("../test/test_socket.py");
    2) 线程方式
    StartPythonPointer->start("../test","test_socket","test_server");
    3) 非线程也非进程方式(会阻塞)
    PythonCallPointer->start("../test","test_socket","test_server");
（5）socketpython,socketpanel是早期开发用于方便测试TCP通讯使用的,现在不需要了
    使用时可以在main.cpp直接调用socketpanel.h的test_socketPanel()函数
    将这个类归入obsolete不参与编译（这个类是测试用的）
（6）由于涉及到python的所有类都已经并不参与编译,所以不需要引入Python.h和调用Python的库
故在CMakeLists.txt内把关于Python的编译配置工作都注释掉

其它：完全过时且不可用的类，这些类只是留着以后可能参考其中的某些逻辑
减少不用的类的编译，提高编译速度