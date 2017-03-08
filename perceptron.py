# -*- coding: utf-8 -*-
"""
Created on Wed Mar  8 09:36:15 2017

@author: kassio
"""

from sklearn import datasets
import numpy as np
from datetime import datetime

# banco de dados para teste: iris

iris = datasets.load_iris()
Iris = np.column_stack((iris.data, iris.target))
irissubdf = Iris[0:100,[0,2,4]]
irissubdf

irissubdf[irissubdf[:,2] == 0,2] = -1
irissubdf

x = irissubdf[:,[0,1]] 
y = irissubdf[:,2]

ii=50; niter=10; eta=1

def perceptron(x,y,eta,niter):
    weight = np.zeros(x.shape[1]+1)
    errors = np.zeros(niter)
    
    for jj in range(0,niter):
        for ii in range(0,len(y)):
            z = sum(weight[1:len(weight)] * x[ii,]) + weight[0]
            
            if z<0:
                ypred = -1
            else:
                ypred = 1
                
            weightdiff = eta * (y[ii] - ypred) * np.append(1,x[ii,])
            weight = weight + weightdiff
            
            if y[ii]-ypred != 0:
                errors[jj] = errors[jj] + 1
    
    print(weight)
    return errors
        

# microbenchmark

C = np.zeros(100);

for i in range(0,100):
    a = datetime.now()
    perceptron(x,y,1,10)
    b = datetime.now()
    c = b-a
    C[i] = c.microseconds

np.mean(C)
np.median(C)
np.max(C)
np.min(C)