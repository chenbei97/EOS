/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-16 16:03:16
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-16 16:22:07
 * @FilePath: \EOS\component\include\datastructure\linedelegate.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_MAIN_CORE_PY_LINEDELEGATE_H
#define EOS_MAIN_CORE_PY_LINEDELEGATE_H

#include "datastructure.h"
#include "lineedit.h"

class COMPONENT_IMEXPORT LineDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit LineDelegate(int min,int max,QObject*parent = nullptr);
    void setIntRange(int min,int max);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,const QModelIndex &index) const override;
private:
    int minval = 0;
    int maxval = 0;
};
#endif //EOS_MAIN_CORE_PY_LINEDELEGATE_H
