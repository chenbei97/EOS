/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2024-01-09 17:26:46
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2024-01-10 08:35:28
 * @FilePath: \EOS\interface\src\data\datatable.cpp
 * @Copyright (c) 2024 by ${chenbei}, All Rights Reserved. 
 */

#include "datatable.h"
#include "qrandom.h"

void DataTable::initTableFromDataBase()
{
    mPlateImageInfo = createTestData();
    model->blockSignals(true);
    view->blockSignals(true); // 初始化表时不要触发更新图案,表和图案
    selection->blockSignals(true);
    QStringList rowText;
    for(auto experInfo: mPlateImageInfo) {
        rowText.clear();
        rowText<<experInfo.experName;
        rowText<<(experInfo.isRunning?RunningFieldUpper:CompleteFieldUpper);
        rowText<<experInfo.experTime.toString(DefaultDateTimeFormat);
        rowText<<experInfo.objective;
        model->appendRowTexts(rowText);
        emit experRecordAppended(experInfo);
    };
    model->blockSignals(false);
    view->blockSignals(false);
    selection->blockSignals(false);
//    view->selectRow(0); // 表格行带有颜色
//    clickRow(0); // 触发一下刷新图片展示 // 取消选中否则启动会慢
}

QVector<PlateImageInfo> DataTable::createTestData() const
{
    auto time= QTime::currentTime();
    QRandomGenerator generator(time.msec()+time.second()*1000);
    QVector<PlateImageInfo> infos;
    for(int j = 0; j < 6; ++j) { // 6个实验
        PlateImageInfo info;
        for(int i = 0; i < 10; ++i){ // 不一定有10个孔,可能随机出来有重复的
            ImageInfo image;
            ( j % 2)?
            image.setHoleCoordinate(generator.bounded(0,8),generator.bounded(0,12)):
            image.setHoleCoordinate(generator.bounded(0,4),generator.bounded(0,6));
            auto tmp = generator.bounded(1.0);
            if (tmp>=0.0 && tmp <= 0.3) {
                image.setHoleGroup("A组");
                image.setHoleColor(Qt::yellow);
                image.channel = BRField;
            } else if (tmp > 0.3 && tmp < 0.7) {
                image.setHoleGroup("B组");
                image.setHoleColor(Qt::red);
                image.channel = DAPIField;
            } else {
                image.setHoleGroup("C组");
                image.setHoleColor(Qt::cyan);
                image.channel = GFPField;
            }

            for(int k = 0; k < 5; ++k) { // 每个孔设置5个点,注意如果有孔重复了,这个孔就会得到10个视野而不是5个
                image.setViewCoordinate(generator.bounded(1.0),generator.bounded(1.0));
                for(int m = 0; m < 5; ++m)
                {
                    image.path = CURRENT_PATH+QString("/images/images/%1%2").arg(generator.bounded(1,10)).arg(DataReadImageFormat);
                    image.stamp = QDateTime::currentDateTime().addSecs(generator.bounded(24*60*60));
                    info.images.append(image);
//                    LOG<<"current exper:"<<j<<"current hole:"<<image.getHoleCoordinate()<<"current view:"<<image.getViewCoordinate()
//                        <<"stamp:"<<image.stamp <<"group:"<<image.getHoleGroup();
                }
            }
            //LOG<<QPointF(image.view.x,image.view.y)<<image.stamp;

        }
        if ( j % 2) {
            info.experName = "10倍镜96孔板明场实验";
            info.experTime = QDateTime::currentDateTime();
            info.isRunning = true;
            info.plateSize = QSize(8,12);
            info.objective = "br10x";
        } else {
            info.experName = "20倍镜24孔板相差实验";
            info.experTime = QDateTime::currentDateTime();
            info.isRunning = false;
            info.plateSize = QSize(4,6);
            info.objective = "ph20x";
        }
        infos.append(info);
    }

    return infos;
}

void DataTable::toggleRow(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);
    if (current.isValid()) {
        bool isRunning = isRunningState(current.row());
        auto info = mPlateImageInfo[current.row()];
        emit currentRowChanged(current.row(),isRunning,info);
    }
}

void DataTable::clickRow(int row)
{
    bool isRunning = isRunningState(row);
    auto info = mPlateImageInfo[row];
    emit currentRowClicked(row,isRunning,info);
}

bool DataTable::isRunningState(int row) const
{
    auto s = model->data(row,DataExperimentTableStatusColumn).toString();
    return s == RunningFieldUpper;
}

void DataTable::initConnections()
{
    connect(selection,&QItemSelectionModel::currentRowChanged,this,&DataTable::toggleRow);
    connect(view,&TableView::currentRowClicked,this,&DataTable::clickRow);
}

void DataTable::initAttributes()
{
    model->setRowCount(0);
    model->setColumnCount(DataExperimentTableHeaders.count());
    model->setHorizontalHeaderLabels(DataExperimentTableHeaders);
    view->setModel(model);
    view->setSelectionModel(selection);
    view->setSelectionBehavior(TableView::SelectRows);
}

DataTable::DataTable(QWidget*parent): GroupBox(parent)
{
    view = new TableView;
    model = new StandardItemModel;
    selection = new QItemSelectionModel(model);
    auto lay = new QHBoxLayout;
    lay->addWidget(view);
    lay->setMargin(0);
    lay->setSpacing(0);
    setLayout(lay);
    initAttributes();
    initConnections();
    //initTableFromDataBase();
}



