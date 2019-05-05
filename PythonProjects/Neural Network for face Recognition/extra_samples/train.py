'''
======================
Program to train the RNA
======================
'''
import cv2
import numpy as np
import random
import matplotlib.pyplot as plt

# names=["Edu","antonio","jorge","patty","jp","persona6",]
names=["Edu","antonio","jorge","patty","jp","america","amoran"]

ni=10 #numero de iteraciones
ncaras=len(names) #ncaras

samples =  np.empty((0,2500))
#######   training part    ############### 
samples_edu = np.loadtxt('data/data_images/Persons/%s/generalsamples.data'%(names[0]),np.float32)
samples = np.append(samples,samples_edu ,0)

samples_antonio = np.loadtxt('data/data_images/Persons/%s/generalsamples.data'%(names[1]),np.float32)
samples = np.append(samples,samples_antonio ,0)

samples_jorge = np.loadtxt('data/data_images/Persons/%s/generalsamples.data'%(names[2]),np.float32)
samples = np.append(samples,samples_jorge ,0)

samples_patty = np.loadtxt('data/data_images/Persons/%s/generalsamples.data'%(names[3]),np.float32)
samples = np.append(samples,samples_patty ,0)

samples_jp = np.loadtxt('data/data_images/Persons/%s/generalsamples.data'%(names[4]),np.float32)
samples = np.append(samples,samples_jp ,0)


#<--------------------------------------------------------------------------------------------------------------
#add mor people
samples_np6= np.loadtxt('data/data_images/Persons/%s/generalsamples.data'%(names[5]),np.float32)
samples = np.append(samples,samples_np6 ,0)

samples_np7= np.loadtxt('data/data_images/Persons/%s/generalsamples.data'%(names[6]),np.float32)
samples = np.append(samples,samples_np7 ,0)

# samples_np8= np.loadtxt('data/data_images/Persons/%s/generalsamples.data'%(names[7]),np.float32)
# samples = np.append(samples,samples_np8 ,0)

# samples_np9= np.loadtxt('data/data_images/Persons/%s/generalsamples.data'%(names[8]),np.float32)
# samples = np.append(samples,samples_np9 ,0)

# samples_np10= np.loadtxt('data/data_images/Persons/%s/generalsamples.data'%(names[9]),np.float32)
# samples = np.append(samples,samples_np10 ,0)
#<----

for i in range(0,30):
	img=  samples[i,:]
	test= img.reshape((50,50))
	cv2.imshow("cropped",test)
	cv2.waitKey(0)
cv2.destroyAllWindows()

yzeros=np.diag(np.ones(ncaras))
edu = np.asmatrix(yzeros[0,:])
antonio = np.asmatrix(yzeros[1,:])
jorge = np.asmatrix(yzeros[2,:])
patty = np.asmatrix(yzeros[3,:])
jp = np.asmatrix(yzeros[4,:])
# #<--------------------------------------------------------------------------------------------------------------
# #add mor people
np6 = np.asmatrix(yzeros[5,:])
np7 = np.asmatrix(yzeros[6,:])
# np8 = np.asmatrix(yzeros[7,:])
# np9 = np.asmatrix(yzeros[8,:])
# np10 = np.asmatrix(yzeros[9,:])
# #<----

yb =  np.empty((0,len(names)))
nfotos=0 #nfotos

#Edu
se=samples_edu.shape
nfotos=nfotos+ se[0]
for x in range(0, se[0]):
	yb = np.append(yb,edu ,0)

#Antonio
se=samples_antonio.shape
nfotos=nfotos+se[0]
for x in range(0, se[0]):
	yb = np.append(yb,antonio,0)

#Jorge
se=samples_jorge.shape
nfotos=nfotos+se[0]
for x in range(0, se[0]):
	yb = np.append(yb,jorge,0)

#Patty
se=samples_patty.shape
nfotos=nfotos+se[0]
for x in range(0, se[0]):
	yb = np.append(yb,patty,0)

