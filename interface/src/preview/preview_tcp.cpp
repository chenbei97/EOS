/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-31 14:06:48
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-31 14:08:07
 * @FilePath: \EOS\interface\src\preview\preview_tcp.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "preview.h"

void Preview::takingPhoto()
{
    auto toolinfo = toolbar->toolInfo();
    previewinfo[PreviewToolField] = toolinfo;
    LOG<<"preview info = "<<previewinfo;

    //AssemblerPointer->assemble(TcpFramePool.frame0x0001,toolinfo);

    //LOG<<"request = "<<AssemblerMessage;

    //SocketPointer->exec(TcpFramePool.frame0x0001,AssemblerMessage);

//    QFile file(CURRENT_PATH+"/../test/test0x0001.json");
//    file.open(QIODevice::WriteOnly|QIODevice::Truncate);
//    QTextStream s(&file);
//    s<<AssemblerMessage;
//    file.close();
}

void Preview::previewView(const QPoint &point)
{
    
}

void Preview::saveExperConfig(const QString& path)
{ // 保存实验配置
    //LOG<<path<<previewinfo;
    previewinfo[PreviewPatternField] = pattern->patternInfo();
    previewinfo[PreviewToolField] = toolbar->toolInfo();

    AssemblerPointer->assemble(TcpFramePool.frame0x0001,previewinfo);
    auto json = AssemblerPointer->message();
    json.chop(3);//删掉尾缀@@@

    JsonReadWrite m; // 借助工具类写到文件内
    m.writeJson(path,json);
}