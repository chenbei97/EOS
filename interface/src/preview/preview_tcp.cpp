/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-31 14:06:48
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-17 16:16:08
 * @FilePath: \EOS\interface\src\preview\preview_tcp.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "preview.h"

void Preview::manualFocus(double val)
{ // 手动调焦,来自focusslider的信号,目前是移动滑动条时就会触发,点击2个细调按钮也会触发
    //LOG<<"focus = "<<val;
    QVariantMap m;
    m[FocusField] = val;
    m[FrameField] = TcpFramePool.manualFocusEvent;
    auto msg = assembleManualFocusEvent(m);
    SocketPointer->exec(TcpFramePool.manualFocusEvent,msg, true);
    if (ParserResult.toBool()) {
        LOG<<"adjust focus to "<<val<<"successful!";
    }
}

void Preview::toggleObjective(int objective,int objective_loc,int isPh)
{ // objective: 物镜倍数代号 objective_loc: 切到的物镜位置代号 isPH:指示是否为PH类型
    // 这个初始化setting时会初始化4个位置的物镜,从而会触发本函数,在mainwindow没构造完成就发了命令,故需要invokeMethod立即处理
    // 其它的函数不需要invokeMethod,本函数特殊
    //LOG<<"objective = "<<objective<<" loc = "<<objective_loc<<" isph = "<<isPh;
    QVariantMap m;
    m[ObjectiveField] = objective;
    m[ObjectiveLocationField] = objective_loc;
    m[IsPHField] = isPh;

    AssemblerPointer->assemble(TcpFramePool.toggleObjectiveEvent,m);
    auto msg = AssemblerPointer->message();

    SocketPointer->exec(TcpFramePool.toggleObjectiveEvent,msg, true);
    QMetaObject::invokeMethod(SocketPointer,"processRequestQueue",Qt::DirectConnection);
    if (ParserResult.toBool()) {
        LOG<<"toggle objective successful! magnification ="<<ObjectiveMagnificationFields[objective]<<"isPH? "<<isPh;
    } else {LOG<<"toggle objective failed!";}
}

void Preview::toggleChannel(int option)
{
    auto toolinfo = previewtool->toolInfo();
    auto current_channel = toolinfo[CurrentChannelField].toString();

    if (current_channel.isEmpty()) {
        LOG<<"toggle channel isvalid";
        Q_ASSERT(option == -1); // 如果没有通道,此时是空字符串,切option=1,那么这个时候就直接关灯即可
        // 不过关灯需要知道关哪个灯,这个工作在channelboxx已经做了,这里什么也不做
        return; // 灯全灭的情况直接返回
    }

    auto current_info = toolinfo[CurrentInfoField].value<CameraInfo>();

    QVariantMap m;
    m[CurrentChannelField] = getIndexFromFields(current_channel);
    m[BrightField] = current_info[BrightField];
    m[TurnOffLight] = 0;

    AssemblerPointer->assemble(TcpFramePool.toggleChannelEvent,m);
    auto msg = AssemblerPointer->message();

    SocketPointer->exec(TcpFramePool.toggleChannelEvent,msg, true);

    if (ParserResult.toBool()) {
        LOG<<"turn on light successful! current channel is "<<m[CurrentChannelField];
    }
}

void Preview::adjustLens(int option)
{ // 0-left,1-rop,2-right,3-bottom

    // 点调节镜头
    QJsonObject object;
    object[FrameField] = TcpFramePool.adjustLensEvent;
    object[DirectionField] = option;
    TcpAssemblerDoc.setObject(object);
    auto msg = AppendSeparateField(TcpAssemblerDoc.toJson());

    //LOG<<msg;
    SocketPointer->exec(TcpFramePool.adjustLensEvent,msg, true);

    if (ParserResult.toBool()) {
        LOG<<"移动镜头方向: "<<option;
    }
}

