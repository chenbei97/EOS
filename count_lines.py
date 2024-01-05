'''
Author: chenbei97 chenbei_electric@163.com
Date: 2023-12-19 09:07:21
LastEditors: chenbei97 chenbei_electric@163.com
LastEditTime: 2024-01-04 15:39:07
FilePath: \EOS\count_lines.py
Copyright (c) 2024 by ${chenbei}, All Rights Reserved. 
'''
import os
import time
path = os.path.dirname(os.path.abspath(__file__))
path1 = os.path.join(os.path.expanduser("~"), 'Desktop')+"/EOS"
path2 = os.path.join(os.path.expanduser("~"), 'Desktop')+"/EOSI_Tests"
# print(path1)
total_lines = 0
file_count = 0
def f(path):
    global total_lines,file_count
    for filepath,dirnames,filenames in os.walk(path):
        if filepath.count("cmake-build-debug") or filepath.count("bin") or filepath.count(".vscode")\
            or filepath.count(".git") or filepath.count("opencv2") or filepath.count("toupcam") \
            or filepath.count(".idea") or filepath.count("lib") or filepath.count("__pycache__") \
                or filepath.count("python310") or filepath.count("qml") or filepath.count("data")\
                or filepath.count("cmake-build-release"):
            continue
        
        for filename in filenames:
            
            if filename.count(".h") or filename.count(".cpp") or filename.count(".py") or filename.count("CMakeLists.txt"):
                    file_count += 1
                    p = os.path.join(filepath,filename)
                    print(file_count,p)
                    try:
                        count = len(open(p, 'r',encoding="utf-8",errors="ignore").readlines())
                        try:
                            count = len(open(p, 'r',encoding="gbk",errors="ignore").readlines())
                        except Exception as e:
                            print(e)
                    except Exception as e:
                        print(e)
                    total_lines += count              

if __name__ == '__main__':
    f(path)
    print("total lines = ",total_lines, "file count = ",file_count)
    # bufsize = (271 // 16 * 2) * 10
    # print(bufsize)
    time.sleep(1)