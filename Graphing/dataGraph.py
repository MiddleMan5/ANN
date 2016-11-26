#!/usr/bin/env python
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

fig, (ax1, ax2) = plt.subplots(2,1)
# ax1 = plt.axes(xlim=(0, 10000), ylim=(0, 300))
line1, = ax1.plot([], [], lw=.08)
line2, = ax2.plot([], [], lw=1, color='r')

# plt.xlabel('Node Generation Number')
# plt.ylabel('Percent Error')
# plt.grid(True);

line = [line1, line2]
for ax in [ax1, ax2]:
    ax1.set_ylim(-10, 10)
    ax.set_ylim(0, 10)
    ax.set_xlim(0, 10000)
    ax.grid()

def init():
    line[0].set_data([], [])
    line[1].set_data([], [])
    return line

def animate(i):
    pullData = open("./Graphing/dataLog.csv","r").read()
    dataArray = pullData.split('\n')
    xar = []
    y1ar = []
    y2ar = []
    for eachLine in dataArray:
        if len(eachLine)>1:
            x,y1,y2 = eachLine.split(',')
            xar.append(int(x))
            y1ar.append(140*float(y1))
            y2ar.append(140*float(y2))
    line[0].set_data(xar, y1ar)
    line[1].set_data(xar, y2ar)
    return line

ani = animation.FuncAnimation(fig, animate, init_func=init,frames=20, interval=2, blit=True)
plt.show()
