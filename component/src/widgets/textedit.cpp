/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-17 17:04:22
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-17 17:07:32
 * @FilePath: \EOS\component\src\widgets\textedit.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "textedit.h"

TextEdit::TextEdit(QWidget*parent):QTextEdit(parent)
{
    setFontPointSize(DefaultFontSize);
    setFontFamily(DefaultFontFamily);
    setUndoRedoEnabled(false);
    setCurrentFont(QFont(DefaultFontFamily,DefaultFontSize));
    setReadOnly(true);
}

TextEdit::TextEdit(const QString& text,QWidget*parent): QTextEdit(text,parent)
{
    setFontPointSize(DefaultFontSize);
    setFontFamily(DefaultFontFamily);
    setUndoRedoEnabled(false);
    setCurrentFont(QFont(DefaultFontFamily,DefaultFontSize));
    setReadOnly(true);
}

QString TextEdit::text() const
{
    return toPlainText();
}

void TextEdit::setIndent(int moveLength,const QString& indent)
{ // 到左侧需要移动的光标距离
    QTextCursor cursorPosition = this->textCursor();
    cursorPosition.movePosition(QTextCursor::Left,QTextCursor::MoveAnchor,moveLength);
    cursorPosition.insertText(indent,QTextCharFormat());
}
