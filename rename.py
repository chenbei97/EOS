'''
Author: chenbei97 chenbei_electric@163.com
Date: 2024-01-12 14:14:22
LastEditors: chenbei97 chenbei_electric@163.com
LastEditTime: 2024-01-12 15:19:18
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

        filetype = os.path.splitext(files)[1] 
        Newdir = os.path.join(path, str(i) + ".tif")   #新的文件路径
        os.rename(Olddir, Newdir)    #重命名
    return True

if __name__ == '__main__':
    rename()