void Preview::adjustCamera(int exp,int gain,int br)
{
    auto toolinfo = previewtool->toolInfo();

    auto current_channel = toolinfo[CurrentChannelField].toString();

    if (current_channel.isEmpty()) {
        LOG<<"没有通道的灯被打开,不执行滑动条的参数调整!";
        return; // 如果通道无效,没有开灯,调节参数没有意义,不发给下位机
    }
#ifndef notusetoupcamera
    LOG<<"exposureOption = "<<ToupCameraPointer->exposureOption();
    if (!ToupCameraPointer->exposureOption()) {
        ToupCameraPointer->setExposure(exp);
        ToupCameraPointer->setGain(gain);
    }
;
#else
    LOG<<"exposureOption = "<<exposureOption();
    if (!exposureOption()) { // 设置手动曝光,exp option = 0
        setExposure(exp);
        setGain(gain);
    }
#endif
    // 滑动条速度很快,这里组装不再通过Assembler来组装,可能同步会出问题,直接组装
    QJsonObject object;
    object[FrameField] = TcpFramePool.adjustBrightEvent;
    object[FieldAdjustBrightEvent.bright] = br;
    object[FieldAdjustBrightEvent.current_channel] = getIndexFromFields(current_channel);
    TcpAssemblerDoc.setObject(object);
    auto msg = AppendSeparateField(TcpAssemblerDoc.toJson());;

    // 发送消息同步,异步都行,如果用同步这里就可以打印消息,这里所有的函数都可以异步也可以同步,2种方式都没有问题
    // 异步做法对adjustCamera则是用ParsePointer的parseResult信号绑定到下边的onAdjustCamera函数
    // 然后打印消息也是可以的,2种写法机制测试了所有情况都没有问题,尤其现在exec引入请求消息队列更加优化了代码不出问题
    SocketPointer->exec(TcpFramePool.adjustBrightEvent,msg, true);
    if (ParserResult.toBool()) {
        LOG<<"adjust exp gain bright to"<<exp<<gain<<br;
    }
}

// 一个例子,其它的函数例如adjustLens也可以绑一个函数给ParserPointer,内部f==TcpFramePool.adjustLensEvent时去做一些事
// 只不过这里我觉得为了方便打log犯不着额外绑定个函数
void Preview::onAdjustCamera(const QString & f,const QVariant & d)
{ // 这个是异步获取ParsePointer的parseResult,上方不使用同步,连接本函数也是可以的
    static int count = 0;
    if (d.toBool() && f == TcpFramePool.adjustBrightEvent) {
        LOG<<"adjust exp gain bright successful!"<<count;
    }
    count++;
}

#ifndef notusetoupcamera
void Preview::showCapturedImage(const QImage& image)
{
#ifdef uselabelcanvas
    auto pix = QPixmap::fromImage(image).scaled(livecanvas->size(),Qt::KeepAspectRatio,Qt::FastTransformation);
    livecanvas->setPixmap(pix);
    livecanvas->repaint();
#else
    // QVariantMap m;
    // m[ImageField] = image;
    // livecanvas->setData(m);
    livecanvas->setImage(image,10);
#endif
}
#endif

