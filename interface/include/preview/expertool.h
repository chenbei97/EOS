
#ifndef EOS_MAIN_CORE_PY_EXPERTOOL_H
#define EOS_MAIN_CORE_PY_EXPERTOOL_H

#include "window.h"
#include "timebox.h"
#include "zstackbox.h"
#include "savebox.h"

class INTERFACE_IMEXPORT ExperTool: public QWidget
{
    Q_OBJECT
public:
    explicit ExperTool(QWidget*parent= nullptr);
    ExperToolInfo toolInfo() const;
    void importExperConfig(const QVariantMap& m);
private:
    TimeBox * timebox;
    ZStackBox * zstackbox;
    SaveBox * savebox;
    QVBoxLayout * lay;
signals:
    void zstackChanged(bool isChecked);// 是否勾选zstack和stitch
    void stitchChanged(bool isChecked);
    void exportFilePath(const QString&path); // 导出文件配置的路径
    void loadExper(); // 启动实验
signals:
    void objectiveChanged(const QString& obj);
};

#endif //EOS_MAIN_CORE_PY_EXPERTOOL_H
