'''
Author: chenbei97 chenbei_electric@163.com
Date: 2024-01-12 14:14:22
LastEditors: chenbei97 chenbei_electric@163.com
LastEditTime: 2024-01-12 14:19:02
FilePath: \EOS\rename.py
Copyright (c) 2024 by ${chenbei}, All Rights Reserved. 
'''
import os

def rename():
    i = 0
    path = r"C:\Users\22987\Desktop\EOS\bin\images\images"

    filelist = os.listdir(path)   #该文件夹下所有的文件（包括文件夹）
    for files in filelist:   #遍历所有文件
        i = i + 1
        Olddir = os.path.join(path, files)    #原来的文件路径
        if os.path.isdir(Olddir):       #如果是文件夹则跳过
                continue

        filetype = os.path.splitext(files)[1] 
        Newdir = os.path.join(path, str(i) + ".jpg")   #新的文件路径
        os.rename(Olddir, Newdir)    #重命名
    return True

if __name__ == '__main__':
    rename()