void Preview::takingPhoto()
{
    auto toolinfo = previewtool->toolInfo();

    auto current_info = toolinfo[CurrentInfoField].value<CameraInfo>();
    int exp = current_info[ExposureField].toUInt();
    int ga = current_info[GainField].toUInt();

//  auto save_channels = toolinfo[CaptureChannelField].toStringList();//保存过设置的所有通道
//  QVariantMap m;
// 不从保存过的参数去拿,而是从UI的信息直接去拿
//    if (save_channels.contains(current_channel)) {
//        // 例如当前通道PH,设置过PH的相机参数
//        auto camera_info = toolinfo[current_channel].value<QVariantMap>();
//        Q_ASSERT(camera_info[ChannelField].toString() == current_channel);
//        exp = camera_info[ExposureField].toUInt();
//        ga = camera_info[GainField].toUInt();
//        m[BrightField] = camera_info[BrightField];
//    }

//    QVariantMap m;
//    m[BrightField] = current_info[BrightField];
//    AssemblerPointer->assemble(TcpFramePool.frame0x0004,m);
//    auto msg = AssemblerPointer->message();
//    SocketPointer->exec(TcpFramePool.frame0x0004,msg,true);
//    // 等待回复后调用相机拍照(现在没有拍照事件了,无需发送0x0004命令直接拍就行)

#ifndef notusetoupcamera
        ToupCameraPointer->setExposure(exp);
        ToupCameraPointer->setGain(ga);
        auto pix = ToupCameraPointer->capture();
        auto current_channel = toolinfo[CurrentChannelField].toString();
        previewtool->captureImage(pix,current_channel); // 把当前通道拍到的图像传回去用于后续合成通道,以及显示到缩略图
        LOG<<"已经调整亮度为 "<<current_info[BrightField].toInt()
        <<" 曝光和增益为 "<<ToupCameraPointer->exposure()<<ToupCameraPointer->gain();
#else
        setExposure(exp);
        setGain(ga);
        auto pix = capture();
        auto current_channel = toolinfo[CurrentChannelField].toString();
        previewtool->captureImage(pix,current_channel); // 把当前通道拍到的图像传回去用于后续合成通道
        LOG<<"已经调整亮度为 "<<current_info[BrightField].toInt()
       <<" 曝光和增益为 "<<exposure()<<gain();
#endif

        cameramode->changeMode(CameraMode::PhotoMode);
        photocanvas->setImage(pix);

        // 存图功能,目前默认存到temp下,用户会更改前缀目录,或者需要按分不同通道文件夹归类等
        createPath(TakePhotoTempPath);
        auto path = TakePhotoTempPath
                    +QDateTime::currentDateTime().toString(DefaultImageSaveDateTimeFormat)+JPGSuffix;
        pix.save(path,JPGField,100);

}

void Preview::previewViewByClickView(const QPointF &viewpoint)
{
    auto toolinfo = previewtool->toolInfo();

    // 预览事件需要的参数
    auto objective = getIndexFromFields(toolinfo[ObjectiveField].toString()).toUInt();
    auto brand = toolinfo[BrandField].toUInt();
    auto manufacturer = toolinfo[ManufacturerField].toUInt();
    auto wellsize = toolinfo[WellsizeField].toUInt();
    auto viewsize = ViewCircleMapFields[manufacturer][brand][objective];//点孔触发预览的时候需要传递viewsize
    auto holecoordinate = viewpattern->viewInfo()[HoleCoordinateField].toPoint(); // 这个信息单独点击孔是没有传递的
    //auto current_channel = getIndexFromFields(toolinfo[CurrentChannelField].toString());
    auto current_info = toolinfo[CurrentInfoField].value<CameraInfo>();
    auto bright = current_info[BrightField];

    // 自己需要的相机参数
    int exp = current_info[ExposureField].toUInt();
    int ga = current_info[GainField].toUInt();
    //LOG<<wellsize<<viewpoint<<holecoordinate<<bright<<current_channel;
#ifndef notusetoupcamera
    ToupCameraPointer->setExposure(exp);
    ToupCameraPointer->setGain(ga);
#else
    setExposure(exp);
    setGain(ga);
#endif
    QVariantMap m;
    m[ObjectiveField] = objective;
    m[BrandField] = brand;
    m[ManufacturerField] = manufacturer;
    m[WellsizeField] = wellsize;
    m[HoleViewSizeField] = viewsize;
    m[HoleCoordinateField] = holecoordinate;
    m[ViewCoordinateField] = viewpoint;
    //m[CurrentChannelField] = current_channel;// 不需要再发当前通道了
    m[BrightField] = bright;
    m[IsHoleField] = 0;

    AssemblerPointer->assemble(TcpFramePool.previewEvent,m);
    auto msg = AssemblerPointer->message();
    SocketPointer->exec(TcpFramePool.previewEvent,msg, true);
    if (ParserResult.toBool()) {
        LOG<<QString("move to view point (%1,%2)").arg(convertPrecision(viewpoint.x()))
        .arg(convertPrecision(viewpoint.y()));
    }
}

