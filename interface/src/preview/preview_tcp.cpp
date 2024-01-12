/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-31 14:06:48
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-27 15:42:56
 * @FilePath: \EOS\interface\src\preview\preview_tcp.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "preview.h"

void Preview::parseResult(const QString & f,const QVariant & d)
{ // 任何来自服务端的消息都在这里
    //LOG<<"[async] frame:"<<f<<"result:"<<d;
    // 0
    if (f == TcpFramePool.previewEvent) {
        d.toBool()?
        LOG<<"[async] preview event successful!":
        LOG<<"[async] preview event failed!";
    }
    // 1
    if (f == TcpFramePool.loadExperEvent) {
        if (d.toBool()) {
            LOG<<"[async] start experiment successful!";
            QMessageBox::information(this,InformationChinese,tr("Start the experiment successfully !"));
            bool enable = false;
            historybox->setEnabled(enable);
            wellbox->setEnabled(enable);
            objectivebox->setEnabled(enable);
            viewModeBox->setEnabled(enable);
            channelbox->setEnabled(enable);
            stack_view_pattern->setEnabled(enable);
            camerabox->setEnabled(enable);
            timebox->setEnabled(enable);
            focusbox->setEnabled(enable);
            recordbox->setEnabled(enable);
            zstackbox->setEnabled(enable);
            savebox->setChildWidgetsEnabled(enable);
        } else {
            LOG<<"[async] start experiment failed!";
            QMessageBox::information(this,InformationChinese,tr("Start the experiment failed!"));
        }
    }
    // 2
    if (f == TcpFramePool.askConnectedStateEvent) {
        d.toBool()?
        LOG<<"[async] socket is connect successful!":
        LOG<<"[async] socket is connect failed!";
    }
    // 3
    if (f == TcpFramePool.askActivateCodeEvent) {
        LOG<<"[async] activate code: "<<d.toString();
    }
    // 4
    if (f == TcpFramePool.adjustBrightEvent) {
        d.toBool()?LOG<<"[async] adjust bright successful!":
        LOG<<"[async] adjust bright failed!";
    }
    // 5
    if (f == TcpFramePool.toggleChannelEvent) {
        d.toBool()?LOG<<"[async] toggle channel successful!":
        LOG<<"[async] toggle channel failed!";
    }
    // 6
    if (f == TcpFramePool.adjustLensEvent) {
        d.toBool()?LOG<<"[async] adjust len successful!":
        LOG<<"[async] adjust len failed!";
    }
    // 7
    if (f == TcpFramePool.moveMachineEvent) {
        d.toBool()?LOG<<"[async] move machine successful!":
        LOG<<"[async] move machine failed!";
    }
    // 8
    if (f == TcpFramePool.stopExperEvent) {
        d.toBool()?LOG<<"[async] stop experiment successful!":
        LOG<<"[async] stop experiment failed!";
        if (d.toBool()) {
            QMessageBox::information(this,InformationChinese,tr("Successfully stop the experiment!"));
        }
    }
    // 9
    if (f == TcpFramePool.toggleObjectiveEvent) { // 9
        d.toBool()?LOG<<"[async] toggle objective successful!":
        LOG<<"[async] toggle objective failed!";
    }
    // 10 录制视频无
    // 11
    if (f == TcpFramePool.manualFocusEvent) {
        d.toBool()?LOG<<"[async] manual adjust focus successful!":
        LOG<<"[async] manual adjust focus failed!";
    }
    // 12
    if (f == TcpFramePool.autoFocusEvent) {
        auto pair = d.value<QPair<bool,double>>();
        if (pair.first) {
            LOG<<"[async] wait move machine auto focus successful!new focus is"<<pair.second;
        } else
            LOG<<"[async] wait move machine auto focus failed!";
    }
    // 13
    if (f == TcpFramePool.channelMergeEvent) {
        d.toString().isEmpty()?
        LOG<<"[async] merge channel image failed!":
        LOG<<"[async] merge channel image successful! merge_path: "<<d.toString();
    }
    // 14
    if (f == TcpFramePool.experFinishedEvent) { // 14
        if (d.toInt()) {
            LOG<<"[async] exper is finished, ready open camera";
            if (!ToupCameraPointer->isOpen())
                ToupCameraPointer->openCamera();
            bool enable = true;
            historybox->setEnabled(enable);
            wellbox->setEnabled(enable);
            objectivebox->setEnabled(enable);
            viewModeBox->setEnabled(enable);
            channelbox->setEnabled(enable);
            stack_view_pattern->setEnabled(enable);
            camerabox->setEnabled(enable);
            timebox->setEnabled(enable);
            focusbox->setEnabled(enable);
            recordbox->setEnabled(enable);
            zstackbox->setEnabled(enable);
            savebox->setChildWidgetsEnabled(enable);
        } else LOG<<"[async] exper is not finished, not open camera";
    }

    if (f == "test0x2") {
        d.toBool()?LOG<<"[async] auto focus successful!":LOG<<"[async] auto focus failed!";
    }
}

