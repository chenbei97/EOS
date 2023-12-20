/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-31 14:06:48
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-17 16:16:08
 * @FilePath: \EOS\interface\src\preview\preview_tcp.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "preview.h"

void Preview::parseResult(const QString & f,const QVariant & d)
{ // 任何来自服务端的消息都在这里
    if (d.toBool() && f == TcpFramePool.adjustBrightEvent) {
        LOG<<"adjust bright successful!";
    }

    if (d.toInt() == 1 && f == TcpFramePool.experFinishedEvent) {
        LOG<<"exper is finished, open camera";
        if (!ToupCameraPointer->isOpen())
            ToupCameraPointer->openCamera();
    }

    if (f == TcpFramePool.moveMachineEvent && d.toBool()) {
        LOG<<"move machine successful!"; // 使用异步时也可以绑定信号来实现
    }
}

void Preview::previewSlideEvent(const QPointF& point)
{
    LOG<<"slide point: "<<point;
}

void Preview::stitchSlide()
{ // slide 拼图功能
    auto brand = previewtool->boxInfo(WellBoxTitle)[BrandField].toUInt();
    if(brand == SlideIndexInBrand) {
        canvasmode->changeMode(CanvasMode::PhotoMode);
        photocanvas->setStrategy(PhotoCanvas::GridPixmap);

        // photocanvas的边界矩形的出现和删除只和slidepattern有关,是同步的
        Q_ASSERT(slidepattern->haveSlide() == photocanvas->hasBoundingRect());
        if (slidepattern->haveSlide()) { // 确实选中一部分区域
            auto points = slidepattern->viewPoints(false); // 载玻片框选区域的离散归一化中心坐标
            photocanvas->clearGridImage(); // 清除可能的上次拼图缓存
            // 1.逐坐标发给下位机,让其移动电机
            for(auto pt: points) {

                // 2.获取照片实现拼图效果
                auto pix = ToupCameraPointer->capture();

                // 3.phtotcanvas内部绘制网格图
                photocanvas->appendImage(pix,pt);
            }



        }
    }
}

void Preview::adjustFocus(double val)
{ // 手动调焦,来自focus_slider的信号,目前是移动滑动条时就会触发,点击2个细调按钮也会触发
    //LOG<<"focus = "<<val;
    QVariantMap m;
    m[FocusField] = val;
    m[FrameField] = TcpFramePool.manualFocusEvent;
    auto msg = assembleManualFocusEvent(m);
    SocketPointer->exec(TcpFramePool.manualFocusEvent,msg);
    if (ParserResult.toBool()) {
        LOG<<"adjust focus to "<<val<<"successful!";
    }
}

void Preview::toggleObjective(int objective,int objective_loc,int isPh)
{ // objective: 物镜倍数代号 objective_loc: 切到的物镜位置代号 isPH:指示是否为PH类型
    // 切换物镜同时动电机事件,setting更改物镜设置时也会触发
    // 这个初始化setting时会初始化4个位置的物镜,从而会触发本函数,在mainwindow没构造完成就发了命令
    //LOG<<"objective = "<<objective<<" loc = "<<objective_loc<<" isph = "<<isPh;
    QVariantMap m;
    m[ObjectiveField] = objective;
    m[ObjectiveLocationField] = objective_loc;
    m[IsPHField] = isPh;

    AssemblerPointer->assemble(TcpFramePool.toggleObjectiveEvent,m);
    auto msg = AssemblerPointer->message();

    SocketPointer->exec(TcpFramePool.toggleObjectiveEvent,msg);
    if (ParserResult.toBool()) {
        LOG<<"toggle objective successful! magnification ="<<ObjectiveMagnificationFields[objective]<<"isPH?"<<bool(isPh);
    } else {LOG<<"toggle objective failed!";}
}

void Preview::closeChannel(int option)
{
    QVariantMap m;
    m[CurrentChannelField] = option;
    m[TurnOffLight] = 1;
    m[BrightField] = -1;
    SocketPointer->exec(TcpFramePool.toggleChannelEvent,assembleToggleChannelEvent(m),true);
    if (ParserResult.toBool()) {
        switch(option) {
            case 0:LOG<<"close br channel";
                break;
            case 1:LOG<<"close ph channel";
                break;
            case 2:LOG<<"close gfp channel";
                break;
            case 3:LOG<<"close rfp channel";
                break;
            case 4:LOG<<"close dapi channel";
                break;
        }

    }
}