void Preview::previewViewByClickHole(const QPoint &holepoint)
{
    if (holepoint == QPoint(-1,-1)) return;

    auto toolinfo = previewtool->toolInfo();

    // 预览事件需要的参数
    auto objective = getIndexFromFields(toolinfo[ObjectiveField].toString()).toUInt();
    auto brand = toolinfo[BrandField].toUInt();
    auto manufacturer = toolinfo[ManufacturerField].toUInt();
    auto wellsize = toolinfo[WellsizeField].toUInt();
    auto viewsize = ViewCircleMapFields[manufacturer][brand][objective];//点孔触发预览的时候需要传递viewsize
    //auto holecoordinate = viewpattern->currentViewInfo()[HoleCoordinateField].toPoint(); // 这个信息单独点击孔是没有传递的
    auto holecoordinate = holepoint; // 所以才为什么只能分成previewViewByClickHole和previewViewByClickView 2个函数写了
    //auto current_channel = getIndexFromFields(toolinfo[CurrentChannelField].toString());
    auto current_info = toolinfo[CurrentInfoField].value<CameraInfo>();
    auto bright = current_info[BrightField];

    // 自己需要的相机参数
    int exp = current_info[ExposureField].toUInt();
    int ga = current_info[GainField].toUInt();
#ifndef notusetoupcamera
    ToupCameraPointer->setExposure(exp);
    ToupCameraPointer->setGain(ga);
#else
    setExposure(exp);
    setGain(ga);
#endif
    QVariantMap m;
    m[ObjectiveField] = objective;
    m[BrandField] = brand;
    m[ManufacturerField] = manufacturer;
    m[WellsizeField] = wellsize;
    m[HoleViewSizeField] = viewsize;
    m[HoleCoordinateField] = holecoordinate;
    m[ViewCoordinateField] = QPointF(-1.0,-1.0);//点孔触发视野坐标没有意义
    m[BrightField] = bright;
    m[IsHoleField] = 1;

    AssemblerPointer->assemble(TcpFramePool.previewEvent,m);
    auto msg = AssemblerPointer->message();
    SocketPointer->exec(TcpFramePool.previewEvent,msg, true);
    if (ParserResult.toBool()) {
        LOG<<"move to hole point "<<holepoint;
    }
}

void Preview::exportExperConfig(const QString& path)
{ // 导出实验配置
    previewinfo[PreviewPatternField] = pattern->patternInfo();
    previewinfo[PreviewToolField] = previewtool->toolInfo();
#ifdef usetab
    previewinfo[ExperToolField] = expertool->toolInfo();
#endif
    AssemblerPointer->assemble(TcpFramePool.loadExperEvent,previewinfo);
    auto json = AssemblerPointer->message();

    json.chop(QString(SeparateField).count());//删掉尾缀@@@

    JsonReadWrite m; // 借助工具类写到文件内
    m.writeJson(path,json);
}

void Preview::importExperConfig(const QString& path)
{// 导入实验配置,对于camera_channel和group字段要特殊解析

    ConfigReadWrite m; // 借助工具类读取文件内
    auto json = m.readJson(path);

    m.parseJson(json);
    auto result = m.map();
    previewtool->importExperConfig(result);
#ifdef usetab
    expertool->importExperConfig(result);
#endif

    auto brand = result[BrandField].toUInt();
    auto manufacturer = result[ManufacturerField].toUInt();
    auto objective = result[ObjectiveField].toUInt();
    auto viewsize = ViewCircleMapFields[manufacturer][brand][objective];

    auto info = result[GroupField].value<QVariantMap>();
    foreach(auto group,info.keys()){
        auto groupinfo = info[group].value<QVariantMap>();
        foreach(auto hole,groupinfo.keys()) {
            auto holeinfo = groupinfo[hole].value<QVariantMap>();
            //LOG<<holeinfo;
            auto holepoint = holeinfo[HoleCoordinateField].toPoint();
            auto viewpoints = holeinfo[PointsField].value<QPointVector>();

            // 把holepoint这个孔的信息更改(和wellsize有关,所以需要先更新wellpattern的信息就不会越界了)
            pattern->updateHoleInfo(holepoint,group,viewpoints,viewsize);
            viewpattern->updateViewWindowCache(holepoint,viewpoints,viewsize);
        }

    }

}

