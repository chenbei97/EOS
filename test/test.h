/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-18 09:08:50
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-18 09:14:07
 * @FilePath: \EOS\test\test.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_TEST_H
#define EOSI_TEST_H

#include "interface.h"
#include "socketpanel.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <toupcam.h>
#include <thread>
using std::async;
using std::promise;
using std::future;
//using namespace cv;
#define LOG (qDebug()<<"["<<QTime::currentTime().toString("h:mm:ss:zzz")<<__FUNCTION__<<"] ")

static void test_camera()
{
    uchar*          m_pData = nullptr;
    int             m_res = 0;
    ToupcamDeviceV2 m_cur;
    HToupcam        m_hcam = nullptr;
    if (m_hcam)
    {
        Toupcam_Close(m_hcam);
        m_hcam = nullptr;
    }
    else {
        ToupcamDeviceV2 arr[TOUPCAM_MAX] = {0};
        unsigned count = Toupcam_EnumV2(arr);
        if (0 == count)
            QMessageBox::warning(nullptr, "Warning", "No camera found.");
        else if (1 == count) {
            m_cur = arr[0];
            m_hcam = Toupcam_Open(m_cur.id);
            Toupcam_get_eSize(m_hcam, (unsigned*)&m_res);//获取视频分辨率的索引,0=(2048,1536),1=(1024,768),2=(680,510)
            unsigned m_imgWidth = m_cur.model->res[m_res].width; // ToupcamResolution存了该相机的分辨率
            unsigned m_imgHeight = m_cur.model->res[m_res].height;
            m_pData = new uchar[TDIBWIDTHBYTES(m_imgWidth * 24) * m_imgHeight];
            Toupcam_put_Option(m_hcam, TOUPCAM_OPTION_BYTEORDER, 0); //Qimage use RGB byte order
            
            Toupcam_put_AutoExpoEnable(m_hcam, 1);
            int bAuto = 0;
            Toupcam_get_AutoExpoEnable(m_hcam, &bAuto);

            unsigned exposuremax = 0, exposuremin = 0, exposuredef = 0;
            Toupcam_get_ExpTimeRange(m_hcam, &exposuremin, &exposuremax, &exposuredef);

            unsigned short gainmax = 0, gainmin = 0, gaindef = 0;
            Toupcam_get_ExpoAGainRange(m_hcam, &gainmin, &gainmax, &gaindef);

            unsigned time = 0;
            unsigned short gain = 0;
            Toupcam_get_ExpoTime(m_hcam, &time);
            Toupcam_get_ExpoAGain(m_hcam, &gain);
        }
    }
}

static void test_opencv()
{
    auto path = CURRENT_PATH+"/images/cell.png";
    cv::Mat src = cv::imread(path.toStdString().c_str(),cv::IMREAD_GRAYSCALE);
    if (!src.empty())
    {
        LOG << "src.depth()=" << src.depth();
        cv::namedWindow("input", cv::WINDOW_FREERATIO);//可调的自由比例
        cv::imshow("input", src);
        cv::waitKey(0);
        cv::destroyAllWindows();
    }
    else
        LOG <<"could not load image...";

}

static void test_async()
{
// promise<int> pval;
// pval.set_value(-1);
// auto func1 = [](promise<int> & p){
//     //p.set_value_at_thread_exit(9);
//     p.set_value(9);
//
// };
// std::thread thread1(func1,std::ref(pval));
// LOG<<"is joinable? "<<thread1.joinable();
// //thread1.join();
// thread1.detach();
// auto future1 = pval.get_future();
// LOG<< "promise val = "<<future1.get();


 auto func2 = []()->int{
     int val = -1;
     LOG<<"before sleep_for val = "<<val;
     std::this_thread::sleep_for(std::chrono::seconds(3));
     val = 9;
     LOG<<"after sleep_for val = "<<val;
     return val;
 };
    std::future<int> future2 = std::async(std::launch::async, func2);
    std::future_status status;
    do {
        status = future2.wait_for(std::chrono::seconds(1));
        if (status == std::future_status::deferred) {
            LOG << "deferred\n";
        } else if (status == std::future_status::timeout) {
            LOG << "timeout\n";
        } else if (status == std::future_status::ready) {
            LOG << "ready!\n";
        }
    } while (status != std::future_status::ready);
    LOG << "result is " << future2.get() << '\n';
}

static void test_tcp()
{
    auto w = new SocketPanel;
    w->resize(1000,600);
    setWindowAlignCenter(w);
    w->show();

}

static void test_assemble_parse()
{ // 测试组装和解析和主线程是否相同,结果是不相同,且可以通过loop阻塞等待异步信号拿到结果
    LOG << "main thread: "<<QThread::currentThread();

    // (1) 组装
    QVariantMap m;
    m["0x0000"] = "0x0000";
    AssemblerPointer->assemble("0x0000",m);
    auto msg = AssemblerPointer->message();
    msg.chop(3);
    LOG<<"msg = "<<msg;

    // (2) 解析
    ParserPointer->parse("0x0000",msg);
    LOG<<"frame = "<<ParserFrame<<" res = "<<ParserResult;
}
#endif //EOSI_TEST_H