void Preview::previewViewEvent(const QPointF &viewpoint)
{ //  帧头: 0
    auto objective = objectivebox->currentObjective();
    auto brand = wellbox->wellBrand();
    auto manufacturer = wellbox->wellManufacturer();
    auto wellsize = wellbox->wellSize();
    auto viewsize = ViewCircleMapFields[manufacturer][brand][objective];//点孔触发预览的时候需要传递viewsize
    auto holecoordinate = wellview->viewInfo()[HoleCoordinateField].toPoint();
    auto bright = camerabox->bright();

    // 自己需要的相机参数
    int exp = camerabox->exposure();
    int ga = camerabox->gain();
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
    m[ViewCoordinateField] = viewpoint;
    m[BrightField] = bright;
    m[IsHoleField] = 0;

    AssemblerPointer->assemble(TcpFramePool.previewEvent,m);
    auto msg = AssemblerPointer->message();
    SocketPointer->exec(TcpFramePool.previewEvent,msg);
    if (ParserResult.toBool()) {
        LOG<<"[sync] move to view point "<<viewpoint<<"successful!";
        //autoFocus();
    } else LOG<<"[sync] move to view point "<<viewpoint<<"failed!";
}

void Preview::previewHoleEvent(const QPoint &holePoint)
{// 帧头: 0
    if (holePoint == QPoint(-1,-1))
        return;

    auto channel = channelbox->currentChannel();
    if (channel.isEmpty()) {
        // 没有开灯,默认就开明场的灯
        channelbox->openDefaultChannel();
    }

    // 预览事件需要的参数
    auto objective = objectivebox->currentObjective();
    auto brand = wellbox->wellBrand();
    auto manufacturer = wellbox->wellManufacturer();
    auto wellsize = wellbox->wellSize();
    auto viewsize = ViewCircleMapFields[manufacturer][brand][objective];//点孔触发预览的时候需要传递viewsize
    auto holecoordinate = holePoint;
    auto bright = camerabox->bright();

    // 自己需要的相机参数
    int exp = camerabox->exposure();
    int ga = camerabox->gain();
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
    SocketPointer->exec(TcpFramePool.previewEvent,msg);
    if (ParserResult.toBool()) {
        LOG<<"move to hole point "<<holePoint<<" successful!";
        //autoFocus();
    } else LOG<<"move to hole point "<<holePoint<<" failed!";
}

