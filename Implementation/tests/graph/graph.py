import sys
import numpy as np
import matplotlib.pyplot as plt
import plotGraph as pg

def plot(path,iterations):
    order =[4,8,16]
    kary =  []
    binary = []

    k_findOpenX,  k_findOpenY,  k_findCloseX,  k_findCloseY = {} , {} , {} , {}
    b_findOpenX,  b_findOpenY,  b_findCloseX,  b_findCloseY = [],  [],  [],  []

    for k in order:
        with open("%s/kary_i%s_m%s.csv" %(path,iterations,k), "r") as f:
            kary = f.readlines()[11:]
    
        for linha in kary:
            col = [s for s in linha.split(",")]
            name = col[0].replace('"', '').split("/")
            data = []
            if(name[1] == "findOpen_k"):
                k_findOpenX["k%s" %k]   = "%s-ary tree" %k
                k_findOpenY["k%s" %k]   = float(col[3])/iterations

            if(name[1] == "findClose_k"):
                k_findCloseX["k%s" %k]  = "%s-ary tree" %k
                k_findCloseY["k%s" %k]  = float(col[3])/iterations

    with open("%s/bin_i%s.csv" %(path,iterations),"r") as f:
        binary = f.readlines()[11:]
    
    for linha in binary:
        col = [s for s in linha.split(",")]
        name = col[0].replace('"', '').split("/")
        if(name[1] == "findOpen_b"):
            b_findOpenX.append("Bianry tree")
            b_findOpenY.append(float(col[3])/iterations)
        
        elif(name[1] == "findClose_b"):
            b_findCloseX.append("Bianry tree")
            b_findCloseY.append(float(col[3])/iterations)

    pg.graph(k_findCloseX, k_findCloseY, b_findCloseX, b_findCloseY, "findClose", "%s"%path,  iterations, order)
    pg.graph(k_findOpenX,  k_findOpenY,  b_findOpenX,  b_findOpenY,  "findOpen",  "%s"%path,  iterations, order)

def main(args):
    path = args[1]
    iterations = int(args[2])
    plot(path,iterations)
    return 0
 
if __name__ == '__main__':
    sys.exit(main(sys.argv))