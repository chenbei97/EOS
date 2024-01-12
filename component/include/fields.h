/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-01 14:55:19
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-26 13:34:41
 * @FilePath: \EOS\component\include\fields.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_FIELDS_H
#define EOSI_FIELDS_H

#include "alias.h"

// setting涉及的
static const char* loc1_ = "[loc1]";
static const char* loc2_ = "[loc2]";
static const char* loc3_ = "[loc3]";
static const char* loc4_ = "[loc4]";

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
        "brand1_6","brand1_24","brand1_96","brand1_384","slide"
};
static const QFieldList Brand2Fields = {
        "brand2_6","brand2_24","brand2_96","brand2_384","slide"
};
static const QFieldList Brand3Fields = {
        "brand3_6","brand3_24","brand3_96","brand3_384","slide"
};
static const QFieldList Brand4Fields = {
        "brand4_6","brand4_24","brand4_96","brand4_384","slide"
};
static const int SlideIndexInBrand = 4;
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
#define view_well_96_4x 2
#define view_well_96_10x 5
#define view_well_96_20x 10
#define view_well_96_40x 24

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

/* Slide载玻片
    4x: *，
    10x: *
    20x: *
    40x: *
 * */
#define view_slide_4x 3
#define view_slide_10x 5
#define view_slide_20x 7
#define view_slide_40x 10

const QVector<QVector<QVector<int>>> ViewCircleMapFields = {
        {//不同的厂家
                {view_well_6_4x,view_well_6_10x,view_well_6_20x,view_well_6_40x},// 品牌1
                {view_well_24_4x,view_well_24_10x,view_well_24_20x,view_well_24_40x},// 品牌2
                {view_well_96_4x,view_well_96_10x,view_well_96_20x,view_well_96_40x},// 品牌3
                {view_well_384_4x,view_well_384_10x,view_well_384_20x,view_well_384_40x},// 品牌4
                {view_slide_4x,view_slide_10x,view_slide_20x,view_slide_40x},// slide
        },

        {
                {view_well_6_4x,view_well_6_10x,view_well_6_20x,view_well_6_40x},// 品牌1
                {view_well_24_4x,view_well_24_10x,view_well_24_20x,view_well_24_40x},// 品牌2
                {view_well_96_4x,view_well_96_10x,view_well_96_20x,view_well_96_40x},// 品牌3
                {view_well_384_4x,view_well_384_10x,view_well_384_20x,view_well_384_40x},// 品牌4
                {view_slide_4x,view_slide_10x,view_slide_20x,view_slide_40x},// slide
        },

        {
                {view_well_6_4x,view_well_6_10x,view_well_6_20x,view_well_6_40x},// 品牌1
                {view_well_24_4x,view_well_24_10x,view_well_24_20x,view_well_24_40x},// 品牌2
                {view_well_96_4x,view_well_96_10x,view_well_96_20x,view_well_96_40x},// 品牌3
                {view_well_384_4x,view_well_384_10x,view_well_384_20x,view_well_384_40x},// 品牌4
                {view_slide_4x,view_slide_10x,view_slide_20x,view_slide_40x},// slide
        },

        {
                {view_well_6_4x,view_well_6_10x,view_well_6_20x,view_well_6_40x},// 品牌1
                {view_well_24_4x,view_well_24_10x,view_well_24_20x,view_well_24_40x},// 品牌2
                {view_well_96_4x,view_well_96_10x,view_well_96_20x,view_well_96_40x},// 品牌3
                {view_well_384_4x,view_well_384_10x,view_well_384_20x,view_well_384_40x},// 品牌4
                {view_slide_4x,view_slide_10x,view_slide_20x,view_slide_40x},// slide
        },
};

// objective涉及的数据类型
static const char* ObjectiveField = "objective";
static const char* ObjectiveDescripField = "objective_descrip";
static const char* ObjectiveTypeField = "objective_type";
static const char* ObjectiveFieldLabel = "objective: ";
static const char* IsPHField = "isPH";
static const char* ObjectivePH = "ph";
static const char* ObjectiveBR = "br";
static const char* Objective4x  = "4x";
static const char* Objective10x = "10x";
static const char* Objective20x = "20x";
static const char* Objective40x = "40x";
static const char* ObjectiveLabel4x  = "4x: ";
static const char* ObjectiveLabel10x = "10x: ";
static const char* ObjectiveLabel20x = "20x: ";
static const char* ObjectiveLabel40x = "40x: ";
static const QFieldList ObjectiveTypeFields = {
        ObjectiveBR,ObjectivePH
};
static const QFieldList ObjectiveMagnificationFields = {
        Objective4x,Objective10x,Objective20x,Objective40x
};

