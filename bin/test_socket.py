'''
Author: chenbei97 chenbei_electric@163.com
Date: 2023-10-19 15:25:08
LastEditors: chenbei97 chenbei_electric@163.com
LastEditTime: 2023-12-18 14:18:35
FilePath: \EOS\test\test_socket.py
Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
'''
import os,sys
import threading
from socket import socket
import json
import time
from collections import defaultdict
from random import randint
from queue import Queue
import numpy as np

class ParseManager:
    def __init__(self):
        self.frame = "frame"
        self.repeat = "repeat"
        self.separate = "@@@"
        self.x = "x"
        self.stop = "stop"
        self.y = "y"
        self.focus = "focus"
        self.brand = "brand"
        self.equip = "equip"
        self.path = "path"
        self.hole_x = "hole_x"
        self.hole_y = "hole_y"
        self.view_x = "view_x"
        self.view_y = "view_y"
        self.objective = "objective"
        self.wellsize = "wellsize"
        self.manufacturer = "manufacturer"
        self.viewsize = "viewsize"
        self.video_format = "video_format"
        self.video_framerate = "video_framerate"
        self.state = "state"
        self.activate_code = "activate_code"
        self.bright = "bright"
        self.channel = "channel"
        self.current_channel = "current_channel"
        self.turnoff_light = "turnoff_light"
        self.direction = "direction"
        self.objective_loc = "objective_loc"
        self.ishole = "ishole"
        self.isPH = "isPH"
        self.__socket = None
        self.parseFunctions = {
            "test0x0": self.__parsetest0x0,
            "test0x1": self.__parsetest0x1,
            "test0x2": self.__parsetest0x2,
            "test0x3": self.__parsetest0x3,
            "0": self.__parse0x0000,
            "1": self.__parse0x0001,
            "2": self.__parse0x0002,
            "3": self.__parse0x0003,
            "4": self.__parse0x0004,
            "5": self.__parse0x0005,
            "6": self.__parse0x0006,
            "7": self.__parse0x0007,
            "8": self.__parse0x0008,
            "9": self.__parse0x0009,
            "10": self.__parse0x0010,
            "11": self.__parse0x0011,
        }
    def setSocket(self, sock: socket):
        self.__socket = sock
    # 外部传入客户端套接字指针,这个套接字用于解析函数内使用可以给客户端回复消息
    def __parsetest0x0(self,msg: dict):
        frame = msg[self.frame]
        point_x = msg[self.x]
        point_y = msg[self.y]

        reponse = defaultdict()
        path = r"path/to/image/" + str(randint(1, 100))
        reponse[self.frame] = frame  # 只有这部分保留,逐坐标拍照需要发送的参数
        reponse[self.x] = point_x
        reponse[self.y] = point_y
        reponse[self.path] = path
        response = json.dumps(reponse)
        response+=self.separate
        print("test0x0回复: ", reponse)
        self.__socket.sendall(response.encode("utf-8"))
    def __parsetest0x1(self,msg: dict):
        frame = msg[self.frame]
        equip = msg[self.equip]

        reponse = defaultdict()
        reponse[self.frame] = frame  # 只有这部分保留,逐坐标拍照需要发送的参数
        reponse[self.equip] = equip
        response = json.dumps(reponse)
        response+=self.separate
        print("test0x1回复: ", reponse)
        self.__socket.sendall(response.encode("utf-8"))
    def __parsetest0x2(self,msg: dict):
        frame = msg[self.frame]
        path = msg[self.path]
        reponse = defaultdict()
        reponse[self.frame] = frame
        reponse[self.state] = "ok"
        response = json.dumps(reponse)
        response +=self.separate
        print("test0x2回复: ", reponse)
        time.sleep(3)
        self.__socket.sendall(response.encode("utf-8"))
    def __parsetest0x3(self,msg: dict):
        frame = msg[self.frame]
        x = msg[self.x]
        y = msg[self.y]
        reponse = defaultdict()
        reponse[self.frame] = frame
        reponse[self.state] = "ok"
        response = json.dumps(reponse)
        response +=self.separate
        print("test0x3回复: ", reponse)
        #time.sleep(3)
        self.__socket.sendall(response.encode("utf-8"))
    # 预览事件
    def __parse0x0000(self,msg:dict):
        frame = msg[self.frame]
        brand = msg[self.brand]
        hole_x = msg[self.hole_x]
        hole_x = msg[self.hole_y]
        view_x = msg[self.view_x]
        view_y = msg[self.view_y]
        wellsize = msg[self.wellsize]
        objective = msg[self.objective]
        manufacturer = msg[self.manufacturer]
        viewsize = msg[self.viewsize]
        #current_channel = msg[self.current_channel]
        ishole = msg[self.ishole]

        reponse = defaultdict()
        reponse[self.frame] = frame
        reponse[self.state] = "ok"
        response = json.dumps(reponse)
        response+=self.separate
        print("0x0000回复: ", reponse)
        #time.sleep(3)
        self.__socket.sendall(response.encode("utf-8"))

    # 启动实验
    def __parse0x0001(self,msg:dict):
        frame = msg[self.frame]
        reponse = defaultdict()
        reponse[self.frame] = frame
        reponse[self.state] = "ok"
        response = json.dumps(reponse)
        response+=self.separate
        print("0x0001回复: ", reponse)
        #time.sleep(5)
        self.__socket.sendall(response.encode("utf-8"))

    # 询问连接状态
    def __parse0x0002(self,msg:dict):
        frame = msg[self.frame]
        reponse = defaultdict()
        reponse[self.frame] = frame
        reponse[self.state] = "ok"
        response = json.dumps(reponse)
        response+=self.separate
        print("0x0002回复: ", reponse)
        #time.sleep(6)
        self.__socket.sendall(response.encode("utf-8"))

    # 回复激活码
    def __parse0x0003(self,msg:dict):
        frame = msg[self.frame]
        reponse = defaultdict()
        reponse[self.frame] = frame
        reponse[self.activate_code] = "89732kjdcjkwa"+str(np.random.randint(0,100))
        response = json.dumps(reponse)
        response+=self.separate
        print("0x0003回复: ", reponse)
        #time.sleep(1)
        self.__socket.sendall(response.encode("utf-8"))

    # 滑动条调节br
    def __parse0x0004(self,msg:dict):
        frame = msg[self.frame]
        bright = msg[self.bright]
        channel = msg[self.current_channel]
        reponse = defaultdict()
        reponse[self.frame] = frame
        reponse[self.state] = "ok"
        response = json.dumps(reponse)
        response+=self.separate
        print("0x0004回复: ", reponse)
        #time.sleep(1)
        self.__socket.sendall(response.encode("utf-8"))

    # 更改通道
    def __parse0x0005(self,msg:dict):
        frame = msg[self.frame]
        bright = msg[self.bright]
        channel = msg[self.current_channel]
        turnoff_light = msg[self.turnoff_light] # = 1
        reponse = defaultdict()
        reponse[self.frame] = frame
        reponse[self.state] = "ok"
        response = json.dumps(reponse)
        response+=self.separate
        print("0x0005回复: ", reponse)
        time.sleep(2)
        self.__socket.sendall(response.encode("utf-8"))

    # 移动镜头
    def __parse0x0006(self,msg:dict):
        frame = msg[self.frame]
        direction = msg[self.direction]
        reponse = defaultdict()
        reponse[self.frame] = frame
        reponse[self.state] = "ok"
        response = json.dumps(reponse)
        response+=self.separate
        print("0x0006回复: ", reponse)
        #time.sleep(1)
        self.__socket.sendall(response.encode("utf-8"))

    # 移动电机
    def __parse0x0007(self,msg:dict):
        frame = msg[self.frame]
        objective_loc = msg[self.objective_loc]
        reponse = defaultdict()
        reponse[self.frame] = frame
        reponse[self.state] = "ok"
        response = json.dumps(reponse)
        response+=self.separate
        print("0x0007回复: ", reponse)
        #time.sleep(3)
        self.__socket.sendall(response.encode("utf-8"))

    # 停止实验
    def __parse0x0008(self,msg:dict):
        frame = msg[self.frame]
        stop = msg[self.stop]
        reponse = defaultdict()
        reponse[self.frame] = frame
        reponse[self.state] = "ok"
        response = json.dumps(reponse)
        response+=self.separate
        print("0x008回复: ", reponse)
        #time.sleep(3)
        self.__socket.sendall(response.encode("utf-8"))

    # 切物镜动电机事件
    def __parse0x0009(self,msg:dict):
        frame = msg[self.frame]
        objective = msg[self.objective]
        loc = msg[self.objective_loc]
        isph = msg[self.isPH]
        reponse = defaultdict()
        reponse[self.frame] = frame
        reponse[self.state] = "ok"
        response = json.dumps(reponse)
        response+=self.separate
        print("0x0009回复: ", reponse)
        #time.sleep(3)
        self.__socket.sendall(response.encode("utf-8"))

    # 录制视频事件
    def __parse0x0010(self,msg:dict):
        frame = msg[self.frame]
        path = msg[self.path]
        format = msg[self.video_format]
        rate = msg[self.video_framerate]
        reponse = defaultdict()
        reponse[self.frame] = frame
        reponse[self.state] = "ok"
        response = json.dumps(reponse)
        response+=self.separate
        print("0x0010回复: ", reponse)
        self.__socket.sendall(response.encode("utf-8"))

    # 手动调焦事件
    def __parse0x0011(self,msg:dict):
        frame = msg[self.frame]
        focus = msg[self.focus]
        reponse = defaultdict()
        reponse[self.frame] = frame
        reponse[self.state] = "ok"
        response = json.dumps(reponse)
        response += self.separate
        print("0x0011回复: ", reponse)
        #time.sleep(1)
        self.__socket.sendall(response.encode("utf-8"))
