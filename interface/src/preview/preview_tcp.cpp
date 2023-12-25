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
    LOG<<"[async] frame:"<<f<<"result:"<<d;
    if (f == TcpFramePool.previewEvent) { // 1
        d.toBool()?
        LOG<<"[async] preview event successful!":
        LOG<<"[async] preview event failed!";
    }

    if (f == TcpFramePool.askConnectedStateEvent) { // 2
        d.toBool()?
        LOG<<"[async] socket is connect successful!":
        LOG<<"[async] socket is connect failed!";
    }

    if (f == TcpFramePool.askActivateCodeEvent) { // 3
        LOG<<"[async] activate code: "<<d.toString();
    }

    if (f == TcpFramePool.adjustBrightEvent) { // 4
        d.toBool()?LOG<<"[async] adjust bright successful!":
        LOG<<"[async] adjust bright failed!";
    }

    if (f == TcpFramePool.toggleChannelEvent) { // 5
        d.toBool()?LOG<<"[async] toggle channel successful!":
        LOG<<"[async] toggle channel failed!";
    }

    if (f == TcpFramePool.adjustLensEvent) { // 6
        d.toBool()?LOG<<"[async] adjust len successful!":
        LOG<<"[async] adjust len failed!";
    }

    if (f == TcpFramePool.moveMachineEvent) { // 7
        d.toBool()?LOG<<"[async] move machine successful!":
        LOG<<"[async] move machine failed!";
    }

    if (f == TcpFramePool.toggleChannelEvent) { // 9
        d.toBool()?LOG<<"[async] toggle objective successful!":
        LOG<<"[async] toggle objective failed!";
    }

    if (f == TcpFramePool.manualFocusEvent) { // 11
        d.toBool()?LOG<<"[async] adjust focus successful!":
        LOG<<"[async] adjust focus failed!";
    }

    if (f == TcpFramePool.experFinishedEvent) { // 14
        if (d.toInt()) {
            LOG<<"[async] exper is finished, open camera";
            if (!ToupCameraPointer->isOpen())
                ToupCameraPointer->openCamera();
        } else LOG<<"[async] exper is not finished";
    }


    if (f == "test0x2") {
        d.toBool()?LOG<<"[async] auto focus successful!":LOG<<"[async] auto focus failed!";
    }
}

void Preview::previewSlideEvent(const QPointF& point)
{
    LOG<<"slide point: "<<point;
}

void Preview::stitchSlide()
{ // slide 拼图功能
    auto brand = wellbox->wellInfo()[BrandField].toUInt();
    if(brand == SlideIndexInBrand) { // 品牌是载玻片才响应
        canvasmode->changeMode(CanvasMode::PhotoMode);
        photocanvas->setStrategy(PhotoCanvas::GridPixmap);
        // photocanvas的边界矩形的出现和删除只和slidepattern有关,是同步的
        Q_ASSERT(slidepattern->haveSlide() == photocanvas->hasBoundingRect());
        if (slidepattern->haveSlide()) { // 确实选中一部分区域
            auto points = slidepattern->viewPoints(false); // 载玻片框选区域的离散归一化中心坐标
            photocanvas->clearGridImage(); // 清除可能的上次拼图缓存

            auto f = "test0x3";
            QJsonObject object;
            object[FrameField] = f;

            // 1.逐坐标发给下位机,让其移动电机
//            for(auto pt: points) {
//                object[XField] = pt.x();
//                object[YField] = pt.y();
//                TcpAssemblerDoc.setObject(object);
//                auto json = AppendSeparateField(TcpAssemblerDoc.toJson());
//                SocketPointer->exec(f,json);
//                if (ParserResult.toBool()) {
//                    // 2.获取照片实现拼图效果
//                    auto pix = ToupCameraPointer->capture();
//                    // 3.phtotcanvas内部绘制网格图
//                    photocanvas->appendImage(pix,pt);
//                }
//            }

            auto count = points.count();
            unsigned i = 0;
            while(i < count ) {
                auto pt = points[i];
                object[XField] = pt.x();
                object[YField] = pt.y();
                TcpAssemblerDoc.setObject(object);
                auto json = AppendSeparateField(TcpAssemblerDoc.toJson());
                SocketPointer->exec(f,json,i);
                if (ParserResult.toBool()) {
                    auto pix = ToupCameraPointer->capture();
                    photocanvas->appendImage(pix,pt);
                }
                ++i;
            }
        } else LOG<<"empty slide";
    } else {
        LOG<<"current brand is not slide";
    }
}

