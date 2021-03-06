import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

fig = plt.figure()
ax1 = plt.axes(xlim=(0, 10000), ylim=(0, 100))
line, = ax1.plot([], [], lw=2)

plt.xlabel('Node Generation Number')
plt.ylabel('Percent Error')
plt.grid(True);

def init():
    line.set_data([], [])
    return line,

def animate(i):
    pullData = open("Python/errorLog.csv","r").read()
    dataArray = pullData.split('\n')
    xar = []
    yar = []
    for eachLine in dataArray:
        if len(eachLine)>1:
            x,y = eachLine.split(',')
            xar.append(int(x))
            yar.append(100*float(y))
    line.set_data(xar, yar)
    return line,

ani = animation.FuncAnimation(fig, animate, init_func=init,frames=200, interval=20, blit=True)
plt.show()