#JP
se=samples_jp.shape
nfotos=nfotos+se[0]
for x in range(0, se[0]):
	yb = np.append(yb,jp,0)


#<--------------------------------------------------------------------------------------------------------------
#add mor people
# npersona 6
se=samples_np6.shape
nfotos=nfotos+se[0]
for x in range(0, se[0]):
	yb = np.append(yb,np6,0)

# #npersona 7
se=samples_np7.shape
nfotos=nfotos+se[0]
for x in range(0, se[0]):
	yb = np.append(yb,np7,0)

# #npersona 8
# se=samples_np8.shape
# nfotos=nfotos+se[0]
# for x in range(0, se[0]):
# 	yb = np.append(yb,np6,0)

# #npersona 9
# se=samples_np9.shape
# nfotos=nfotos+se[0]
# for x in range(0, se[0]):
# 	yb = np.append(yb,np6,0)

# #npersona 10
# se=samples_np10.shape
# nfotos=nfotos+se[0]
# for x in range(0, se[0]):
# 	yb = np.append(yb,np6,0)

#<----


x=np.array(samples)
h,w=x.shape
bias=np.ones((h,1))
x=np.column_stack((x,bias))
y=np.zeros(yb.shape) #current output

#Rows,columns MatLab
#Neural Network
ne=2501; #Numero de valores de cada foto + bias
nm=4*ncaras; #Numero de neuronas 4 x #de caras
ns=ncaras;  #Numero de caras

v = 0.001*np.random.random((ne,nm))
w = 0.001*np.random.random((nm,ns))

#Training of neural network
nx=nfotos #caras x # de fotos
eta=0.1/nx #0.1/nx


error =  np.zeros((nx,ns))
numIter=ni;

J =  np.zeros((numIter,1))
count =  np.zeros((numIter,1))

for iter in range (1,numIter):
	count[iter]=iter
	dJdw = 0;
	dJdv = 0;
	for k in range (0,nx): # 1 - 30
  		inX=np.transpose(np.asmatrix(x[k,:]))
  		m=np.transpose(v).dot(inX)
 		n = 1.0/(1.0+np.exp(-m)) 				# Sigmoid 1
 		# n = 2.0/(1+np.exp(-m)) - 1;    # sigmoid 2
		# n = np.exp(-np.asarray(m)*np.asarray(m));                 # Gaussian
		out=np.transpose(w).dot(n)
		y[k,:]=np.transpose(out)
		er = out - np.asmatrix(yb[k,:]).transpose()
		error[k,:]=er.transpose()
		
		#Derivates from "n"
		#Both values (dJdw,dJdv) should be added due to each loop only one row is being changed

		dndm =np.asarray(n)*(1-np.asarray(n))                # Sigmoid 1
		# dndm = (1 - np.asarray(n)*np.asarray(n))/2;          # Sigmoid 2
		# dndm = -2.0*(np.asarray(m)*np.asarray(m));          # Gaussian
 		dydw = n
		dJdw = 1*dJdw + dydw.dot(er.transpose())
		wedn=np.asarray(w.dot(er))*np.asarray(dndm)
		dJdv = 1*dJdv + inX.dot(np.transpose(wedn))
	
	v = v - (eta)*dJdv; 
	w = w - (eta)*dJdw;   
 	JJ = 0.5*sum(sum(error*error))
	print("JJ:  %0.12f"%JJ)
 	J[iter] = JJ

print("Finished!!")
np.savetxt('data/data_images/v.data',v)
np.savetxt('data/data_images/w.data',w)

plt.figure(1)
acaras = np.asmatrix(np.arange(ncaras))

plt.plot(y[1,:],'bo',label='Current values')
plt.plot(acaras,yb[0,:],'r*')
plt.legend()
plt.grid(True)
plt.xlim((-0.5,5.5))
plt.ylim((-0.1,1.2))

plt.figure(2)
plt.plot(count,J,'g')
plt.title('error')
plt.grid(True)
plt.show()