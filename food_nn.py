from pybrain.datasets import SupervisedDataSet
from pybrain.tools.shortcuts import buildNetwork
from pybrain.supervised.trainers import BackpropTrainer
import random
import numpy as np
import cv2
import sys
import pickle

with open(sys.argv[1]) as f:
    train_set = [x.strip() for x in f.readlines()]
with open(sys.argv[2]) as f:
    test_set = [x.strip() for x in f.readlines()]

pictures = {}
data_model = []
input_layer = 600

for filename in train_set:
    img2 = cv2.imread(filename, 0)
    img = cv2.resize(img2, (0, 0), fx = 0.5, fy = 0.5)
    img_vector = [float(255 - x) / 255 for sublist in img for x in sublist]
    pictures[filename] = img_vector
    if filename.endswith("_e.jpg"):
        data_model.append([tuple(img_vector), (0,)])
    else:
        data_model.append([tuple(img_vector), (1,)])

for filename in test_set:
    img2 = cv2.imread(filename, 0)
    img = cv2.resize(img2, (0, 0), fx = 0.5, fy = 0.5)
    img_vector = [float(255 - x) / 255 for sublist in img for x in sublist]
    # print >> sys.stderr, len(img_vector)
    pictures[filename] = img_vector

print >> sys.stderr, "Loaded pictures"

ds = SupervisedDataSet(input_layer, 1)
for input, target in data_model:
    ds.addSample(input, target)

# create a large random data set
random.seed()
trainingSet = SupervisedDataSet(input_layer, 1);
for ri in range(0,1000):
    input,target = random.choice(data_model);
    trainingSet.addSample(input, target)

net = buildNetwork(input_layer, 30, 1, bias=True)

trainer = BackpropTrainer(net, ds, learningrate = 0.01, momentum = 0.99)
trainer.trainUntilConvergence(verbose=True,
                              dataset=trainingSet,
                              maxEpochs=10)

print >> sys.stderr, "Finished!"

with open("neural_network.pickle", 'wb') as f:
	pickle.dump(net, f)
	
with open("neural_network.pickle", 'rb') as f:
	net = pickle.load(f)

for filename in train_set:
    print filename + " ", float(net.activate(pictures[filename]))
# print >> sys.stderr, "----"
for filename in test_set:
    print filename + " ", float(net.activate(pictures[filename]))
