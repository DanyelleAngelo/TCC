import numpy as np
import matplotlib.pyplot as plt
import plotGraph as pg

kary = []
binary = []

k_findOpenX, k_findOpenY, b_findOpenX, b_findOpenY = [], [], [], []
k_findCloseX, k_findCloseY ,b_findCloseX, b_findCloseY = [], [],[],[]
i=0

with open("../result_kary_rmm_tree.csv","r") as f:
    kary = f.readlines()[11:]

with open("../result_binary_rmm_tree.csv","r") as f:
    binary = f.readlines()[11:]

for linha in kary:
    col = [s for s in linha.split(",")]
    name = col[0].replace('"', '').split("/")
    if(name[1] == "findOpen_kary"):
        k_findOpenX.append(name[2])
        k_findOpenY.append(float(col[3])) 
    
    elif(name[1] == "findClose_kary"):
        k_findCloseX.append(name[2])
        k_findCloseY.append(float(col[3])) 
   
for linha in binary:
    col = [s for s in linha.split(",")]
    name = col[0].replace('"', '').split("/")
    if(name[1] == "findOpen_bin"):
        b_findOpenX.append(name[2])
        b_findOpenY.append(float(col[3])) 
    
    elif(name[1] == "findClose_bin"):
        b_findCloseX.append(name[2])
        b_findCloseY.append(float(col[3]))
        
pg.graph(k_findCloseX, k_findCloseY ,b_findCloseX, b_findCloseY, "findClose")
pg.graph(k_findOpenX, k_findOpenY ,b_findOpenX, b_findOpenY, "findOpen")

