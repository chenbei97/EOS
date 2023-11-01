/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-01 14:55:19
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-01 14:56:33
 * @FilePath: \EOS\component\include\constant.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_FIELDS_H
#define EOSI_FIELDS_H

#include "alias.h"

static const char* PreviewToolField = "previewtool";
static const char* PreviewPatternField = "previewpattern";

// wellbox涉及的数据类型
static const char* ManufacturerField = "manufacturer";
static const char* BrandField = "brand";
static const char* BrandFieldLabel = "brand: ";
static const char* WellsizeField = "wellsize";
static const char* WellsizeFieldLabel = "wellsize: ";
static const char* WellSize6 = "6";
static const char* WellSize24 = "24";
static const char* WellSize96 = "96";
static const char* WellSize384 = "384";
static const QFieldList ManufacturerFields = {
        "manufacturer1","manufacturer2","manufacturer3","manufacturer4",
};
static const QFieldList WellsizeFields = {
        WellSize6,WellSize24,WellSize96,WellSize384
};
static const QFieldList Brand1Fields = {
        "brand1_6","brand1_24","brand1_96","brand1_384"
};
static const QFieldList Brand2Fields = {
        "brand2_6","brand2_24","brand2_96","brand2_384"
};
static const QFieldList Brand3Fields = {
        "brand3_6","brand3_24","brand3_96","brand3_384"
};
static const QFieldList Brand4Fields = {
        "brand4_6","brand4_24","brand4_96","brand4_384"
};
/*
    6孔板
    4x: 10*10，
    10x: 27*27
    20x: 50*50
    40x: 116*116
 */
#define view_well_6_4x 10
#define view_well_6_10x 27
#define view_well_6_20x 50
#define view_well_6_40x 116

/*  24孔板
    4x: 4*4，
    10x: 14*14
    20x: 20*20
    40x: 45*45
*/
#define view_well_24_4x 4
#define view_well_24_10x 14
#define view_well_24_20x 20
#define view_well_24_40x 45

/*  96孔板
    4x: 2*2，
    10x: 5*5
    20x: 10*10
    40x: 24*24
*/
#define view_well_96_4x 4
#define view_well_96_10x 14
#define view_well_96_20x 20
#define view_well_96_40x 45

/*  384孔板
    4x: 1*1，
    10x: 3*3
    20x: 5*5
    40x: 12*12
*/
#define view_well_384_4x 1
#define view_well_384_10x 3
#define view_well_384_20x 5
#define view_well_384_40x 12

const QVector<QVector<QVector<int>>> ViewCircleMapFields = {
        {//不同的厂家
                {view_well_6_4x,view_well_6_10x,view_well_6_20x,view_well_6_40x},// 品牌1
                {view_well_24_4x,view_well_24_10x,view_well_24_20x,view_well_24_40x},// 品牌2
                {view_well_96_4x,view_well_96_10x,view_well_96_20x,view_well_96_40x},// 品牌3
                {view_well_384_4x,view_well_384_10x,view_well_384_20x,view_well_384_40x},// 品牌4
        },

        {
                {view_well_6_4x,view_well_6_10x,view_well_6_20x,view_well_6_40x},// 品牌1
                {view_well_24_4x,view_well_24_10x,view_well_24_20x,view_well_24_40x},// 品牌2
                {view_well_96_4x,view_well_96_10x,view_well_96_20x,view_well_96_40x},// 品牌3
                {view_well_384_4x,view_well_384_10x,view_well_384_20x,view_well_384_40x},// 品牌4
        },

        {
                {view_well_6_4x,view_well_6_10x,view_well_6_20x,view_well_6_40x},// 品牌1
                {view_well_24_4x,view_well_24_10x,view_well_24_20x,view_well_24_40x},// 品牌2
                {view_well_96_4x,view_well_96_10x,view_well_96_20x,view_well_96_40x},// 品牌3
                {view_well_384_4x,view_well_384_10x,view_well_384_20x,view_well_384_40x},// 品牌4
        },

        {
                {view_well_6_4x,view_well_6_10x,view_well_6_20x,view_well_6_40x},// 品牌1
                {view_well_24_4x,view_well_24_10x,view_well_24_20x,view_well_24_40x},// 品牌2
                {view_well_96_4x,view_well_96_10x,view_well_96_20x,view_well_96_40x},// 品牌3
                {view_well_384_4x,view_well_384_10x,view_well_384_20x,view_well_384_40x},// 品牌4
        },
};

// objective涉及的数据类型
static const char* ObjectiveField = "objective";
static const char* ObjectiveMagnificationField = "objective_magnification";
static const char* ObjectiveTypeField = "objective_type";
static const char* ObjectiveFieldLabel = "objective: ";
static const char* ObjectivePH = "ph";
static const char* ObjectiveBR = "br";
static const char* Objective4x  = "4x";
static const char* Objective10x = "10x";
static const char* Objective20x = "20x";
static const char* Objective40x = "40x";
static const QFieldList ObjectiveTypeFields = {
        ObjectivePH,ObjectiveBR,
};
static const QFieldList ObjectiveMagnificationFields = {
        Objective4x,Objective10x,Objective20x,Objective40x
};