void Preview::toggleChannel(int option)
{
    auto toolinfo = previewtool->toolInfo();
    auto current_channel = toolinfo[CurrentChannelField].toString();

    if (current_channel.isEmpty()) {
        LOG<<"not have channel is open";
        Q_ASSERT(option == -1); // 如果是关灯,此时当前通道是空字符串,此时传来的option=-1,那么这个时候就直接关灯即可
        // 不过关灯需要知道关哪个灯,这个工作在closeChannel去做
        return; // 灯全灭的情况直接返回
    }

    auto current_info = toolinfo[CurrentInfoField].value<CameraInfo>();

    QVariantMap m;
    m[CurrentChannelField] = getIndexFromFields(current_channel).toInt();
    m[BrightField] = current_info[BrightField].toDouble();
    m[TurnOffLight] = 0;

    AssemblerPointer->assemble(TcpFramePool.toggleChannelEvent,m);
    auto msg = AssemblerPointer->message();

    SocketPointer->exec(TcpFramePool.toggleChannelEvent,msg, true);

    if (ParserResult.toBool()) {
        LOG<<"turn on light successful! current channel is"<<m[CurrentChannelField].toInt();
    }
}

void Preview::adjustLens(int option)
{ // 0-left,1-rop,2-right,3-bottom 微调节镜头
    QJsonObject object;
    object[FrameField] = AdjustLensEvent;
    object[DirectionField] = option;
    TcpAssemblerDoc.setObject(object);
    auto msg = AppendSeparateField(TcpAssemblerDoc.toJson());

    SocketPointer->exec(TcpFramePool.adjustLensEvent,msg);

    if (ParserResult.toBool()) {
        LOG<<"move direction: "<<option<<"successful!";
        wellview->adjustViewPoint(option);
    } else LOG<<"move direction: "<<option<<"failed!";
}

void Preview::adjustBright(int br)
{
    auto toolinfo = previewtool->toolInfo();

    auto current_channel = toolinfo[CurrentChannelField].toString();

    if (current_channel.isEmpty()) {
        LOG<<"没有通道的灯被打开,不执行滑动条的参数调整!";
        return; // 如果通道无效,没有开灯,调节参数没有意义,不发给下位机
    }

    // 滑动条速度很快,这里组装不再通过Assembler来组装,可能同步会出问题,直接组装
    QJsonObject object;
    object[FrameField] = AdjustBrightEvent;
    object[FieldAdjustBrightEvent.bright] = br;
    object[FieldAdjustBrightEvent.current_channel] = getIndexFromFields(current_channel).toInt();
    TcpAssemblerDoc.setObject(object);
    auto msg = AppendSeparateField(TcpAssemblerDoc.toJson());;

    SocketPointer->exec_queue(TcpFramePool.adjustBrightEvent,msg); // 不适用同步
}

void Preview::adjustCamera(int exp,int gain)
{
    LOG<<"adjusting exp to"<<exp<<" gain to"<<gain;
#ifndef notusetoupcamera
    //LOG<<"exposureOption = "<<ToupCameraPointer->exposureOption();
    if (!ToupCameraPointer->exposureOption()) {
        ToupCameraPointer->setExposure(exp);
        ToupCameraPointer->setGain(gain);
    }
;
#else
    //LOG<<"exposureOption = "<<exposureOption();
    if (!exposureOption()) { // 设置手动曝光,exp option = 0
        setExposure(exp);
        setGain(gain);
    }
#endif
}

