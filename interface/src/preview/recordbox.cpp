
#include "recordbox.h"

RecordBox::RecordBox(QWidget *parent) : GroupBox(parent)
{
    startrecord = new PushButton(tr("start"));
    stoprecord = new PushButton(tr("stop"));
    playvideo = new PushButton(tr("play"));
    stopvideo = new PushButton(tr("stop"));
    pausevideo  = new PushButton(tr("pause"));
    selectvideo  = new Label;
    videoformat = new ComboBox(VideoFormatFields);
    imageformat = new ComboBox(ImageFormatFields);
    framerate = new ComboBox(VideoFrameRateFields);
    savepath = new Label;

    savepath->setFrameShape(Label::Box);
    selectvideo->setFrameShape(Label::Box);
//    savepath->setWordWrap(true);
//    selectvideo->setWordWrap(true);
    savepath->setElidedText(getAppStandardDirPath());
    selectvideo->setElidedText(tr("select video"));

    auto hlay1 = new QHBoxLayout;
    hlay1->addWidget(startrecord);
    hlay1->addWidget(stoprecord);
    hlay1->addWidget(savepath);
    hlay1->addStretch();

    auto hlay2 = new QHBoxLayout;
    hlay2->addWidget(playvideo);
    hlay2->addWidget(pausevideo);
    hlay2->addWidget(stopvideo);
    hlay2->addWidget(selectvideo);
    hlay2->addStretch();


    auto formlay = new QFormLayout;
    formlay->addRow(tr("video record: "),hlay1);
    formlay->addRow(tr("video play: "),hlay2);
    formlay->addRow(tr(ImageFormatLabelField),imageformat);
    formlay->addRow(tr(VideoFormatLabelField),videoformat);
    formlay->addRow(tr(VideoFrameRateLabelField),framerate);


    connect(startrecord,&PushButton::clicked,this,&RecordBox::startRecord);
    connect(stoprecord,&PushButton::clicked,this,&RecordBox::stopRecord);
    connect(savepath,&Label::mouseClicked,this,&RecordBox::changeSavePath);


    connect(playvideo,&PushButton::clicked,[this]{ if(!selectvideo->elidedText().isEmpty()) emit playVideo(selectvideo->elidedText());});
    connect(stopvideo,&PushButton::clicked,this,&RecordBox::stopVideo);
    connect(pausevideo,&PushButton::clicked,this,&RecordBox::pauseVideo);
    connect(selectvideo,&Label::mouseClicked,this,&RecordBox::selectVideo);

    setLayout(formlay);
    setTitle(tr("Record"));
}

void RecordBox::startRecord()
{

}

void RecordBox::recordImage(const QImage &img)
{ // 相机传过来的原始无缩放图像

}

void RecordBox::stopRecord()
{

}

void RecordBox::selectVideo()
{
    QString curdir = QDir::homePath();
    if (pathExisted(selectvideo->elidedText()))
        curdir = QFileInfo(selectvideo->elidedText()).dir().path();
    auto filter = "wav(*.wav);avi(*.avi);mp4(*.mp4)";
    QString path=QFileDialog::getOpenFileName(this,tr("Select Video"),curdir,filter);
    if (path.isEmpty())
        return;
    else selectvideo->setElidedText(path);
    //LOG<<selectvideo->elidedText();
}

void RecordBox::changeSavePath()
{
    QString dir =  QFileDialog::getExistingDirectory(this,tr("Select Save Dir"),savepath->elidedText(),
                                                     QFileDialog::ShowDirsOnly|QFileDialog::DontUseNativeDialog
                                                     |QFileDialog::DontResolveSymlinks);
    if (!dir.isEmpty()) savepath->setElidedText(dir);
}

RecordInfo RecordBox::recordInfo() const
{
    RecordInfo info;

    info[VideoFormatField] = getIndexFromFields(videoformat->currentText());
    info[ImageFormatField] = getIndexFromFields(imageformat->currentText());
    info[VideoFrameRateField] = framerate->currentText(); // 实际值就可以无需转换
    info[VideoSavePathField] = savepath->elidedText();

    return info;
}