#!/usr/bin/python

import numpy as np
import scipy.sparse as sp
from scipy.sparse.linalg import spsolve
from numpy.linalg import solve, norm

eps=np.finfo(float).eps
kMax=10000
tenAt8=10**8

def sparseMatrixStack(n,A):
	indptr=A.indptr
	indices=A.indices
	data=A.data

	storage=[]

	for i in range(len(indptr)-1):
		row=[]
		for j in range(indptr[i],indptr[i+1]):
			row.append((data[j],indices[j]))
		storage.append(row)

	return storage

def sparseMatrixNullDiag(n,A):
	# returns true if A's diagonal is null (filled with 0)
	# return (np.equal(A.diagonal(),np.zeros(n)))
	return (not False in (np.equal(A.diagonal(),np.zeros(n))))

def SOA(n,AStack,b,omega):
	if (omega <= 0 or omega >= 2):
		print("Omega must be in (0,2); Omega is {0}".format(omega))
		exit(-1)

	x = np.zeros(n)
	k=0
	sigma=0
	
	while (k <= kMax):
		for i in range(n):
			sigma=0
			for value_column in AStack[i]:
				j=value_column[1]
				if (i!=j):
					sigma+=value_column[0]*x[j]
						
			for value_column in AStack[i]:
				if (value_column[1]==i):
					# x[i]+=omega*(float((b[i]-sigma)/value_column[0])-x[i])
					x[i]+=float(omega/value_column[0])*(b[i]-sigma)
					break
		k+=1

		if (np.linalg.norm(x-x) < eps or np.linalg.norm(x-x) > tenAt8):
			return (x,k)
			break
		
		print ("Step: {0}".format(k))
		print (x)
		print(np.linalg.norm(x-x))
		# print(np.linalg.norm(x-sigma, np.inf))
		
	return (x,k)

def readFile(filename):

	f = open(filename,"r")

	n = int(f.readline())
	v,c,r,b=[],[],[],[]	#values, column_index, rows_start, b
	'''
		values = elements of the matrix that are not null;
		column_index = column index of the elements in "values"
		rows_start = the index in vector "values"/"column_index"
		of the first element from row i in the vectors "values"/"column_index"
	'''
	f.readline() #blank row after n
	#read values, column_index, lines_start vectors:
	while(1):
		row=f.readline()
		if (row=='\n'):
			break
		row = row.replace('\n','').replace(' ','').split(',')
		v.append(float(row[0]))
		c.append(int(row[1]))
		r.append(int(row[2]))
	#read vector of free terms: 
	while (1):
		row = f.readline()
		if (row=='\n'):
			break
		b.append(float(row.replace('\n','')))

	f.close()
	return n, sp.csr_matrix((v,(r,c)), shape=(n,n)), np.array(b)

if __name__ == '__main__':
	n,A,b=readFile("m_rar_2019_2.txt")
	# n,A,b=readFile("test.txt")
	# print (sparseMatrixStack(n,A))
	# print (sparseMatrixStack(n,A))
	# print (sparseMatrixNullDiag(n,A))
	print (np.linalg.norm(SOA(n,sparseMatrixStack(n,A),b,0.8)[0] - b, np.inf))
	print (np.linalg.norm(SOA(n,sparseMatrixStack(n,A),b,1)[0] - b, np.inf))
	print (np.linalg.norm(SOA(n,sparseMatrixStack(n,A),b,1.2)[0] - b, np.inf))
