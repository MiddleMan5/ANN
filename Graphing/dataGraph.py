#!/usr/bin/env python
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

N = 5
ind = np.arange(N)  # the x locations for the groups
width = .75       # the width of the bars
x = (100, 100, 100, 100, 100)

fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2)
# fig, (ax1, ax2, ax3, ax4) = plt.subplots(2,1)
# ax1 = plt.axes(xlim=(0, 10000), ylim=(0, 300))
line1, = ax1.plot([], [], lw=.08, color='b')
line2, = ax2.plot([], [], lw=1, color='b')
line3, = ax3.plot([], [], lw=.08, color='r')
line4, = ax4.plot([], [], lw=1, color='r')
# rects1 = ax4.bar(ind + width, x, width, color='r')
# rects2 = ax4.bar(ind + width, womenMeans, width, color='b', yerr=womenStd)

# ax4.set_ylabel('Percentage')
# ax4.set_title('Nodal Activation')
# ax4.set_xticks(ind + width)
# ax4.set_xticklabels(('1', '2', '3', '4', '5'))


# ax4.legend(rects1[0], 'Activation')
# ax4.legend((rects1[0], rects2[0]), ('Activation', 'Weight'))

#From Here: http://matplotlib.org/examples/api/barchart_demo.html
# def autolabel(rects):
#     # attach some text labels
#     for rect in rects:
#         height = rect.get_height()
#         ax4.text(rect.get_x() + rect.get_width()/2., 1.05*height,
#             '%d' % int(height),
#             ha='center', va='bottom')

        #autolabel(rects1)
        # autolabel(rects2)

# plt.xlabel('Node Generation Number')
# plt.ylabel('Percent Error')
# plt.grid(True);
ax1.set_title('ANN Test Progress')

line = [line1, line2, line3, line4]
axis = [ax1,ax2,ax3]
for ax in axis:
    ax1.set_ylim(-3, 3)
    ax.set_ylim(0, 3)
    ax.set_xlim(0, 50)
    ax.grid()
    ax4.grid()

def init():
    line[0].set_data([], [])
    line[1].set_data([], [])
    line[2].set_data([], [])
    # for rect, h in zip(rects1, x):
    #          rect.set_height(h)
    return line

def animate(i):
    win = fig.canvas.manager.window
    pullData = open("./Graphing/dataLog.csv","r").read()
    dataArray = pullData.split('\n')
    #x=(100,100,100,100,100)
    xar = []
    y1ar = []
    y2ar = []
    for eachLine in dataArray:
        if len(eachLine)>1:
            x,y1,y2 = eachLine.split(',')
            xar.append(int(x))
            y1ar.append(70*float(y1))
            y2ar.append(70*float(y2))

    line[0].set_data(xar, y1ar)
    line[1].set_data(xar, y2ar)
    line[2].set_data(xar, y1ar)
    xmin, xmax = ax1.get_xlim()
    if (xar >= xmax).any():
        for i in range(3):
            axis[i].set_xlim(xmin, 2*xmax)


    # for rect, h in zip(rects1, y1ar):
    #          rect.set_height(h)
    return line

ax4.autoscale(tight=True)
ani = animation.FuncAnimation(fig, animate, init_func=init,frames=60,interval=50, blit=True)
plt.show()
