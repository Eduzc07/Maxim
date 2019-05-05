#!/usr/bin/env python
'''
Programa de entrenamiento de redes neuronales para 
reconocer anomalias cardiacas
'''
import cv2
import numpy as np
import random
import matplotlib.pyplot as plt

import scipy.io as sio

mat_contents = sio.loadmat('ECGVilla.mat')



print mat_contents