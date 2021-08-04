import sys
import numpy as np
import matplotlib.pyplot as plt
import plotGraph as pg

fileK = ""
fileB = " "
path = " "
order = " "
iterations = " "

def plot():
    kary_4 = []
    kary_8 = []
    kary_16 = []
    binary = []

    k_findOpenX, k_findOpenY, b_findOpenX, b_findOpenY = [], [], [], []
    k_findCloseX, k_findCloseY ,b_findCloseX, b_findCloseY = [], [],[],[]
    i=0
    
    infos = "i%s_m%s" %(iterations,order)

    with open("%s/kary_i500000_m4.csv" %(path,infos),"r") as f:
        kary_4 = f.readlines()[11:]

    with open("%s/kary_i500000_m8.csv" %(path,infos),"r") as f:
        kary_8 = f.readlines()[11:]

    with open("%s/kary_i500000_m16.csv" %(path,infos),"r") as f:
        kary_16 = f.readlines()[11:]

    with open("%s/bin_i500000.csv" %(path),"r") as f:
        binary = f.readlines()[11:]

    for linha in kary:
        col = [s for s in linha.split(",")]
        name = col[0].replace('"', '').split("/")
        if(name[1] == "findOpen_k"):
            k_findOpenX.append(name[2])
            k_findOpenY.append(float(col[3])) 
        
        elif(name[1] == "findClose_k"):
            k_findCloseX.append(name[2])
            k_findCloseY.append(float(col[3])) 
    
    for linha in binary:
        col = [s for s in linha.split(",")]
        name = col[0].replace('"', '').split("/")
        if(name[1] == "findOpen_b"):
            b_findOpenX.append(name[2])
            b_findOpenY.append(float(col[3])) 
        
        elif(name[1] == "findClose_b"):
            b_findCloseX.append(name[2])
            b_findCloseY.append(float(col[3]))
    
    pg.graph(k_findCloseX, k_findCloseY ,b_findCloseX, b_findCloseY,"findClose","%s%s"%(path,infos))
    pg.graph(k_findOpenX, k_findOpenY ,b_findOpenX, b_findOpenY, "findOpen","%s%s"%(path,infos))

def main(args):
    fileK = args[1]
    fileB = args[2]
    path = args[3]
    order = args[4]
    iterations = args[5]
    plot()
    return 0
 
if __name__ == '__main__':
    sys.exit(main(sys.argv))