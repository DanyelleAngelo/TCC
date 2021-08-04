import numpy as np
import matplotlib.pyplot as plt

barWidth=0.5
rgb = ["#6A5ACD", "#6495ED",   "#00BFFF", "#008CFF"] 

def graph(k_x, k_y, b_x, b_y, operacao,path,iterations,order):
    plt.figure(figsize=(7,15))

    i=0
    plt.grid(axis='y',linestyle='dotted', color=rgb[i])
    for k in order:
        plt.bar(k_x["k%s" %k],k_y["k%s"%k],color=rgb[i],width=barWidth, label=k_x["k%s"%k], align='center')
        i+=1

    plt.bar(b_x, b_y,color=rgb[i],width=barWidth,label=b_x,  align='center')

    plt.ylabel("Tempo médio para %s chamadas a função (ns) " %(iterations))
    plt.xlabel("Ordem da rmM-tree")
    plt.title("Desempenho da k-ary rmM-tree para a operação %s" %operacao)
    plt.savefig("%s/%s-i%s.png" %(path,operacao,iterations),dpi=300)
    #plt.show()