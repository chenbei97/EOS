/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-12-04 13:48:06
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-26 13:34:53
 * @FilePath: \EOS\interface\src\preview\viewmode.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "viewmode.h"

ViewModeBox::ViewModeBox(QWidget *parent) : GroupBox(parent)
{
    initObjects();
    initAttributes();
    initLayout();

    setTitle(tr(ViewModeBoxTitle));
    connect(groupMode,&RadioGroup::radioClicked,this, &ViewModeBox::radioClick);
    connect(triangleMove,&TriangleMove::triangleClicked,this,&ViewModeBox::triangleClick);
    connect(enableGroupBtn,&PushButton::clicked,this,&ViewModeBox::enableGroup);
    connect(endGroupBtn,&PushButton::clicked,this,&ViewModeBox::endGroup);
    connect(groupType,&ComboBox::currentTextChanged,this,&ViewModeBox::toggleGroup);
    connect(enableSelectBtn,&PushButton::clicked,this,&ViewModeBox::enableSelect);
}

void ViewModeBox::enableSelect()
{
    auto btn = qobject_cast<PushButton*>(sender());
    if (btn->isChecked()) {
        btn->resetBackGroundColor();
    } else {
        btn->setBackGroundColor(Qt::yellow);
    }
    if (btn->isEnabled()) {
        btn->setText(tr("location select[enable]"));
    } else {
        btn->setText(tr("location select[disable]"));
    }

    if (groupMode->checkedID() == 1){ // 点模式下,三角的使能和location_select按钮的checked状态有关
        triangleMove->setEnabled(btn->isChecked());
    }
    // 无论是哪种模式,能否启用wellview的鼠标绘图事件只和enableSelectBtn的checked状态有关
    emit viewEnableChanged(btn->isChecked());
}

void ViewModeBox::enableGroup()
{ // 使能分组
    enableGroupBtn->setBackGroundColor(Qt::yellow);// 高亮可以分组
    endGroupBtn->resetBackGroundColor(); // 结束分组取消高亮
    groupType->setEnabled(true); // 组的下拉框可选
    endGroupBtn->setEnabled(true); // 结束分组使能
    enableSelectBtn->setEnabled(false);// 分组时不可以选择视野
    enableSelectBtn->setText(tr("location select[disable]"));// 分组时按钮文本变化
    if (groupMode->checkedID() == 1 && enableSelectBtn->isChecked()
        && enableSelectBtn->isEnabled()){ // 点模式+视野选择按钮使能且checked
        triangleMove->setEnabled(true); // 才可以移动4个三角
    } else triangleMove->setEnabled(false);

    emit enableWellPattern(true); // 使能分组后控制wellPattern的使能
    emit viewEnableChanged(false); // 但是禁用鼠标/绘图事件
}

void ViewModeBox::endGroup()
{ // 结束分组
    endGroupBtn->setBackGroundColor(Qt::yellow); // 结束分组高亮
    enableGroupBtn->resetBackGroundColor(); // 可以分组取消高亮
    groupType->setEnabled(false); // 组下拉框不可选
    enableSelectBtn->setEnabled(true); // 可以启用选择视野
    enableSelectBtn->setText(tr("location select[enable]")); // 修改按钮文本
    //emit enableWellPattern(false); // 不要控制图案的使能禁止,因为还要继续选点
    //emit viewEnableChanged(true); // 启用鼠标/绘图事件不由endGroup决定,而是enableSelect
}

void ViewModeBox::radioClick(int mode)
{ // 切换选视野模式,可能之前都是使能,切到非点模式后要进行调整
    if (mode == 1 && enableSelectBtn->isEnabled()) { // 切到点模式时,且选择视野使能状态
            triangleMove->setEnabled(enableSelectBtn->isChecked());//如果当前是checked那么三角可用否则不可用
            // 这种情况是区域模式切到三角时可能enableSelectBtn已经高亮,三角应该跟随
    } else { // 非点模式不允许三角,
        //enableSelectBtn->resetBackGroundColor(); // 不需要帮助取消高亮和置unchecked,区域模式也可以看视野
        triangleMove->setEnabled(false);
    }
    emit viewEnableChanged(enableSelectBtn->isChecked());//需要刷新进入视野动作菜单的状态
    emit modeSelected(mode);
}

void ViewModeBox::triangleClick(int option)
{ // 点击4个三角,这是以防万一的一层保障
    if (groupMode->checkedID() == 1 && enableSelectBtn->isEnabled()
        && enableSelectBtn->isChecked()) {
        emit triangleClicked(option); // 点模式且启用视野后可以
    }
}

void ViewModeBox::toggleGroup(const QString &text)
{
    auto idx = groupType->currentIndex();
    auto color = groupType->itemData(idx,Qt::BackgroundRole).toString();
    // 把当前组的类型和组的颜色传递给groupinfo
    emit groupTypeChanged(text);
    emit groupColorChanged(QColor(color));
    groupType->setStyleSheet(tr("QComboBox:!editable{background:%1}").arg(color));//让下拉框当前项跟随颜色
}

void ViewModeBox::updateGroupItemIcon(const QString &group)
{ // groupInfo点击确定后,更新这里的下拉栏图标,表示不能再选了
    auto cmodel = static_cast<QStandardItemModel*>(groupType->model());
    for(int i = 0; i < cmodel->rowCount(); ++i) {
        auto item = cmodel->item(i);
        if (item->text() == group) {
            cmodel->item(i)->setIcon(QIcon(":/images/radiobutton_unchecked.png"));
            item->setEnabled(false);
            break;
        }
    }
}

