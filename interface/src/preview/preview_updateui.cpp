/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-31 10:43:52
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-31 10:45:37
 * @FilePath: \EOS\interface\src\preview\preview_updateui.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "preview.h"

void Preview::updateViewWindow(const QVariantMap& m)
{ // 打开选视野的窗口,更新视野的信息(groupname,groupcolor,grouppoints,coordinate+allgroup)+viewsize

    // 1. 相机模式的切换 live/photo => 切换stack,关闭相机等动作
//    cameramode->changeMode(CameraMode::PhotoMode);
//    stack->setCurrentWidget(photocanvas);

    // 2. 更新视野窗口的标题
    auto point = m[HoleCoordinateField].toPoint(); //双击或者右键打开视野窗口带来的孔信息
    auto groupname = m[HoleGroupNameField].toString();
    if (groupname.isEmpty()) groupname = tr("未设置组");
    dock->setWindowTitle(tr("选择孔内视野(%1,%2)-组别(%3)")
                                 .arg(QChar(point.x()+65)).arg(point.y()+1).arg(groupname));
    dock->setFloating(true);

    // 3.根据当前brand/objective更新视野的尺寸
    auto toolinfo = toolbar->toolInfo();
    auto objective = getIndexFromFields(toolinfo[ObjectiveMagnificationField].toString()).toUInt();
    auto brand = toolinfo[BrandField].toUInt();
    auto manufacturer = toolinfo[ManufacturerField].toUInt();
    auto size = ViewCircleMapFields[manufacturer][brand][objective];
    if (size > view_well_6_4x*10)
        dock->setWindowSize(PreviewPhotoCanvasViewDefaultSize*3,PreviewPhotoCanvasViewDefaultSize*3);
    else if (size < view_well_6_4x) dock->setWindowSize(PreviewPhotoCanvasViewDefaultSize,PreviewPhotoCanvasViewDefaultSize);
    else dock->setWindowSize(PreviewPhotoCanvasViewDefaultSize*2,PreviewPhotoCanvasViewDefaultSize*2);
    LOG<<" manufacturer = "<<manufacturer<<" brand = "<<brand<<" objective = "<<objective<<" size = "<<size;

    // 4. 更新视野绘制小正方形行列数
    auto nm = m;
    nm[HoleViewSizeField] = QSize(size,size);

    // 5. 鼠标点击边缘设置无策略或者有效时更新视野窗口策略
    if (point == QPoint(-1,-1))
        viewpattern->setStrategy(ViewPattern::NoStrategy);
    else viewpattern->setStrategy(ViewPattern::InnerCircleRect,nm);//⭐⭐⭐⭐ 把图案的信息传给视野窗口
}

void Preview::updateViewPatternUi()
{
    // 1.更新视野的尺寸
    auto toolinfo = toolbar->toolInfo();
    auto objective = getIndexFromFields(toolinfo[ObjectiveMagnificationField].toString()).toUInt();
    auto brand = toolinfo[BrandField].toUInt();
    auto manufacturer = toolinfo[ManufacturerField].toUInt();
    auto size = ViewCircleMapFields[manufacturer][brand][objective];
    if (size > view_well_6_4x*10)
        dock->setWindowSize(PreviewPhotoCanvasViewDefaultSize*3,PreviewPhotoCanvasViewDefaultSize*3);
    else if (size < view_well_6_4x) dock->setWindowSize(PreviewPhotoCanvasViewDefaultSize,PreviewPhotoCanvasViewDefaultSize);
    else dock->setWindowSize(PreviewPhotoCanvasViewDefaultSize*2,PreviewPhotoCanvasViewDefaultSize*2);

    // 2. 更新视野窗口去更新视野窗口绘制和临时保存信息
    LOG<<manufacturer<<brand<<objective<<size;
    viewpattern->clearAllViewWindowCache(size);

    // 3. 视野窗口的数据信息临时信息需要更改,因为尺寸变了
    dock->setWindowTitle(tr("选择孔内视野"));
    // 4. 视野尺寸/孔板尺寸 变的话所有信息都重新初始化
    pattern->initHoleInfo();
}

void Preview::updateSetGroupWindow(const QVariantMap& m)
{ // 打开分组窗口事件 会把当前孔所属组颜色/组名称(可能设置过)
    // 1. 更新分组窗口的ui信息
//    auto type = m[GroupTypeField].toString();
//    auto color = m[GroupColorField].toString();
//    auto group = m[GroupNameField].toString();
//    auto medicine = m[GroupMedicineField].toString();
//    auto dose = m[GroupDoseField].toString();
//    auto unit = m[GroupDoseUnitField].toString();
//    groupinfo->setGroupType(type);
//    groupinfo->setGroupColor(color);// color可能是设置过的组颜色,没设置过是红色
//    groupinfo->setGroupName(group); // 组名称没设置过就是孔,更新分组窗口的UI信息
//    groupinfo->setGroupMedicine(medicine);
//    groupinfo->setGroupDose(dose);
//    groupinfo->setGroupDoseUnit(unit);
    groupinfo->setGroupInfo(m);

    // 2. 分组窗口设置的组信息去更新孔的数据
    int ret = groupinfo->exec();
    if (ret == QDialog::Accepted) {
        pattern->updateHoleInfoByGroupInfo(groupinfo->groupInfo()); // 用当前的组信息去更新孔的颜色
    }
}
