import numpy as np
import matplotlib.pyplot as plt

def graph(k_op_x, k_op_y ,b_op_x, b_op_y,operacao,path):
    plt.plot(b_op_x, b_op_y,label='Binary rmM-tree',linewidth=1.0)
    plt.plot(k_op_x, k_op_y, label='Kary rmM-tree',linewidth=1.0)

    plt.xlabel("Entrada para %s" %operacao)
    plt.ylabel("Tempo de CPU gasto")
    plt.title("Tempo de CPU para as operações %s" %operacao)
    plt.legend()#para o label de cada plot
    plt.xticks([])
    plt.savefig("%s-%s.png" %(path,operacao),dpi=300)
    plt.show()