// channel涉及的数据类型
static const char* ChannelField = "channel";
static const char* ChannelFieldUpper = "Channel";
static const char* ChannelFieldLabel = "channel: ";
static const char* CurrentChannelField = "current_channel";
static const char* SaveChannelsField = "save_channels";
static const char* CaptureField = "capture";
static const char* BRPathField = "br_path";
static const char* PHPathField = "ph_path";
static const char* RFPPathField = "rfp_path";
static const char* GFPPathField = "gfp_path";
static const char* DAPIPathField = "dapi_path";
static const char* MergePathField = "merge_path";
static const char* BRField = "BR";
static const char* PHField = "PH";
static const char* GFPField = "GFP";
static const char* RFPField = "RFP";
static const char* DAPIField = "DAPI";
static const char* _4xPHField = "4xPH";
static const char* _10xPHField = "10xPH";
static const char* _20xPHField = "20xPH";
static const char* _40xPHField = "40xPH";
static const char* NA20x05Field = "NA20x0.5";
static const char* NA20x08Field = "NA20x0.8";
static const char* NA40x095Field = "NA40x0.95";
static const char* NAField = "NA";
static const char* NoneField = "none";
static const QFieldList ObjectiveSettingFields = {
        Objective4x,_4xPHField,Objective10x,_10xPHField,
        Objective20x,_20xPHField,NA20x05Field,NA20x08Field,
        Objective40x,_40xPHField,NA40x095Field,NoneField
};
static const QFieldList ChannelFields = {
        BRField,PHField,GFPField,RFPField,DAPIField
};
// 其实还跟不同孔板规格有关系: 6,24,96,384孔板对于2个NA型号共8种设置应该,现在先这样简写
static const QPointVector NA20x05DisablePoints = { // 对于NA0.5-20倍镜需要限制不能选择的视野坐标
        QPoint(0,0),QPoint(0,1),QPoint(0,2),QPoint(0,3),
};
static const QPointVector NA20x08DisablePoints = { // 对于NA0.8-20倍镜需要限制不能选择的视野坐标
        QPoint(0,0),QPoint(0,1),QPoint(0,2),QPoint(0,3),
};

// camerabox涉及的
#define ExposureLowLimit 0
#define ExposureUpLimit 15000
#define GainLowLimit 100
#define GainUpLimit 5000
#define BrightLowLimit 0
#define BrightUpLimit 100
#define FocusToolFocusMaxVal 10
#define FocusToolStepMaxVal 10
#define FocusToolStepVal 0.001
#define FocusToolScaleVal 1000 // 是FocusToolStepVal的倒数
#define FocusToolStepDecimal 3
static const char* ObjectiveLocationField = "objective_loc";
static const char* CameraChannelField = "camera_channel";
static const char* ExposureField = "exposure";
static const char* CurrentExposureField = "current_exposure";
static const char* ExposureFieldLabel = "exp: ";
static const char* ExposureFieldUnit = "ms";
static const char* GainField = "gain";
static const char* CurrentGainField = "current_gain";
static const char* GainFieldLabel = "gain: ";
static const char* GainFieldUnit = "%";
static const char* BrightField = "bright";
static const char* CurrentBrightField = "current_bright";
static const char* BrightFieldLabel = "bright: ";
static const char* FocusToolFocusLabel = "focus: ";
static const char* FocusToolStepLabel = "step: ";
static const char* FocusField = "focus";
static const char* FocusxField = "Focus";
static const char* StepField = "step";
static const char* IsCoarseField = "isCoarse";
static const char* SkipHolesField = "skip holes";
static const char* SkipViewsField = "skip views";
static const char* FocusSkipHolesLabelField = "skip holes: ";
static const char* FocusSkipViewsLabelField = "skip views: ";
static const char* FocusExtendRangeLabelField = "extend range?";
static const char* ExtendRangeField = "extend range";
static const char* AllCycleField = "all cycle";
static const char* FocusAllCycleField = "all cycle?";
static const char* FocusAllChannelsField = "all channels?";
static const char* FocusAutoFocusLabelField = "auto focus?";
static const char* FocusStepField = "focus_step";
static const char* ObjectiveLocationField1 = "objective1";
static const char* ObjectiveLocationField2 = "objective2";
static const char* ObjectiveLocationField3 = "objective3";
static const char* ObjectiveLocationField4 = "objective4";
static const QFieldList ObjectiveLocationFields = {
        ObjectiveLocationField1,ObjectiveLocationField2,ObjectiveLocationField3,ObjectiveLocationField4
};
#define ObjectiveLocationField1Index  ObjectiveLocationFields.indexOf(ObjectiveLocationField1)
#define ObjectiveLocationField2Index  ObjectiveLocationFields.indexOf(ObjectiveLocationField2)
#define ObjectiveLocationField3Index  ObjectiveLocationFields.indexOf(ObjectiveLocationField3)
#define ObjectiveLocationField4Index  ObjectiveLocationFields.indexOf(ObjectiveLocationField4)

