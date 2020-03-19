#!/usr/bin/env python
'''
========================
Program to train the RNA
========================
'''
import cv2
import numpy as np
import random
import matplotlib.pyplot as plt
import os

# names=["Edu","antonio","jorge","patty","jp","america","amoran"]
#It it does not work, add manually all the names
names = os.listdir("data/data_images/Persons/") 
#names=["Edu","america","amoran"]
print(names)

ni = 1000 #numero de iteraciones
ncaras = len(names) #ncaras

def activation(m):
	#Sigmoid 1
	n = 1.0/(1.0 + np.exp(-m))
	# n = 2.0/(1+np.exp(-m)) - 1;    # sigmoid 2
	# n = np.exp(-np.asarray(m)*np.asarray(m));                 # Gaussian
	return n

def main():
	samples = np.empty((0, 2500))
	samples_pp = np.empty((0, 2500))
	yb = np.empty((0, ncaras))

	yzeros = np.diag(np.ones(ncaras))
	yb = np.empty((0, ncaras))
	nfotos = 0 #nfotos
	nArrayNumberFotos = np.zeros(ncaras, dtype = int)
	for i in range(0, ncaras):
		#print(names[i])
		samples_pp = np.loadtxt('data/data_images/Persons/%s/generalsamples.data'%(names[i]), np.float32)
		#print(samples_pp.shape)
		if len(samples_pp)<=0:
  			print("[Error!!] \"data/data_images/Persons/%s\": %s was not trained or is empty!!"%(names[i],names[i]))
  			quit()
		samples = np.append(samples, np.asmatrix(samples_pp), 0)

		# Create the Labels
		# Rest 1 because the generalsamples.data
		for j in range(0, samples_pp.shape[0]):
			yb = np.append(yb, np.asmatrix(yzeros[i,:]), 0)
			nfotos += 1
		
		nArrayNumberFotos[i] = nfotos - 1

	#--------------------------------------------
	# Display all the saved images
	#--------------------------------------------
	# n = 0
	# for i in range(0, len(names)):
	# 	title = names[i]	
	# 	n_person = len(os.listdir("data/data_images/Persons/%s"%names[i]))
	# 	for j in range(0, n_person-1):
	# 		img =  samples[n,:]
	# 		test = img.reshape((50,50))
	# 		cv2.imshow(title, test)
	# 		cv2.waitKey(0)
	# 		n+=1
	# 	cv2.destroyWindow(title)
	#--------------------------------------------

	x = np.array(samples)
	h, w = x.shape
	bias = np.ones((h, 1))
	#current input
	x = np.column_stack((x, bias))
	#current output
	y = np.zeros(yb.shape)

	print(x.shape)
	print(y.shape)

	# Rows, Columns MatLab
	#---------------------------------------------
	# Neural Network
	#---------------------------------------------
	# Hyperparameters
	ne = 2501; 		#Numero de valores de cada foto + bias
	# Hidden Layers
	nm = 4*ncaras; 	#Numero de neuronas 4 x #de caras
	ns = ncaras;  	#Numero de caras

	v = 0.001*np.random.random((ne,nm))
	w = 0.001*np.random.random((nm,ns))

	#Training of neural network
	nx = x.shape[0] #nfotos #caras x # de fotos
	eta = 0.1/nx #0.1/nx

	error = np.zeros((nx, ns))
	numIter = ni + 1

	#Cost function, variables to plot
	J = np.zeros((numIter, 1))
	count = np.zeros((numIter, 1))

	for iter in range (0, numIter):
		count[iter] = iter
		dJdw = 0
		dJdv = 0

		# one Batch
		for k in range (0, nx): # 1 - 30
	  		inX = np.transpose(np.asmatrix(x[k,:]))
	  		m = np.transpose(v).dot(inX)

			# Forward
	  		# Activation Function
	 		n = activation(m)

			out = np.transpose(w).dot(n)
			y[k,:] = np.transpose(out)
			er = out - np.asmatrix(yb[k,:]).transpose()
			error[k,:] = er.transpose()
			
			#Derivates from "n"
			#Both values (dJdw,dJdv) should be added due to each loop only one row is being changed

			#Backpropagation
			dndm = np.asarray(n)*(1 - np.asarray(n))                # Sigmoid 1
			# dndm = (1 - np.asarray(n)*np.asarray(n))/2;          # Sigmoid 2
			# dndm = -2.0*(np.asarray(m)*np.asarray(m));          # Gaussian
	 		dydw = n
			dJdw = 1*dJdw + dydw.dot(er.transpose())
			wedn = np.asarray(w.dot(er))*np.asarray(dndm)
			dJdv = 1*dJdv + inX.dot(np.transpose(wedn))
		
		#print(error)
		#Update Weights
		v = v - (eta)*dJdv; 
		w = w - (eta)*dJdw;   
		# Calculating Cost function
	 	JJ = 0.5*sum(sum(error*error))
	 	if (iter%250 == 0 or iter == 0):
			print("JJ: %0.12f ---- [%d/%d]"%(JJ, iter, ni))

	 	J[iter] = JJ

	#print(y)
	print("Finished!!")
	np.savetxt('data/data_images/v.data', v)
	np.savetxt('data/data_images/w.data', w)

	plt.figure(1)
	acaras = np.asmatrix(np.arange(ncaras))

	# names=["Edu","antonio","jorge","patty","jp","america","amoran"]
	for person in range (0, ncaras):
		plt.figure(person)
		plt.plot(y[nArrayNumberFotos[person],:],'go', label='Output')
		plt.plot(yzeros[person,:],'r*', label='Labels')
		plt.title(names[person])
		plt.legend()
		plt.grid(True)
		plt.xlim((-0.5, 6.5))
		plt.ylim((-0.1, 1.5))

	plt.figure(ncaras)
	plt.plot(count, J, 'g')
	plt.title('Error')
	plt.grid(True)
	plt.show()

if __name__ == '__main__':
    print(__doc__)
    main()