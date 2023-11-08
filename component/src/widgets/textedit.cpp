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

}

TextEdit::TextEdit(const QString& text,QWidget*parent): QTextEdit(text,parent)
{

}

void TextEdit::setIndent(int moveLength,const QString& indent)
{ // 到左侧需要移动的光标距离
    QTextCursor cursorPosition = this->textCursor();
    cursorPosition.movePosition(QTextCursor::Left,QTextCursor::MoveAnchor,moveLength);
    cursorPosition.insertText(indent,QTextCharFormat());
}