// timebox涉及的
#define TimeBoxPhotoTimeLimit 1800
#define TimeBoxPhotoTimeWarning (QObject::tr("Warning:duration should be greater than or equal to %1 minutes!").arg(TimeBoxPhotoTimeLimit/60))
#define TimeBoxTotalBeyondDurationWarning (QObject::tr("Warning:total should be greater than or equal to duration!"))
#define TimeBoxStartTimeWarning (QObject::tr("start time cannot be in the past!"))
static const char* IsScheduleField = "is_schedule";
static const char* IsScheduleLabelField = "is schedule? ";
static const char* TotalTimeField = "total_time";
static const char* TotalTimeLabelField = "total time: ";
static const char* DurationTimeField = "duration_time";
static const char* DurationTimeLabelField = "duration time: ";
static const char* StartTimeField = "start_time";
static const char* StartTimeLabelField = "start time: ";
static const char* ChannelOptionLabelField = "channel option: ";

// groupinfo涉及的
static const char* HoleGroupNameField = "hole_groupname"; // 本孔所属组名称
static const char* HoleGroupColorField = "hole_groupcolor"; // 本孔所属组颜色
static const char* HoleGroupCoordinatesField = "hole_groupcoordinates"; // 本孔所属组的所有孔坐标
static const char* HoleCoordinateField = "hole_coordinate"; // 孔的坐标
static const char* ViewCoordinateField = "view_coordinate"; // 视野的坐标
static const char* ViewXCoordinateField = "view_x";
static const char* ViewYCoordinateField = "view_y";
static const char* HoleXCoordinateField = "hole_x";
static const char* HoleYCoordinateField = "hole_y";
static const char* ViewSizeField = "viewsize";
static const char* HoleSizeField = "holesize";
static const char* HoleMedicineField = "hole_medicine"; // 本孔的药物信息
static const char* HoleDoseField = "hole_dose"; // 本孔的剂量信息
static const char* HoleDoseUnitField = "hole_doseunit"; // 本孔的剂量单位信息
static const char* HoleViewSizeField = "hole_viewsize"; // 孔的视野尺寸
static const char* HoleViewRectsField = "hole_viewrects";// 孔选择的视野区域
static const char* HoleViewUiPointsField = "hole_uiviewpoints"; // 孔选择的视野坐标
static const char* HoleViewPointsField = "hole_viewpoints"; // 孔选择的电机坐标,更精细
static const char* HoleAllGroupsField = "hole_allgroups"; // 孔板已分过的所有组名称
static const char* HoleAllCoordinatesField = "hole_allcoordinates"; // 孔板已选过的所有孔坐标
static const char* IsHoleField = "ishole"; // 是孔坐标触发还是视野坐标触发

