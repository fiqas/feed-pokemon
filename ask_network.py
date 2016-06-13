from pybrain.datasets import SupervisedDataSet
from pybrain.tools.shortcuts import buildNetwork
from pybrain.supervised.trainers import BackpropTrainer
import random
import numpy as np
import cv2
import sys
import pickle

filename = sys.argv[1].strip(".png") + ".jpg"
img2 = cv2.imread(filename, 0)
img = cv2.resize(img2, (0, 0), fx = 0.5, fy = 0.5)
img_vector = [float(255 - x) / 255 for sublist in img for x in sublist]

with open("neural_network.pickle", 'rb') as f:
	net = pickle.load(f)
	
print float(net.activate(img_vector))