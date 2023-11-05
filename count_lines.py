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
            or filepath.count(".idea") or filepath.count("lib") or filepath.count("__pycache__"):
            continue
        
        for filename in filenames:
            
            if filename.count(".h") or filename.count(".cpp") or filename.count(".py") or filename.count("CMakeLists.txt"):
                    file_count += 1
                    p = os.path.join(filepath,filename)
                    print(file_count,p)
                    count = len(open(p, 'r',encoding="utf-8").readlines())
                    total_lines += count              

if __name__ == '__main__':
    f(path)
    print("total lines = ",total_lines, "file count = ",file_count)
    # bufsize = (271 // 16 * 2) * 10
    # print(bufsize)
    time.sleep(1)