static const QStringList GroupDoseUnitFields =  {
        "mg/mL","mg/L","g/mL","g/L","μM",
};
static const QStringList GroupTypeFields = {
        QObject::tr("Positive Control 1"),QObject::tr("Negative Control 1"),QObject::tr("Treatment 1"),
        QObject::tr("Positive Control 2"),QObject::tr("Negative Control 2"),QObject::tr("Treatment 2"),
        QObject::tr("Positive Control 3"),QObject::tr("Negative Control 3"),QObject::tr("Treatment 3"),
        QObject::tr("Positive Control 4"),QObject::tr("Negative Control 4"),QObject::tr("Treatment 4"),
};

// stackbox
static const char* ZStackField = "zstack";
static const char* StitchField = "stitch";

// recordbox
static const char* ImageFormatField = "image_format";
static const char* VideoFormatField = "video_format";
static const char* VideoFrameRateField = "video_framerate";
static const char* VideoSavePathField = "video_savepath";
static const char* ImageFormatLabelField = "image format: ";
static const char* VideoFormatLabelField = "video format: ";
static const char* VideoFrameRateLabelField = "video framerate: ";
static const QFieldList ImageFormatFields = {
        "jpg","jpeg","png","ico","bmp","ttf"
};
static const QFieldList VideoFormatFields = {
        "avi","wmv","mp4"
};
static const QFieldList VideoFrameRateFields = {
        "10","20","30","40"
};

// ViewSelectBox涉及的
static const char* ViewModeField = "viewmode";
static const char* CurrentGroupField = "currentgroup";

// photocanvas
static const char* LiveField = "live";
static const char* PhotoField = "photo";
static const char* VideoField = "video";
#define LiveFieldMetricWidth DefaultPainterMetric.width(LiveField)
#define PhotoFieldMetricWidth DefaultPainterMetric.width(PhotoField)
#define VideoFieldMetricWidth DefaultPainterMetric.width(VideoField)
#define PreviewPhotoCanvasViewRectColor QColor("#00A2E8")

// appselect + canvas
static const char* AppSelectField = "appselect";
#define AppSelectColor QColor("#2F7CAD")
#define NavigBarFieldsCount  (NavigBarFields.count())
static const QFieldList AppFields = {
        QObject::tr("Proliferation"),QObject::tr("Scratch"),QObject::tr("Invasion"),QObject::tr("Transfection"),
        QObject::tr("Morphology"),QObject::tr("Organoid"),QObject::tr("Pharmacodynamics"),QObject::tr("Custom")
};

enum class AppType {
    Proliferation = 0 ,
    Scratch = 1,
    Invasion = 2,
    Transfection = 3,
    Morphology = 4,
    Organoid = 5,
    Pharmacodynamics = 6,
    Custom = 7,
};
enum class LenMoveDirectionType {
    Left = 0,
    Top = 1,
    Right = 2,
    Bottom = 3
};

static const QFieldList NavigBarFields = {
        "Logo","Application","Preview","Data","Analysis","Settings"
};

//wellpattern涉及的
static const char* GroupField = "group";
static const char* GroupFieldUpper = "Group";
static const char* XField = "x";
static const char* YField = "y";
static const char* PointsField = "points";
static const char* PointField = "point";

// sql数据库存储涉及的
static const char * LoginTableName = "logininfo";
static const char* ExperTableName = "experinfo";
static const char* WellInfoField = "wellinfo"; // ManufacturerField,BrandField,WellsizeField
static const char* ObjectiveInfoField = "wellinfo";// CameraLocationField,ObjectiveField
static const char* ExperInfoField = "experinfo";// IsScheduleField,TotalTimeField,DurationTimeField,StartTimeField
static const char* ChannelInfoField = "channelinfo";// BRField,PHField,RFPField,GFPField,DAPIField,ChannelField
static const char* FocusInfoField = "focusinfo";//FocusField,FocusStepField
static const char* GroupInfoField = "groupinfo"; // 1个组有哪些孔,选了哪些视野,孔信息等这些信息
static const char* OtherInfoField = "otherinfo";//ZStackField,StitchField

// 需要手动转换成idx的字段
static const QList<QFieldList> SocketNeedMapFields{
        Brand1Fields,Brand2Fields,Brand3Fields,Brand4Fields,
        ChannelFields,ManufacturerFields,ObjectiveMagnificationFields,ObjectiveTypeFields,
        ObjectiveLocationFields,AppFields,WellsizeFields,
        VideoFormatFields,ImageFormatFields,
};
// #include "qdebug.h"
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
