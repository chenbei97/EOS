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
#include <thread>
#include <iostream>
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb/stb_image.h"
//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include "stb/stb_image_write.h"
using std::thread;
using std::async;
using std::promise;
using std::future;

static QImage test_splitImage(Qt::GlobalColor color)
{
    QImage img(CURRENT_PATH+"/images/cell.png");

    auto mat = qimageToMat(img);
    std::vector<cv::Mat> channels;
    cv::split(mat,channels); // b,g,r
    std::vector<cv::Mat> new_channels;
    cv::Mat res;
    auto image = QImage();
    auto lab = new Label;
    lab->setScaledContents(true);
    lab->setAttribute(Qt::WA_DeleteOnClose);
    lab->resize(DefaultWindowSize);
    if (color == Qt::red) {
        new_channels = {
                        cv::Mat::zeros(mat.size(),CV_8UC1),
                        cv::Mat::zeros(mat.size(),CV_8UC1),
                        channels[2]
                        };
        cv::merge(new_channels,res);
        image = matToqimage(res);
        lab->setWindowTitle("red image");
    } else if (color == Qt::green) {
        new_channels = {cv::Mat::zeros(mat.size(),CV_8UC1),
                        channels[1],
                        cv::Mat::zeros(mat.size(),CV_8UC1)};
        cv::merge(new_channels,res);
        image = matToqimage(res);
        lab->setWindowTitle("green image");
    } else if (color == Qt::blue){
        new_channels = {
                        channels[0],
                        cv::Mat::zeros(mat.size(),CV_8UC1),
                        cv::Mat::zeros(mat.size(),CV_8UC1),
        };
        cv::merge(new_channels,res);
        image = matToqimage(res);
        lab->setWindowTitle("blue image");
    }
//    LOG<<img;
//    LOG<<image;
//    LOG<<matToqimage(channels[2]); // 32,888,null
    lab->setPixmap(QPixmap::fromImage(image));
    lab->show();
    return image;
}

//static QImage test_stb(const QString& path)
//{
//    int width, height, channels;
//    auto p = path.toLocal8Bit();
//    LOG<<p<<path;
//    unsigned char* data = stbi_load(p, &width, &height, &channels, STBI_rgb_alpha);
//    if (!data) {
//        std::cout << "无法读取灰度图像" << std::endl;
//        return QImage();
//    }
//
//// 创建彩色图像矩阵
//    LOG<<width<<height<<channels;
//    unsigned char* colorImage = new unsigned char[width * height * channels];
//
//// 将灰度图像转换为绿色彩色图像
//    for (int i = 0; i < width * height; i++) {
//        colorImage[i * 3] = data[i]; // 绿色通道
//        colorImage[i * 3 + 1] = 0; // 红色通道设置为0
//        colorImage[i * 3 + 2] = 0; // 蓝色通道设置为0
//    }
//
//    // 释放stb_image加载的图像数据
//
//    stbi_image_free(data);
//    auto pix = QImage(colorImage, width, height, QImage::Format_RGB888);
//    // 将彩色图像保存为文件
//    //const chaoutputPath = "color_image.jpg";
//    //    //channels = 3; // RGB通道数
//    //    //int rowAlign = 4; // 每行像素的对齐字节数r*
//    //stbi_write_png(outputPath, width, height, channels, colorImage, width * channels);
//
//// 释放彩色图像数据内存
//    delete[] colorImage;
//    return pix;
//}

static void test_sharepointer()
{
    // 1. 原始图像的尺寸是QSize(1038, 576)=>597888
    QImage img(CURRENT_PATH+"/images/cell.png");
    LOG<<"img size = "<<img.size();// QSize(1038, 576)

    // 2. 文件读取的size是617719,和wxh的结果并不相等
    QFile file(CURRENT_PATH+"/images/cell.png");
    file.open(QIODevice::ReadOnly);
    int nSize = file.size();
    auto buff = new char[nSize + 1];
    QDataStream in(&file);
    auto buffsize = in.readRawData(buff, nSize);
    LOG<<"buffsize = "<<buffsize<<" nsize = "<<nSize; // buffsize =  617719  nsize =  617719

    // 3. 从buff读取图像的二进制数据生成图像,尺寸不变为(1038, 576),RGB-32bit
    QImage image;
    image.loadFromData(reinterpret_cast<uchar*>(buff), buffsize);
    auto imgsize = image.size();
    Q_ASSERT(imgsize == img.size());
    LOG<<"img size = "<<image.size()<<" "<<image.depth()<<image.format(); // img size =  QSize(1038, 576)   32 QImage::Format_RGB32

    // 4. 智能指针分配buffsize这么大的内存,_msize可以计算指针指向的分配内存大小,分配的必须大于buffsize
    QSharedPointer<uchar> imgdata = QSharedPointer<uchar>(new uchar[buffsize]); // imgsize.width() * 4 * img.height()
    //QSharedPointer<uchar> imgdata = QSharedPointer<uchar>(new uchar[imgsize.width()  * img.height()]); // 1038x576=597888<617719,内存分配不够
    memcpy(imgdata.get(),buff,buffsize);
    Q_ASSERT(_msize(imgdata.get()) == buffsize);
    //LOG<<imgsize.width() * 4 * img.height()<< _msize(imgdata.get()); // 2391552 2391552

    // 5. 从智能指针指向的二进制数据重新读取并保存
    auto bimg = QImage::fromData(imgdata.get(), _msize(imgdata.get()));
    LOG<<"bimg size = "<<bimg.size()<<" "<<bimg.depth()<<bimg.format(); // bimg size =  QSize(1038, 576)   32 QImage::Format_RGB32
    bimg.save("123.jpg");
}

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
//#ifdef use_opencv2
//    auto path = CURRENT_PATH+"/images/cell.png";
//    cv::Mat src = cv::imread(path.toStdString().c_str());
//    if (!src.empty())
//    {
//        LOG<<"images' size = "<<src.rows<<src.cols; // 576,1038
//        LOG<<"images' channels = "<<src.channels(); // 3
//        LOG<<"images' dims = "<<src.dims;// 2
//        LOG<<"images' depth = "<<src.depth(); // 0
//        LOG<<"images' type = "<<src.type();// 16
//        LOG<<"images is 8UC3? = "<<(src.type()==CV_8UC3); // true
//        cv::namedWindow("input", cv::WINDOW_FREERATIO);//可调的自由比例
//        cv::imshow("input", src);
//        cv::waitKey(0);
//        cv::destroyAllWindows();
//    }
//    else
//        LOG <<"could not load image...";
//#endif
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
