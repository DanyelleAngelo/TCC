import numpy as np
import matplotlib.pyplot as plt
import plotGraph as pg

kary = []
binary = []

k_findOpenX, k_findOpenY, b_findOpenX, b_findOpenY = [], [], [], []
k_findCloseX, k_findCloseY ,b_findCloseX, b_findCloseY = [], [],[],[]
i=0
path = "../results/dna/"
infos = "5k_m1024_b16w8"
with open("%skary_%s.csv" %(path,infos),"r") as f:
    kary = f.readlines()[11:]

with open("%sbin_%s.csv" %(path,infos),"r") as f:
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