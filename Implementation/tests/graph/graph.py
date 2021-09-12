import sys
import numpy as np
import matplotlib.pyplot as plt
#import plotGraph as pg

data_2, data_4, data_8, data_16 =  [], [], [], []
operacoes = []
exclude = ["IsLeaf", "Depth", "FirstChild"]
def graph(dataset,path,iterations):
    #plt.figure("Range-min-Max tree - operação %s e %s" %(operacao[0],operacao[1]),figsize=(18,18))
    x =  np.arange(len(operacoes))
    width = 0.20
    
    fig, ax = plt.subplots(figsize=(11.69,8.27))
    pos = x + width/2
    rects_bin = ax.bar(x - width/2, data_2, width, label='Binária')
    rects_4 = ax.bar(pos, data_4, width, label='4-ária')
    rects_8 = ax.bar(pos + width, data_8, width, label='8-ária')
    rects_16 = ax.bar(pos + 2*width, data_16, width, label='16-ária')
    ax.set_ylabel("Tempo médio de CPU em ns para %s chamadas" %'{0:,}'.format(iterations).replace(',','.'))
    ax.set_title("Range-min-Max tree - Tempo médio de operações em nanosegundos")
    ax.set_xticks(pos + width/2)
    ax.set_xticklabels(operacoes,rotation=65,fontsize=8)
    ax.legend()

    ax.bar_label(rects_bin, padding=8,rotation=90,fontsize=8)
    ax.bar_label(rects_4, padding=8,rotation=90,fontsize=8)
    ax.bar_label(rects_8, padding=8,rotation=90,fontsize=8)
    ax.bar_label(rects_16, padding=8,rotation=90,fontsize=8)
    plt.savefig("%s/%s_i%s.png" %(path,dataset,iterations), bbox_inches='tight',dpi=300)
    plt.show()

def plot(path,iterations):
    order =[4,8,16]
    kary =  []
    binary = []
    #estrutura binária
    with open("%s/bin_i%s.csv" %(path,iterations),"r") as f:
        binary = f.readlines()[13:]

    for linha in binary:
        col = [s for s in linha.split(",")]
        name = col[0].replace('"', '').split("_")[1]
        operacoes.append(name)
        data_2.append(float(col[3])/iterations)

    #estrutura 4-aria
    with open("%s/kary_i%s_m4.csv" %(path,iterations), "r") as f:
        kary = f.readlines()[14:]
    for linha in kary:
        col = [s for s in linha.split(",")]
        data_4.append(float(col[3])/iterations)

    #estrutura 8-aria
    with open("%s/kary_i%s_m8.csv" %(path,iterations), "r") as f:
        kary = f.readlines()[14:]
    for linha in kary:
        col = [s for s in linha.split(",")]
        data_8.append(float(col[3])/iterations)

    #estrutura 16-aria
    with open("%s/kary_i%s_m16.csv" %(path,iterations), "r") as f:
        kary = f.readlines()[14:]
    for linha in kary:
        col = [s for s in linha.split(",")]
        data_16.append(float(col[3])/iterations) 
    order.append(2)
    graph(path.split("/")[2], "%s"%path,  iterations)
  

def main(args):
    path = args[1]
    iterations = int(args[2])
    plot(path,iterations)
    return 0
 
if __name__ == '__main__':
    sys.exit(main(sys.argv))