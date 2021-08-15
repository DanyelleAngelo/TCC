import numpy as np
import matplotlib.pyplot as plt
import matplotlib.cm as cm
#https://matplotlib.org/stable/gallery/color/colormap_reference.html

barWidth=0.5

def graph(k_x, k_y, b_x, b_y, operacao,path,iterations,order):
    plt.figure("Range-min-Max tree - operação %s" %operacao,figsize=(18,18))

    x = np.arange(len(k_x)+1)
    colormap = plt.cm.cool #spring, winter,tab20b,  cool ->linspace(0.3, 0.6,len(k_x)+1)
    colors = [colormap(i) for i in np.linspace(0.2, 0.45,len(k_x)+1)] 

    i=0
    plt.rc('axes', axisbelow=True)
    plt.grid(axis='y',linestyle='dotted', color='lightgray')
    for k in order:
        plt.bar(k_x["k%s" %k],k_y["k%s"%k],color=colors[i],width=barWidth, label=k_x["k%s"%k], align='center')
        i+=1

    plt.bar(b_x, b_y,color=colors[i],width=barWidth,label=b_x,  align='center')

    plt.ylabel("Tempo médio para %s chamadas a função (ns) " %(iterations))
    plt.xlabel("Ordem da rmM-tree")

    yticks = [float(i) for i in k_y.values()]
    maxY = max(i for i in yticks)
    maxY = max(maxY,float(b_y[0])) +5
    minY = min(i for i in yticks)
    minY= min(minY,float(b_y[0]))-15
    plt.ylim(minY,maxY)
    #plt.yticks(np.arange(minY,maxY,5.5))
    plt.title("Tempo gasto pela operação %s para diferentes ordens da rmM-tree" %operacao)
    plt.savefig("%s/%s-i%s.png" %(path,operacao,iterations), bbox_inches='tight',dpi=300)
    plt.show()