## **************************************************************************
##  File       [main.cpp]
##  Author     [Yu-Wei Chang]
##  Synopsis   [The main program of 2017 Spring Algorithm Final Project]
##  Modify     [2017/05/11 Yu-Wei Chang]
## **************************************************************************
#  
# encoding: utf-8
# 使用方法如下所述，
# 在terminal上打指令：python drawinput.py [要讀取的檔案] [正數坐標中的最大值] [負數坐標中的最小值]

import sys
from Tkinter import *


fin = open(sys.argv[1])
content = fin.readlines()
# print content

master = Tk()
pos_scale = float(sys.argv[2]) 
neg_scale = float(sys.argv[3])
w = Canvas(master, width=600, height=600)
w.pack()

scale = pos_scale - neg_scale

for i,line in enumerate(content):
    if i<=2:
        continue
    index = line.split(",")
    w.create_rectangle((float(index[0])+scale/2)*600/scale, (float(index[1])+scale/2)*600/scale, (float(index[2])+scale/2)*600/scale, (float(index[3][:-1])+scale/2)*600/scale, fill="blue")
mainloop()
