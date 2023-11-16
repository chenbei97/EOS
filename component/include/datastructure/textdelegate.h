/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-16 16:15:45
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-16 16:22:36
 * @FilePath: \EOS\component\include\datastructure\textdelegate.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_MAIN_CORE_PY_TEXTDELEGATE_H
#define EOS_MAIN_CORE_PY_TEXTDELEGATE_H

#include "datastructure.h"
#include "textedit.h"

class COMPONENT_IMEXPORT TextDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const Q_DECL_OVERRIDE;
    void setEditorData(QWidget *editor, const QModelIndex &index) const Q_DECL_OVERRIDE;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const Q_DECL_OVERRIDE;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const Q_DECL_OVERRIDE;
    bool editorEvent(QEvent *event, QAbstractItemModel *model,
                     const QStyleOptionViewItem &option, const QModelIndex &index) Q_DECL_OVERRIDE;
};
#endif //EOS_MAIN_CORE_PY_TEXTDELEGATE_H