void Preview::loadExper()
{ // 帧头: 1

    { // 警告或者错误信息检验


        if (!wellbox->wellBrandName().contains(SlideField)) { // 孔板的情况下
            // (1) 分组或者选点情况
            if (wellpattern->getAllGroups().isEmpty()) {
                QMessageBox::critical(this,CriticalChinese,
                                      tr("The orifice plate has not been divided into groups and cannot start the experiment!"),QMessageBox::Abort);
                return;
            } else {
                for(auto hole: wellpattern->getAllHoles()) {
                    if (!wellpattern->numberOfViews(hole)) {
                        QMessageBox::warning(this,WarningChinese,
                                              tr("Detected that the orifice plate position [%1,%2] has not selected a valid field of view point!").arg(hole.x()).arg(hole.y()),QMessageBox::Yes);
                    }
                }
            }

            // (2) 整孔模式时提供警告
            if (wellpattern->viewMode() == ViewMode::WholeMode) {
                QMessageBox::warning(this,WarningChinese,
                                      tr("Please note that you have selected the whole hole mode, which will ignore all grouping and field of view points!"),QMessageBox::Yes);
            }
        }

        // (3) 保存的通道参数和勾选的通道不一致
        auto save_channels = camerabox->save_channels();
        if (save_channels.isEmpty()) {
            QMessageBox::warning(this,WarningChinese,
                                 tr("No channel has saved parameters, default exp, gain, bright will be executed!"),
                                 QMessageBox::Ok);
        } else {
            QString unsave_channels;
            for(auto channel: timebox->selectedChannelNames()) {
                if (!save_channels.contains(channel))
                    unsave_channels += channel+",";
            }
            unsave_channels.chop(1);
            if (!unsave_channels.isEmpty()) { // 没有没保存过的
                QMessageBox::warning(this,WarningChinese,
                                     tr("The channels you have checked [%1] not save parameters, default exp, gain, bright will be executed!").arg(unsave_channels),
                                     QMessageBox::Ok);
            }
        }

        if (!timebox->checkTime()) { // 时间不符合要求
            QMessageBox::critical(this,CriticalChinese,
                                  tr("The experimental time setting is incorrect, please verify the prompt!"),QMessageBox::Abort);
            return;
        }
    }

    auto channels = timebox->selectedChannels();
    auto totalViews = wellpattern->numberOfViews();
    auto totalChannels = channels.count("1"); // 为1的是勾选上的
    auto estimateSpace = calculateExperSpaceMB(totalViews,totalChannels);
    LOG<<"总的孔视野数 = "<<totalViews<<"勾选的通道数 = "<<totalChannels<<" 预计占据空间 = "<<estimateSpace<<"MB";

    previewinfo[EstimatedSpaceField] = estimateSpace;
    previewinfo[WellBoxTitle].setValue(wellbox->wellInfo());
    previewinfo[ObjectiveBoxTitle].setValue(objectivebox->objectiveInfo());
    previewinfo[ViewModeBoxTitle].setValue(viewModeBox->viewModeInfo());
    previewinfo[WellPatternField] = wellpattern->patternInfo();
    previewinfo[CameraFocusBoxTitle].setValue(camerabox->cameraFocusInfo());
    previewinfo[TimeBoxTitle].setValue(timebox->timeInfo());
    previewinfo[FocusBoxTitle].setValue(focusbox->focusInfo());
    previewinfo[ZStackBoxField].setValue(zstackbox->zstackInfo());

    auto dlg = new SummaryDialog(previewinfo);
    setWindowAlignCenter(dlg);
    dlg->setAttribute(Qt::WA_DeleteOnClose);

    int ret = dlg->exec();
    if (ret == QDialog::Rejected)
        return;
    ToupCameraPointer->closeCamera(); // 先关相机后执行

#ifdef uselabelcanvas
    livecanvas->setPixmap(QPixmap());
#else
    livecanvas->setImage(QImage(),1);
#endif
    photocanvas->setStrategy(PhotoCanvas::SinglePixmap);
    photocanvas->setImage(QImage());

    AssemblerPointer->assemble(TcpFramePool.loadExperEvent,previewinfo);
    auto json = AssemblerPointer->message();
    LOG<<"发送启动实验命令";
    SocketPointer->exec(TcpFramePool.loadExperEvent,json);
    JsonReadWrite m;
    m.writeJson(CURRENT_PATH+"/load_exper.json",json);
//    if (ParserResult.toBool()) {
//        QMessageBox::information(this,InformationChinese,tr("Successfully launched the experiment!"));
//    }
}