class SocketServerManger:
    def __init__(self, port=3000):  # 测试本地链接,只需要提供端口
        self.__port = port
        self.__hostName = "localhost"
        self.__msgQueue = Queue()
        self.__socket = socket()
        self.__socket.bind((self.__hostName, self.__port))
        self.__isConnected = False
        self.__parser = ParseManager()
        self.__parseFunctions = self.__parser.parseFunctions

    def waitForConnected(self, blockSecs=10):
        print("wait for connection...")
        self.__socket.settimeout(blockSecs)  # 必须在这里设置,下边的listen设置不起作用会一直阻塞
        try:
            self.__socket.listen()
            self.client, client_addr = self.__socket.accept()
            print("connect successful!")
            self.__isConnected = True
            self.__parser.setSocket(self.client)  # 把客户端指针给解析类使用
            self.__run()
        except:
            self.__isConnected = False
            print("have no connection")
    def __run(self):
        if not self.__isConnected:
            return
        self.__thread1 = threading.Thread(target=self.__recvFromClient)
        self.__thread2 = threading.Thread(target=self.__parseMessage)
        self.__thread3 = threading.Thread(target=self.__sendMessage)
        self.__thread1.start()
        self.__thread2.start()
        self.__thread3.start()
        self.__thread1.join()
        self.__thread2.join()
        self.__thread3.join()

    def __sendMessage(self):
        while self.__isConnected:
            reponse = defaultdict()
            reponse["frame"] = 14 # 实验结束
            reponse["exper_finished"] = 1
            response = json.dumps(reponse)
            response += "@@@"
            time.sleep(20)
            #self.client.sendall(response.encode("utf-8"))

    def __recvFromClient(self):
        while self.__isConnected:
            msg = self.client.recv(10240).decode("utf-8")  # 字符串形式,0x0000命令长度>1024
            # 1. 判断是单条还是多条命令
            #print("msg.count(@@@) = ",msg.count(self.__parser.separateStr))
            count = msg.count(self.__parser.separate)
            if count == 1:
                self.__msgQueue.put(msg.removesuffix(self.__parser.separate))  # 放入消息队列（可能包含多条命令）
            else:
                msgs = msg.split(self.__parser.separate)
                for msg in msgs:
                    if msg: # split会多出个空字符串
                        self.__msgQueue.put(msg)
            #print("接收: ", msg, " 队列长度: ", self.__msgQueue.qsize())
    def __parseMessage(self):
        while self.__isConnected:
            while not self.__msgQueue.empty():
                msg = self.__msgQueue.get()  # 可能是多条命令,拆分的方法是{}一条完整命令
                print("取出: ", msg, " 队列长度: ",self.__msgQueue.qsize())
                #print("队列长度: ",self.__msgQueue.qsize())
                msg_dict = json.loads(msg)
                frame = msg_dict[self.__parser.frame] # 客户端传的数字不是字符串了
                self.__parseFunctions[str(frame)](msg_dict)
                time.sleep(0.0001) # 由于已经通过@@@区分开了,下方根据{}的程序也就不必了
def test_server():
    m = SocketServerManger()
    m.waitForConnected()


if __name__ == '__main__':
    print("random = ",np.random.randint(0,100));
    print('os.getcwd(): ',os.getcwd())
    print('sys.executable: ',sys.executable)
    test_server()