import numpy as np
import matplotlib.pyplot as plt

def graph(k_op_x, k_op_y ,b_op_x, b_op_y, operacao):
    plt.plot(b_op_x, b_op_y,label='Binary rmM-tree')
    plt.plot(k_op_x, k_op_y, label='Kary rmM-tree')

    plt.xlabel("Entrada para %s" %operacao)
    plt.ylabel("Tempo de CPU gasto")
    plt.title("Tempo de CPU para as operações %s" %operacao)
    plt.legend()#para o label de cada plot
    plt.xticks([])
    plt.show()
    plt.savefig("%s.png" %operacao,dpi=150)