void Preview::askConnectState()
{// 帧头: 2
    SocketPointer->exec(TcpFramePool.askConnectedStateEvent,assembleAskConnectedStateEvent(QVariantMap()),true);
    //SocketPointer->exec_queue(TcpFramePool.askConnectedStateEvent,assembleAskConnectedStateEvent(QVariantMap()));
    //QMetaObject::invokeMethod(SocketPointer,"processRequestQueue",Qt::DirectConnection);
    if (ParserResult.toBool()) LOG<<"[sync] socket is connect successful!";
    else LOG<<"[sync] socket is connect failed!";
}

void Preview::askActivateCode()
{// 帧头: 3
    SocketPointer->exec(TcpFramePool.askActivateCodeEvent,assembleAskActivateCodeEvent(QVariantMap()),true);
    //SocketPointer->exec_queue(TcpFramePool.askActivateCodeEvent,assembleAskActivateCodeEvent(QVariantMap()));
    //QMetaObject::invokeMethod(SocketPointer,"processRequestQueue",Qt::DirectConnection);
    LOG<<"[sync] activate code is "<<ParserResult.toString();
}

void Preview::adjustBright(int br)
{ //帧头: 4
    auto current_channel = channelbox->currentChannel();

    if (current_channel.isEmpty()) {
        LOG<<"no channels was opened, reject adjust bright!";
        return; // 如果通道无效,没有开灯,调节参数没有意义,不发给下位机
    }

//    QJsonObject object;
//    object[FrameField] = AdjustBrightEvent;
//    object[FieldAdjustBrightEvent.bright] = br;
//    object[FieldAdjustBrightEvent.current_channel] = getIndexFromFields(current_channel).toInt();
//    TcpAssemblerDoc.setObject(object);
//    auto msg = AppendSeparateField(TcpAssemblerDoc.toJson());

    QVariantMap m;
    m[FieldAdjustBrightEvent.bright] = br;
    m[FieldAdjustBrightEvent.current_channel] = getIndexFromFields(current_channel);
    auto msg = assembleAdjustBrightEvent(m);

    SocketPointer->exec_queue(TcpFramePool.adjustBrightEvent,msg); // 不适用同步
}

void Preview::toggleChannel(int option)
{ // 切换通道 帧头: 5
    auto current_channel = channelbox->currentChannel();

    if (current_channel.isEmpty()) {
        LOG<<"not  channel was opened, reject toggle channel";
        Q_ASSERT(option == -1); // 如果是关灯,此时当前通道是空字符串,此时传来的option=-1,那么这个时候就直接关灯即可
        // 不过关灯需要知道关哪个灯,这个工作在closeChannel去做
        return; // 灯全灭的情况直接返回
    }

    auto current_info = camerabox->cameraInfo();

    QVariantMap m;
    m[CurrentChannelField] = getIndexFromFields(current_channel).toInt();
    m[BrightField] = current_info[BrightField].toDouble();
    m[TurnOffLight] = 0;
    /*
     * {
        "current_channel":1
        "frame": 5,
        "bright": 20,
        "turnoff_light":0  0表示开灯
     }
     *
     * */

    AssemblerPointer->assemble(TcpFramePool.toggleChannelEvent,m);
    auto msg = AssemblerPointer->message();

    SocketPointer->exec(TcpFramePool.toggleChannelEvent,msg);

    if (ParserResult.toBool()) {
        LOG<<"[sync] turn on light successful! current channel is"<<m[CurrentChannelField].toInt();
    } else LOG<<"[sync] turn on light failed! current channel is"<<m[CurrentChannelField].toInt();
}

