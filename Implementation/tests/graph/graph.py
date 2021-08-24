import sys
import numpy as np
import matplotlib.pyplot as plt
#import plotGraph as pg

data_2, data_4, data_8, data_16 =  [], [], [], []
exclude = ["IsLeaf", "Depth", "FirstChild"]
def graph(start,end, operacao,path,iterations):
    #plt.figure("Range-min-Max tree - operação %s e %s" %(operacao[0],operacao[1]),figsize=(18,18))
    x =  np.arange(2)
    width = 0.20
    
    fig, ax = plt.subplots(figsize=(18,12))

    pos = x + width/2
    rects_bin = ax.bar(x - width/2, data_2[start:end], width, label='Binária')
    rects_4 = ax.bar(pos, data_4[start:end], width, label='4-ária')
    rects_8 = ax.bar(pos + width, data_8[start:end], width, label='8-ária')
    rects_16 = ax.bar(pos + 2*width, data_16[start:end], width, label='16-ária')

    ax.set_ylabel("Tempo médio de CPU para %s chamadas das funções" %'{0:,}'.format(iterations).replace(',','.'))
    ax.set_title("Range-min-Max tree - operações %s e %s" %(operacao[0],operacao[1]))
    ax.set_xticks(pos + width/2)
    ax.set_xticklabels(operacao)
    ax.legend()

    ax.bar_label(rects_bin, padding=3)
    ax.bar_label(rects_4, padding=3)
    ax.bar_label(rects_8, padding=3)
    ax.bar_label(rects_16, padding=3)
    plt.savefig("%s/%s_and_%s-i%s.png" %(path,operacao[0],operacao[1],iterations), bbox_inches='tight',dpi=300)
    #plt.show()

def plot(path,iterations):
    order =[4,8,16]
    operacoes = []
    kary =  []
    binary = []
   
    #estrutura binária
    with open("%s/bin_i%s.csv" %(path,iterations),"r") as f:
        binary = f.readlines()[12:]

    for linha in binary:
        col = [s for s in linha.split(",")]
        name = col[0].replace('"', '').split("_")[1]
        operacoes.append(name)
        data_2.append(float(col[3])/iterations)

    #estrutura 4-aria
    with open("%s/kary_i%s_m4.csv" %(path,iterations), "r") as f:
        kary = f.readlines()[13:]
    for linha in kary:
        col = [s for s in linha.split(",")]
        data_4.append(float(col[3])/iterations)

    #estrutura 8-aria
    with open("%s/kary_i%s_m8.csv" %(path,iterations), "r") as f:
        kary = f.readlines()[13:]
    for linha in kary:
        col = [s for s in linha.split(",")]
        data_8.append(float(col[3])/iterations)

    #estrutura 16-aria
    with open("%s/kary_i%s_m16.csv" %(path,iterations), "r") as f:
        kary = f.readlines()[13:]
    for linha in kary:
        col = [s for s in linha.split(",")]
        data_16.append(float(col[3])/iterations) 

    order.append(2)
    start =0
    for j in range(1,len(data_2), 2):
        graph(start,j+1,[operacoes[start],operacoes[j]], "%s"%path,  iterations)
        start = j+1
  

def main(args):
    path = args[1]
    iterations = int(args[2])
    plot(path,iterations)
    return 0
 
if __name__ == '__main__':
    sys.exit(main(sys.argv))