// channel涉及的数据类型
static const char* ChannelField = "channel";
static const char* ChannelFieldLabel = "channel: ";
static const char* BRField = "BR";
static const char* PHField = "PH";
static const char* GFPField = "GFP";
static const char* RFPField = "RFP";
static const char* DAPIField = "DAPI";
static const char* Bright4xField = "br4x";
static const char* PH4xField = "ph4x";
static const char* Bright10xField = "br10x";
static const char* PH10xField = "ph10x";
static const char* Bright20xField = "br20x";
static const char* PH20xField = "ph20x";
static const char* Bright40xField = "br40x";
static const char* PH40xField = "ph40x";
static const char* NoneField = "none";
static const QFieldList ObjectiveSettingFields = {
        Bright4xField,PH4xField,Bright10xField,PH10xField,
        Bright20xField,PH20xField,Bright40xField,PH40xField,
        NoneField
};
static const QFieldList ChannelFields = {
        BRField,PHField,GFPField,RFPField,DAPIField
};

// camerabox涉及的
#define ExposureLowLimit 0
#define ExposureUpLimit 15000
#define GainLowLimit 100
#define GainUpLimit 5000
#define BrightLowLimit 0
#define BrightUpLimit 100
#define FocusToolFocusMaxVal 50000
#define FocusToolStepMaxVal 25000
static const char* ExposureField = "exposure";
static const char* ExposureFieldLabel = "exposure: ";
static const char* ExposureFieldUnit = "ms";
static const char* GainField = "gain";
static const char* GainFieldLabel = "gain: ";
static const char* GainFieldUnit = "%";
static const char* BrightField = "bright";
static const char* BrightFieldLabel = "bright: ";
static const char* FocusToolFocusLabel = "focus: ";
static const char* FocusToolStepLabel = "step: ";
static const char* FocusField = "focus";
static const char* FocusStepField = "focusstep";
static const char* CameraLocationField = "camera_loc";
static const char* CameraLocationField1 = "loc_1";
static const char* CameraLocationField2 = "loc_2";
static const char* CameraLocationField3 = "loc_3";
static const char* CameraLocationField4 = "loc_4";
static const QFieldList CameraLocationFieldFields = {
        CameraLocationField1,CameraLocationField2,CameraLocationField3,CameraLocationField4
};
#define CameraLocationField1Index  CameraLocationFieldFields.indexOf(CameraLocationField1)
#define CameraLocationField2Index  CameraLocationFieldFields.indexOf(CameraLocationField2)
#define CameraLocationField3Index  CameraLocationFieldFields.indexOf(CameraLocationField3)
#define CameraLocationField4Index  CameraLocationFieldFields.indexOf(CameraLocationField4)

// timebox涉及的
#define TimeBoxPhotoTimeLimit 1800
#define TimeBoxPhotoTimeWaring (QObject::tr("警告:实验间隔时间不能低于%1分钟!").arg(TimeBoxPhotoTimeLimit/60))
#define TimeBoxTotalBeyondDurationWaring (QObject::tr("警告:实验时长至少大于等于一个实验间隔!"))
static const char* IsScheduleField = "is_schedule";
static const char* TotalTimeField = "total_time";
static const char* DurationTimeField = "duration_time";
static const char* StartTimeField = "start_time";
static const char* ExperChannelField = "exper_channel";

// groupinfo涉及的
static const char* GroupTypeField = "grouptype";
static const char* GroupNameField = "groupname";
static const char* GroupColorField = "groupcolor";
static const char* GroupPointsField = "grouppoints";
static const char* GroupMedicineField = "groupmedicine";
static const char* GroupDoseField = "groupdose";
static const char* GroupDoseUnitField = "groupdoseunit";
static const char* AllGroupsField = "allgroups";
static const char* CoordinateField = "coordinate";
static const char* ViewSizeField = "viewsize";
static const char* ViewPointsField = "viewpoints";
static const QStringList GroupDoseUnitFields =  {
        "mg/mL","mg/L","g/mL","g/L",
};
static const QStringList GroupTypeFields = {
        QObject::tr("控制组"),QObject::tr("对照组"),QObject::tr("对照3"),
};

// stackbox
static const char* ZStackField = "zstack";
static const char* StitchField = "stitch";

// photocanvas
static const char* LiveField = "live";
static const char* PhotoField = "photo";
#define LiveFieldMetricWidth DefaultPainterMetric.width(LiveField)
#define PhotoFieldMetricWidth DefaultPainterMetric.width(PhotoField)
#define PreviewPhotoCanvasViewRectColor QColor("#00A2E8")

// appselect + canvas
static const char* AppSelectField = "appselect";
#define AppSelectColor QColor("#2F7CAD")
#define NavigBarFieldsCount  (NavigBarFields.count())
static const QFieldList AppFields = {
        QObject::tr("增殖"),QObject::tr("划痕"),QObject::tr("侵袭"),QObject::tr("转染"),
        QObject::tr("形态学"),QObject::tr("类器官"),QObject::tr("药效和毒理"),QObject::tr("自定义")
};
static const QFieldList NavigBarFields = {
        "Logo","Main","Preview","Data","Analysis","Settings"
};


// 需要手动转换成idx的字段
static const QList<QFieldList> SocketNeedMapFields{
        Brand1Fields,Brand2Fields,Brand3Fields,Brand4Fields,
        ChannelFields,ManufacturerFields,ObjectiveMagnificationFields,
        CameraLocationFieldFields,AppFields
};

static QString getIndexFromFields (QCString field)
{ // 用于把这些字段统一映射为0,1,2,3方便下位机读取
    // 例如4x,jpg,bright,avi都应该映射为0
    QString idx = "";
    foreach(auto fields, SocketNeedMapFields) { // 仅限于从这些字段查找
        auto index = fields.indexOf(field);
        if (index>=0){
            idx = QString::number(index);
            break; // 找到
        }
    }
    return idx;
}

#endif //EOSI_FIELDS_H