void ViewModeBox::resetGroupItemIcon(const QSet<QString> groups)
{ // 每次删孔都要检测是否某个组的孔删完了,如果删完了groups就没有那个组,对应的图标恢复
    LOG<<groups;
    auto cmodel = static_cast<QStandardItemModel*>(groupType->model());
    for(int i = 0; i < cmodel->rowCount(); ++i) {
        auto item = cmodel->item(i);
        if (groups.contains(item->text())) {
            cmodel->item(i)->setIcon(QIcon(":/images/radiobutton_unchecked.png"));
            cmodel->item(i)->setEnabled(false); // 不能选的禁用
        } else {
            cmodel->item(i)->setIcon(QIcon(":/images/radiobutton_checked.png"));
            cmodel->item(i)->setEnabled(true);
        }
    }
}

void ViewModeBox::resetAllGroupItemIcon()
{ // 切换到slide时需要重置所有组的下拉框选项恢复使能
    auto cmodel = static_cast<QStandardItemModel*>(groupType->model());
    Q_ASSERT(cmodel->rowCount() <= GroupTypeColors.count());
    for(int i = 0; i < cmodel->rowCount(); ++i) {
        cmodel->item(i)->setBackground(GroupTypeColors[i]);
        cmodel->item(i)->setData(GroupTypeColors[i],Qt::BackgroundRole);
        cmodel->item(i)->setIcon(QIcon(":/images/radiobutton_checked.png"));
        cmodel->item(i)->setEnabled(true);
    }
}

void ViewModeBox::initAttributes()
{
    groupMode->setText(QStringList()<<tr(PointModeField)<<tr(AreaModeField)<<tr(WholeModeField));
    triangleMove->setTriangleGap(0);
    triangleMove->setEnabled(false);
    groupType->setEnabled(false);
    groupType->setCurrentIndex(0);
    enableSelectBtn->setEnabled(false);
    endGroupBtn->setEnabled(false);
    auto cmodel = static_cast<QStandardItemModel*>(groupType->model());
    Q_ASSERT(cmodel->rowCount() <= GroupTypeColors.count());
    for(int i = 0; i < cmodel->rowCount(); ++i) {
        cmodel->item(i)->setBackground(GroupTypeColors[i]);
        cmodel->item(i)->setData(GroupTypeColors[i],Qt::BackgroundRole);
        cmodel->item(i)->setIcon(QIcon(":/images/radiobutton_checked.png"));
    }
}

void ViewModeBox::initLayout()
{
    // 1. leftbox
    auto leftbox = new GroupBox;
    auto leftlay = new QHBoxLayout;
    leftlay->addWidget(groupMode);
    leftbox->setLayout(leftlay);

    // 2. rightbox
    auto rightbox = new GroupBox;
    auto rightlay = new QVBoxLayout;
    rightlay->addWidget(enableGroupBtn);
    rightlay->addWidget(groupType);
    rightlay->addWidget(endGroupBtn);
    rightlay->addWidget(enableSelectBtn);
    rightbox->setLayout(rightlay);

    // 3. mainlay
    auto lay = new QHBoxLayout;
    lay->addWidget(leftbox);
    lay->addWidget(rightbox);
    lay->addWidget(triangleMove);
    lay->setSpacing(10);
    //lay->addStretch();
    setLayout(lay);
}

void ViewModeBox::initObjects()
{
    groupMode = new RadioGroup(3,Qt::Vertical);
    triangleMove = new TriangleMove;
    groupType = new ComboBox(GroupTypeFields);
    enableGroupBtn = new PushButton(tr("create group"));
    endGroupBtn = new PushButton(tr("group done"));
    enableSelectBtn = new PushButton(tr("location select"));
}

void ViewModeBox::setViewEnabled(int option)
{ // 0表示孔板类型,1表示载玻片类型 来自wellTypeChanged
    groupMode->setEnabled(1,!option);
    groupMode->setEnabled(2,true); // 区域模式任何时候都可用
    groupMode->setEnabled(3,!option);
    if (option) {
        groupMode->setChecked(2,true); // 如果是载玻片,区域模式的checked要设置上
        groupMode->emitSignals(2);// 这样才能触发信号

        enableGroupBtn->setEnabled(false); // 禁用分组
        groupType->setEnabled(false); // 禁用组下拉框
        endGroupBtn->setEnabled(false); // 禁用结束分组
        enableSelectBtn->setEnabled(false);// 禁用选择视野
        enableSelectBtn->setText(tr("location select[disable]"));// 分组时按钮文本变化
        // areaMode已经控制了三角禁用无需再控制
    } else {
        enableGroupBtn->setEnabled(true); // 启用分组
        enableGroupBtn->resetBackGroundColor(); // 颜色可能之前是高亮先恢复高亮
        groupType->setEnabled(false); // 启用分组去控制,初始false
        endGroupBtn->setEnabled(true); // 启用结束分组
        endGroupBtn->resetBackGroundColor(); // 同理先恢复高亮
    }
}

ViewMode ViewModeBox::viewMode() const
{
    ViewMode mode;
    auto checkID = groupMode->checkedID();
    checkID--;
    mode = ViewMode(checkID);
    return mode;
}

ViewModeInfo ViewModeBox::viewModeInfo() const
{
    ViewModeInfo info;
    switch (viewMode()) {
        case ViewMode::PointMode:
            info[ViewModeField] = QString::number(0);
            break;
        case ViewMode::RectMode:
            info[ViewModeField] = QString::number(1);
            break;
        case ViewMode::WholeMode:
            info[ViewModeField] = QString::number(2);
            break;
    }
    info[CurrentGroupField] = groupType->currentText();
    return info;
}

QString ViewModeBox::currentGroup() const
{
    return groupType->currentText();
}