void Preview::takingPhoto()
{
    auto toolinfo = previewtool->toolInfo();
    auto current_info = toolinfo[CurrentInfoField].value<CameraInfo>();
    int exp = current_info[ExposureField].toUInt();
    int ga = current_info[GainField].toUInt();

#ifndef notusetoupcamera
        ToupCameraPointer->setExposure(exp);
        ToupCameraPointer->setGain(ga);
        auto pix = ToupCameraPointer->capture();
        auto current_channel = toolinfo[CurrentChannelField].toString();
        previewtool->captureImage(pix,current_channel); // 把当前通道拍到的图像传回去用于后续合成通道,以及显示到缩略图
        LOG<<"current (exp,gain,bright) is ("<<ToupCameraPointer->exposure()<<ToupCameraPointer->gain()<<current_info[BrightField].toInt()<<")";
#else
        setExposure(exp);
        setGain(ga);
        auto pix = capture();
        auto current_channel = toolinfo[CurrentChannelField].toString();
        previewtool->captureImage(pix,current_channel); // 把当前通道拍到的图像传回去用于后续合成通道
        LOG<<"current (exp,gain,bright) is ("<<gain()<<current_info[BrightField].toInt()<<")";
#endif

        canvasmode->changeMode(CanvasMode::PhotoMode);
        photocanvas->setStrategy(PhotoCanvas::SinglePixmap);
        photocanvas->setImage(pix);

        // 存图功能,目前默认存到temp下,用户会更改前缀目录,或者需要按分不同通道文件夹归类等
        createPath(TakePhotoTempPath);
        auto path = TakePhotoTempPath
                    +QDateTime::currentDateTime().toString(DefaultImageSaveDateTimeFormat)+JPGSuffix;
        if (!pix.isNull())
            pix.save(path,JPGField,DefaultImageQuality);

}

void Preview::previewViewEvent(const QPointF &viewpoint)
{
    auto toolinfo = previewtool->toolInfo();

    // 预览事件需要的参数
    auto objective = getIndexFromFields(toolinfo[ObjectiveField].toString()).toUInt();
    auto brand = toolinfo[BrandField].toUInt();
    auto manufacturer = toolinfo[ManufacturerField].toUInt();
    auto wellsize = toolinfo[WellsizeField].toUInt();
    auto viewsize = ViewCircleMapFields[manufacturer][brand][objective];//点孔触发预览的时候需要传递viewsize
    auto holecoordinate = wellview->viewInfo()[HoleCoordinateField].toPoint();
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
    m[BrightField] = bright;
    m[IsHoleField] = 0;

    AssemblerPointer->assemble(TcpFramePool.previewEvent,m);
    auto msg = AssemblerPointer->message();
    SocketPointer->exec(TcpFramePool.previewEvent,msg);
    if (ParserResult.toBool()) {
        LOG<<"move to view point "<<viewpoint<<"successful!";
    } else LOG<<"move to view point "<<viewpoint<<"failed!";
}

void Preview::previewHoleEvent(const QPoint &holepoint)
{
    if (holepoint == QPoint(-1,-1))
        return;

    auto toolinfo = previewtool->toolInfo();

    // 预览事件需要的参数
    auto objective = getIndexFromFields(toolinfo[ObjectiveField].toString()).toUInt();
    auto brand = toolinfo[BrandField].toUInt();
    auto manufacturer = toolinfo[ManufacturerField].toUInt();
    auto wellsize = toolinfo[WellsizeField].toUInt();
    auto viewsize = ViewCircleMapFields[manufacturer][brand][objective];//点孔触发预览的时候需要传递viewsize
    auto holecoordinate = holepoint;
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
    LOG<<"exec "<<holepoint;
    SocketPointer->exec(TcpFramePool.previewEvent,msg);
    if (ParserResult.toBool()) {
        LOG<<"move to hole point "<<holepoint<<" successful!";
    } else LOG<<"move to hole point "<<holepoint<<" failed!";
}

void Preview::loadExper()
{
    LOG<<"收集启动实验需要的信息";
    auto patterninfo = wellpattern->patternInfo();
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
    auto totalViews = wellpattern->numberOfViews();
    auto totalChannels = channels.count("1"); // 为1的是勾选上的
    auto estimateSpace = calculateExperSpaceMB(totalViews,totalChannels);
    LOG<<"总的孔视野数 = "<<totalViews<<"勾选的通道数 = "<<totalChannels<<" 预计占据空间 = "<<estimateSpace<<"MB";
    previewinfo[EstimatedSpaceField] = estimateSpace;

    auto dlg = new SummaryDialog(previewinfo);
    setWindowAlignCenter(dlg);
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    LOG<<"打开实验信息面板";
    int ret = dlg->exec();
    if (ret == QDialog::Rejected)
        return;
    LOG<<"首先关闭相机";
#ifndef notusetoupcamera
    ToupCameraPointer->closeCamera(); // 先关相机后执行
#else
    closeCamera();
#endif

    LOG<<"然后清理图像";
#ifdef uselabelcanvas
    livecanvas->setPixmap(QPixmap());
#else
    livecanvas->setImage(QImage(),1);
#endif
    photocanvas->setStrategy(PhotoCanvas::SinglePixmap);
    photocanvas->setImage(QImage());
    
    LOG<<"清理图像完毕";

    AssemblerPointer->assemble(TcpFramePool.loadExperEvent,previewinfo);
    auto json = AssemblerPointer->message();
    LOG<<"然后发送启动实验命令";
    SocketPointer->exec(TcpFramePool.loadExperEvent,json);

    if (ParserResult.toBool()) {
        QMessageBox::information(this,InformationChinese,tr("Successfully launched the experiment!"));
    }
    LOG<<"启动实验结束";
}