void Preview::closeChannel(int option)
{ // 帧头: 5 关闭灯
    /*
     * {
        "current_channel":1
        "frame": 5,
        "bright": -1, // 关灯时bright参数无意义
        "turnoff_light":1 // 关灯
    }
     * */
    QVariantMap m;
    m[CurrentChannelField] = option;
    m[TurnOffLight] = 1;
    m[BrightField] = -1;
    SocketPointer->exec(TcpFramePool.toggleChannelEvent,assembleToggleChannelEvent(m));
    if (ParserResult.toBool()) {
        switch(option) {
            case 0:LOG<<"[sync] close br channel successful!";
                break;
            case 1:LOG<<"[sync] close ph channel successful!";
                break;
            case 2:LOG<<"[sync] close gfp channel successful!";
                break;
            case 3:LOG<<"[sync] close rfp channel successful!";
                break;
            case 4:LOG<<"[sync] close dapi channel successful!";
                break;
        }

    }
}

void Preview::adjustLens(int option)
{ // 0-left,1-rop,2-right,3-bottom 微调节镜头 帧头: 6
    if (!wellview->isGrouped() || !wellview->isValidMousePoint())
        return; // 如果没分过组或者当前鼠标不是有效的点也不能调节
    stack_view_pattern->setCurrentWidget(wellview);
    QJsonObject object;
    object[FrameField] = AdjustLensEvent;
    object[DirectionField] = option;
    TcpAssemblerDoc.setObject(object);
    auto msg = AppendSeparateField(TcpAssemblerDoc.toJson());

    SocketPointer->exec(TcpFramePool.adjustLensEvent,msg); // 6

    if (ParserResult.toBool()) {
        LOG<<"[sync] adjust len move direction: "<<option<<"successful!";
        wellview->adjustViewPoint(option);
    } else LOG<<"[sync] adjust len move direction: "<<option<<"failed!";
}

void Preview::stopExper()
{ // 帧头: 8
    QVariantMap  m;
    m[FrameField] = TcpFramePool.stopExperEvent;
    m[StopField] = 1;
    AssemblerPointer->assemble(TcpFramePool.stopExperEvent,m);
    auto json = AssemblerPointer->message();

    SocketPointer->exec(TcpFramePool.stopExperEvent,json);
    if (ParserResult.toBool()) {
        //QMessageBox::information(this,InformationChinese,tr("Successfully stop the experiment!"));
        LOG<<"[sync] stop experiment successful!";
        if (!ToupCameraPointer->isOpen()) {
            ToupCameraPointer->openCamera();
        }
        bool enable = true;
        historybox->setEnabled(enable);
        wellbox->setEnabled(enable);
        objectivebox->setEnabled(enable);
        viewModeBox->setEnabled(enable);
        channelbox->setEnabled(enable);
        stack_view_pattern->setEnabled(enable);
        camerabox->setEnabled(enable);
        timebox->setEnabled(enable);
        focusbox->setEnabled(enable);
        recordbox->setEnabled(enable);
        zstackbox->setEnabled(enable);
        savebox->setChildWidgetsEnabled(enable);
    } else LOG<<"[sync] stop experiment failed!";
}