void Preview::loadExper()
{
    auto patterninfo = pattern->patternInfo();
    auto previewinfo = previewtool->toolInfo();
    previewinfo[PreviewPatternField] = patterninfo;
    previewinfo[PreviewToolField] = previewinfo;
#ifdef usetab
    auto experinfo = expertool->toolInfo();
    previewinfo[ExperToolField] = experinfo;
#endif


#ifdef usetab
    auto channels = experinfo[FieldLoadExperEvent.channel].toString().split(",",QString::SkipEmptyParts);
#else
    auto channels = previewinfo[FieldLoadExperEvent.channel].toString().split(",",QString::SkipEmptyParts)
#endif
    auto totalViews = pattern->numberOfViews();
    auto totalChannels = channels.count("1"); // 为1的是勾选上的
    auto estimateSpace = calculateExperSpaceMB(totalViews,totalChannels);
    LOG<<"totalViews = "<<totalViews<<"totalChannels = "<<totalChannels<<" estimateSpace = "<<estimateSpace<<"MB";
    previewinfo[EstimatedSpace] = estimateSpace;

    auto dlg = new SummaryDialog(previewinfo);
    setWindowAlignCenter(dlg);
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    int ret = dlg->exec();
    if (ret == QDialog::Rejected)
        return;

    AssemblerPointer->assemble(TcpFramePool.loadExperEvent,previewinfo);
    auto json = AssemblerPointer->message();

    SocketPointer->exec(TcpFramePool.loadExperEvent,json);
#ifndef notusetoupcamera
    ToupCameraPointer->closeCamera();
#else
    closeCamera();
#endif
    if (ParserResult.toBool()) {
        QMessageBox::information(this,InformationChinese,tr("Successfully launched the experiment!"));

#ifdef uselabelcanvas
        livecanvas->setPixmap(QPixmap());
#else
        livecanvas->setImage(QImage());
#endif
        photocanvas->setImage(QImage());
    }

        // 组孔的信息要写到数据库,待写
        foreach(auto group, patterninfo.keys())
        {
            auto groupinfo = patterninfo[group].value<QVariantMap>();
            foreach(auto holename,groupinfo.keys())
            {
                auto holeinfo = groupinfo[holename].value<QVariantMap>();

                auto grouppoints = holeinfo[HoleGroupCoordinatesField].value<QPointVector>();

                auto coordinate = holeinfo[HoleCoordinateField].toPoint();
                auto viewsize = holeinfo[HoleViewSizeField].toInt();
                auto viewpoints = holeinfo[HoleViewPointsField].value<ViewPointVector>();
                auto uipoints = holeinfo[HoleViewUiPointsField].value<ViewPointVector>();
                auto viewrects = holeinfo[HoleViewRectsField].value<ViewRectFVector>();

                auto expertype = holeinfo[HoleExperTypeField].toString();
                auto medicine = holeinfo[HoleMedicineField].toString();
                auto dose = holeinfo[HoleDoseField].toString();
                auto unit = holeinfo[HoleDoseUnitField].toString();
        }
    }
}

void Preview::stopExper()
{
    QVariantMap  m;
    m[FrameField] = TcpFramePool.stopExperEvent;
    m[StopField] = 1;
    AssemblerPointer->assemble(TcpFramePool.stopExperEvent,m);
    auto json = AssemblerPointer->message();

    SocketPointer->exec(TcpFramePool.stopExperEvent,json,true);
    if (ParserResult.toBool()) {
        QMessageBox::information(this,InformationChinese,tr("Successfully stop the experiment!"));
    }
}