void Preview::adjustFocus(double val)
{ // 手动调焦,来自focus_slider的信号,目前是移动滑动条时就会触发,点击2个细调按钮也会触发
    //LOG<<"focus = "<<val;
    QVariantMap m;
    m[FocusField] = val;
    m[FrameField] = TcpFramePool.manualFocusEvent; // 11
    auto msg = assembleManualFocusEvent(m);
    SocketPointer->exec(TcpFramePool.manualFocusEvent,msg);
    if (ParserResult.toBool()) {
        LOG<<"[sync] adjust focus to"<<val<<"successful!";
    } else LOG<<"[sync] adjust focus to"<<val<<"failed!";
}

void Preview::autoFocus()
{
    //    QJsonObject object;
//    object[FrameField] = AutoFocusEvent;
//    TcpAssemblerDoc.setObject(object);
//    auto msg = AppendSeparateField(TcpAssemblerDoc.toJson());
//    SocketPointer->exec(TcpFramePool.autoFocusEvent,msg);

    auto f = "test0x2";
    QJsonObject object;
    object[FrameField] = f;
    object[PathField] = "";
    TcpAssemblerDoc.setObject(object);
    auto json = TcpAssemblerDoc.toJson();
    json = AppendSeparateField(json);
    unsigned i = 0;
    SocketPointer->setWaitText(WaitMessageBoxWaitFocusMsg);
    SocketPointer->exec(f,json);
    while (i < 10 && ParserResult.toBool()) {
        if (i == 0) {
            LOG<<"[sync] ready auto focus";
        } else {
            LOG<<"[sync] send"<<i<<".jpg successful";
        }
        object[PathField] = QString("%1.jpg").arg(i);
        TcpAssemblerDoc.setObject(object);
        json = AppendSeparateField(TcpAssemblerDoc.toJson());
        SocketPointer->exec(f,json,i);
        ++i;
    }
    LOG<<"[sync] send"<<i<<".jpg successful";
    SocketPointer->resetWaitText();
}

void Preview::toggleObjective(int objective,int objective_loc,int isPh)
{ // 9, objective: 物镜倍数代号 objective_loc: 切到的物镜位置代号 isPH:指示是否为PH类型
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
        LOG<<"[sync] toggle objective successful! magnification ="<<ObjectiveMagnificationFields[objective]<<"isPH?"<<bool(isPh);
    } else {LOG<<"[sync] toggle objective failed!";}
}

