#!/usr/bin/python

import numpy as np
import scipy.sparse as sp
from scipy.sparse.linalg import spsolve
from numpy.linalg import solve, norm

def sparseMatrixStack(n,A):
	# A=sp.csr_matrix((v,(r,c)), shape=(n, n))
	indptr=A.indptr
	indices=A.indices
	data=A.data

	print(indptr)
	print(indices)
	print(data)

	storage=[]

	for i in range(len(indptr)-1):
		row=[]
		for j in range(indptr[i],indptr[i+1]):
			row.append((data[j],indices[j]))
		# print(row)
		storage.append(row)

	return storage

def sparseMatrixNullDiag(n,A):
	# A=sp.csr_matrix((v,(r,c)), shape=(n,n))
	# di = np.diag_indices(n)
	# return A[di]
	print (A)
	print (A.diagonal())
	return (not False in (np.equal(A.diagonal(),np.zeros(n))))

def aproxSolution(n,A,omega):
	return None

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
	# n,v,c,r,b=readFile("m_rar_2019_1.txt")
	n,A,b=readFile("test.txt")
	# print (sparseMatrixStack(n,A))
	# print (sparseMatrixNullDiag(n,A))