void Preview::adjustObjective(int objective,int objective_loc,int isPh)
{ // 帧头:9, objective: 物镜倍数代号 objective_loc: 切到的物镜位置代号 isPH:指示是否为PH类型
    // 切换物镜同时动电机事件,setting更改物镜设置时也会触发
    // 这个初始化setting时会初始化4个位置的物镜,从而会触发本函数,在mainwindow没构造完成就发了命令
    LOG<<"objective = "<<objective<<" loc ="<<objective_loc<<" isph ="<<isPh;
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

void Preview::adjustFocus(double val)
{ // 帧头11: 手动调焦
    QVariantMap m;
    m[FocusField] = val;
    m[FrameField] = TcpFramePool.manualFocusEvent; // 11
    auto msg = assembleManualFocusEvent(m);
    SocketPointer->exec(TcpFramePool.manualFocusEvent,msg);
    if (ParserResult.toBool()) {
        LOG<<"[sync] manual adjust focus to"<<val<<"successful!";
    } else LOG<<"[sync] manual adjust focus to"<<val<<"failed!";
}

void Preview::autoFocus()
{// 帧头12: 自动调焦
    createPath(AutoFocusTempPath);
    QJsonObject object;
    object[FrameField] = AutoFocusEvent; // "test0x2"
    object[PathField] = "";
    TcpAssemblerDoc.setObject(object);
    auto json = TcpAssemblerDoc.toJson();
    json = AppendSeparateField(json);
    QString path;
    unsigned i = 0;
    double focus = 0.0;
    SocketPointer->setWaitText(WaitMessageBoxWaitFocusMsg);
    SocketPointer->exec(TcpFramePool.autoFocusEvent,json);
    while (i < 10 && ParserResult.value<QPair<bool,double>>().first) {
        path.clear();
        focus = 0.0;
        auto pix = ToupCameraPointer->capture();
        if (!pix.isNull()) {
            path = AutoFocusTempPath
                   +QDateTime::currentDateTime().toString(DefaultImageSaveDateTimeFormat)+JPGSuffix;
            pix.save(path,JPGField,DefaultImageQuality);
        }

        if (i == 0) {
            LOG<<"[sync] ready auto focus";
        } else {
            focus = ParserResult.value<QPair<bool,double>>().second;
            LOG<<"[sync] send"<<path<<" successful, new focus is"<<focus;
            camerabox->updateFocus(focus); // 更新ui的focus显示
        }

        object[PathField] = path;
        TcpAssemblerDoc.setObject(object);
        json = AppendSeparateField(TcpAssemblerDoc.toJson());
        SocketPointer->exec(TcpFramePool.autoFocusEvent,json,i);
        ++i;
    }
    LOG<<"[sync] send"<<path<<".jpg successful,new focus is"<<focus;
    SocketPointer->resetWaitText();
}

void Preview::takingPhoto()
{ // 合成通道事件 帧头: 13
    int exp = camerabox->exposure();
    int ga = camerabox->gain();

    ToupCameraPointer->setExposure(exp);
    ToupCameraPointer->setGain(ga);
    LOG<<"current (exp,gain,bright) is ("<<ToupCameraPointer->exposure()<<ToupCameraPointer->gain()<<camerabox->bright()<<")";

    canvasmode->changeMode(CanvasMode::PhotoMode);
    photocanvas->setStrategy(PhotoCanvas::SinglePixmap);

    static ChannelImageInfo imageinfo;
    createPath(TakePhotoTempPath);
    createPath(MergeChannelTempPath);
    auto pix = ToupCameraPointer->capture();
    auto current_channel = channelbox->currentChannel();
    if (!current_channel.isEmpty() && !pix.isNull()) { // 都没开灯拍的黑图不存储或者没有图
        auto path = TakePhotoTempPath
                    +QDateTime::currentDateTime().toString(DefaultImageSaveDateTimeFormat)+
                    +"_" + current_channel + JPGSuffix;
        pix.save(path,JPGField,DefaultImageQuality);
        imageinfo[current_channel] = path;

        if(imageinfo.values().count() == 1) {
            photocanvas->setImage(pix); // 只有1张不合成
        } else {
            // 发送命令等待回复合成后的路径
            QVariantMap m;
            m[BRPathField] = imageinfo[BRField];
            m[PHPathField] = imageinfo[PHField];
            m[RFPPathField] = imageinfo[RFPField];
            m[GFPPathField] = imageinfo[GFPField];
            m[DAPIPathField] = imageinfo[DAPIField];
            auto msg = assembleChannelMergeEvent(m);
            SocketPointer->exec(TcpFramePool.channelMergeEvent,msg);
            auto merge_path = ParserResult.toString();
            if (!merge_path.isEmpty()) {
                LOG<<"[sync] merge channel image successful! merge_path: "<<merge_path;
                photocanvas->setImage(QImage(merge_path));
            } else {
                LOG<<"[sync] merge channel image failed!";
            }
        }
    }
}




// 其它
// 实时livecanvas图像
void Preview::showCapturedImage(const QImage& image)
{ // 来自照相机的捕捉图像事件传来的live图像显示
#ifdef uselabelcanvas
    static int count = 0;
    if (count % 10 == 0) {
        auto pix = QPixmap::fromImage(image).scaled(livecanvas->size(),Qt::KeepAspectRatio,Qt::FastTransformation);
        livecanvas->setPixmap(pix);
        livecanvas->repaint();
    }
    count++;
    if (count > 100)
        count = 0;
#elif defined(usegraphicscanvas)
    livecanvas->setImage(image,10); // 10张图片显示1次
#else
    livecanvas->setImage(image,10); // 10张图片显示1次
#endif
}

void Preview::importExperConfig(const QString &path)
{
    ConfigReadWrite m; // 借助工具类读取文件
    auto json = m.readJson(path);
    m.parseJson(json);
    auto result = m.map();

    // 1. 导入厂家和品牌信息
    auto brand = result[BrandField].toInt();
    auto manufacturer = result[ManufacturerField].toInt();
    wellbox->importWellInfo(manufacturer,brand);

    // 2. 导入

    // 3. 导入孔板视野信息
    auto groupInfos = result[GroupField].value<QVariantMap>();
    auto patterSize = convertToPointF(result[HoleSizeField].toString());
    auto viewMode = result[ViewModeField].toInt();

    QHoleInfoVector holeInfoVec; // 把所有孔的信息整合成列表
    for(auto var1: groupInfos.values()) {
        auto groupInfo = var1.value<WellGroupInfo>();
        for(auto var2: groupInfo.values()) {
            auto holeInfo = var2.value<HoleInfo>();
            holeInfoVec.append(holeInfo);
        }
    }
    wellpattern->setPatternSize(patterSize.x(),patterSize.y()); // 先设置再导入孔信息否则会越界
    wellpattern->importHoleInfo(holeInfoVec,ViewMode(viewMode));
    wellview->importViewInfo(holeInfoVec,ViewMode(viewMode));
}

void Preview::exportExperConfig(const QString& path)
{ // 导出实验配置
    previewinfo[WellBoxTitle].setValue(wellbox->wellInfo());
    previewinfo[ObjectiveBoxTitle].setValue(objectivebox->objectiveInfo());
    previewinfo[ViewModeBoxTitle].setValue(viewModeBox->viewModeInfo());
    previewinfo[WellPatternField] = wellpattern->patternInfo();
    previewinfo[HoleSizeField] = wellpattern->patternSize();
    //previewinfo[ChannelBoxTitle].setValue(channelbox->channelInfo());
    previewinfo[CameraFocusBoxTitle].setValue(camerabox->cameraFocusInfo());
    previewinfo[TimeBoxTitle].setValue(timebox->timeInfo());
    previewinfo[FocusBoxTitle].setValue(focusbox->focusInfo());
    previewinfo[ZStackBoxField].setValue(zstackbox->zstackInfo());

    auto json = assembleExportExperEvent(previewinfo);
    JsonReadWrite m; // 借助工具类写到文件内
    m.writeJson(path,json);
    // 存数据库,直接存整个json文件是最简单的
}

void Preview::previewSlideEvent(const QPointF& point)
{
    LOG<<"slide point: "<<point;
}

void Preview::stitchSlide()
{ // slide 拼图功能
    auto brand = wellbox->wellInfo()[BrandField].toUInt();
    LOG<<brand<<SlideIndexInBrand;
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