void Preview::closeChannel(int option)
{ // 5
    QVariantMap m;
    m[CurrentChannelField] = option;
    m[TurnOffLight] = 1;
    m[BrightField] = -1;
    SocketPointer->exec(TcpFramePool.toggleChannelEvent,assembleToggleChannelEvent(m),true);
    if (ParserResult.toBool()) {
        switch(option) {
            case 0:LOG<<"[sync] close br channel";
                break;
            case 1:LOG<<"[sync] close ph channel";
                break;
            case 2:LOG<<"[sync] close gfp channel";
                break;
            case 3:LOG<<"[sync] close rfp channel";
                break;
            case 4:LOG<<"[sync] close dapi channel";
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
        LOG<<"[sync] turn on light successful! current channel is"<<m[CurrentChannelField].toInt();
    } else LOG<<"[sync] turn on light failed! current channel is"<<m[CurrentChannelField].toInt();
}

void Preview::adjustLens(int option)
{ // 0-left,1-rop,2-right,3-bottom 微调节镜头
    if (!wellview->isGrouped() || !wellview->isValidMousePoint())
        return;
    stack_view_pattern->setCurrentWidget(stackview);
    QJsonObject object;
    object[FrameField] = AdjustLensEvent;
    object[DirectionField] = option;
    TcpAssemblerDoc.setObject(object);
    auto msg = AppendSeparateField(TcpAssemblerDoc.toJson());

    SocketPointer->exec(TcpFramePool.adjustLensEvent,msg); // 6

    if (ParserResult.toBool()) {
        LOG<<"[sync] move direction: "<<option<<"successful!";
        wellview->adjustViewPoint(option);
    } else LOG<<"[sync] move direction: "<<option<<"failed!";
}

void Preview::adjustBright(int br)
{ // 4
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
    //LOG<<"exposureOption = "<<ToupCameraPointer->exposureOption();
    if (!ToupCameraPointer->exposureOption()) {
        ToupCameraPointer->setExposure(exp);
        ToupCameraPointer->setGain(gain);
    }
}

void Preview::takingPhoto()
{
    auto toolinfo = previewtool->toolInfo();
    auto current_info = toolinfo[CurrentInfoField].value<CameraInfo>();
    int exp = current_info[ExposureField].toUInt();
    int ga = current_info[GainField].toUInt();

    ToupCameraPointer->setExposure(exp);
    ToupCameraPointer->setGain(ga);
    auto pix = ToupCameraPointer->capture();
    auto current_channel = toolinfo[CurrentChannelField].toString();
    previewtool->captureImage(pix,current_channel); // 把当前通道拍到的图像传回去用于后续合成通道,以及显示到缩略图
    LOG<<"current (exp,gain,bright) is ("<<ToupCameraPointer->exposure()<<ToupCameraPointer->gain()<<current_info[BrightField].toInt()<<")";

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
{ // 1
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
#ifdef viewRowColUnEqual
    m[HoleViewSizeField].setValue(Dimension2D(viewsize,viewsize));
#else
    m[HoleViewSizeField] = viewsize;
#endif
    m[HoleCoordinateField] = holecoordinate;
    m[ViewCoordinateField] = viewpoint;
    m[BrightField] = bright;
    m[IsHoleField] = 0;

    AssemblerPointer->assemble(TcpFramePool.previewEvent,m);
    auto msg = AssemblerPointer->message();
    SocketPointer->exec(TcpFramePool.previewEvent,msg);
    if (ParserResult.toBool()) {
        LOG<<"[sync] move to view point "<<viewpoint<<"successful!";
    } else LOG<<"[sync] move to view point "<<viewpoint<<"failed!";
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
    ToupCameraPointer->setExposure(exp);
    ToupCameraPointer->setGain(ga);
    QVariantMap m;
    m[ObjectiveField] = objective;
    m[BrandField] = brand;
    m[ManufacturerField] = manufacturer;
    m[WellsizeField] = wellsize;
#ifdef viewRowColUnEqual
    m[HoleViewSizeField].setValue(Dimension2D(viewsize,viewsize));
#else
    m[HoleViewSizeField] = viewsize;
#endif
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
    auto patterninfo = wellpattern->patternInfo();
    auto previewinfo = previewtool->toolInfo();
    previewinfo[PreviewPatternField] = patterninfo;
    previewinfo[PreviewToolField] = previewinfo;
    auto experinfo = expertool->toolInfo();
    previewinfo[ExperToolField] = experinfo;

    auto channels = experinfo[FieldLoadExperEvent.channel].toString().split(",",QString::SkipEmptyParts);
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
    ToupCameraPointer->closeCamera(); // 先关相机后执行
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
    previewinfo[ExperToolField] = expertool->toolInfo();
    auto json = assembleExportExperEvent(previewinfo);
    JsonReadWrite m; // 借助工具类写到文件内
    m.writeJson(path,json);
    // 存数据库,直接存整个json文件是最简单的
}

//void Preview::importExperConfigV1(const QString& path)
//{// 导入实验配置,对于camera_channel和group字段要特殊解析
//
//    ConfigReadWrite m; // 借助工具类读取文件
//    auto json = m.readJson(path);
//
//    m.parseJson(json);
//    auto result = m.map();
//
//    LOG<<result[GroupField];
//
//    previewtool->importExperConfig(result);
//#ifdef usetab
//    expertool->importExperConfig(result);
//#endif
//
//    auto brand = result[BrandField].toUInt();
//    auto manufacturer = result[ManufacturerField].toUInt();
//    auto objective = result[ObjectiveField].toUInt();
//    auto viewsize = ViewCircleMapFields[manufacturer][brand][objective];
//
//    auto info = result[GroupField].value<QVariantMap>();
//    for(auto group:info.keys()){
//        auto groupinfo = info[group].value<QVariantMap>();
//                foreach(auto hole,groupinfo.keys()) {
//                auto holeinfo = groupinfo[hole].value<QVariantMap>();
//                //LOG<<holeinfo;
//                auto holepoint = holeinfo[HoleCoordinateField].toPoint();
//                auto viewpoints = holeinfo[PointsField].value<QPointFVector>();
//
//                // 把holePoint这个孔的信息更改(和wellsize有关,所以需要先更新wellpattern的信息就不会越界了)
//                wellpattern->importHoleInfoV1(holepoint,group,viewpoints,viewsize);
//                wellview->importViewInfoV1(holepoint,viewpoints,viewsize);
//            }
//
//    }
//}

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