void Preview::exportExperConfig(const QString& path)
{ // 导出实验配置
    previewinfo[PreviewPatternField] = wellpattern->patternInfo();
    previewinfo[PreviewToolField] = previewtool->toolInfo();
#ifdef usetab
    previewinfo[ExperToolField] = expertool->toolInfo();
#endif
    auto json = assembleExportExperEvent(previewinfo);
    JsonReadWrite m; // 借助工具类写到文件内
    m.writeJson(path,json);
    // 存数据库,直接存整个json文件是最简单的
}

void Preview::importExperConfigV1(const QString& path)
{// 导入实验配置,对于camera_channel和group字段要特殊解析

    ConfigReadWrite m; // 借助工具类读取文件
    auto json = m.readJson(path);

    m.parseJson(json);
    auto result = m.map();

    LOG<<result[GroupField];

    previewtool->importExperConfig(result);
#ifdef usetab
    expertool->importExperConfig(result);
#endif

    auto brand = result[BrandField].toUInt();
    auto manufacturer = result[ManufacturerField].toUInt();
    auto objective = result[ObjectiveField].toUInt();
    auto viewsize = ViewCircleMapFields[manufacturer][brand][objective];

    auto info = result[GroupField].value<QVariantMap>();
    for(auto group:info.keys()){
        auto groupinfo = info[group].value<QVariantMap>();
                foreach(auto hole,groupinfo.keys()) {
                auto holeinfo = groupinfo[hole].value<QVariantMap>();
                //LOG<<holeinfo;
                auto holepoint = holeinfo[HoleCoordinateField].toPoint();
                auto viewpoints = holeinfo[PointsField].value<QPointFVector>();

                // 把holePoint这个孔的信息更改(和wellsize有关,所以需要先更新wellpattern的信息就不会越界了)
                wellpattern->importHoleInfoV1(holepoint,group,viewpoints,viewsize);
                wellview->importViewInfoV1(holepoint,viewpoints,viewsize);
            }

    }
}

void Preview::importExperConfig(const QString &path)
{
    ConfigReadWrite m; // 借助工具类读取文件
    auto json = m.readJson(path);

    m.parseJson(json);
    auto result = m.map();
    auto groupInfos = result[GroupField].value<QVariantMap>();
    auto patterSize = convertToPointF(result[HoleSizeField].toString());
    auto viewMode = result[ViewModeField].toInt();

    QHoleInfoVector holeInfoVec;
    for(auto var1: groupInfos.values()) {
        auto groupInfo = var1.value<WellGroupInfo>();
        for(auto var2: groupInfo.values()) {
            auto holeInfo = var2.value<HoleInfo>();
            holeInfoVec.append(holeInfo);
        }
    }
    wellpattern->setPatternSize(patterSize.x(),patterSize.y());
    wellpattern->importHoleInfo(holeInfoVec,ViewMode(viewMode));
    wellview->importViewInfo(holeInfoVec,ViewMode(viewMode));
}

void Preview::stopExper()
{
    QVariantMap  m;
    m[FrameField] = TcpFramePool.stopExperEvent;
    m[StopField] = 1;
    AssemblerPointer->assemble(TcpFramePool.stopExperEvent,m);
    auto json = AssemblerPointer->message();

    SocketPointer->exec(TcpFramePool.stopExperEvent,json);
    if (ParserResult.toBool()) {
        QMessageBox::information(this,InformationChinese,tr("Successfully stop the experiment!"));
    }
}