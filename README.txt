Name : Geetansh Juneja
Entry No: 2020CS50649
Filename: matrix_op.cpp

Instruction to run the program:

1. In the command line first run make.
2. Then to execute the program run .\yourcode.out (arguments)

Function defined in file:

1. fully connected (FC) layer: This function calculates the innerproduct of input and weight matrix and add bias vector to it.
2. relu: Applies every element of the input matrix with relu activation function i.e max(0,x).
3. tanh : Applies every element of the input matrix with relu activation function i.e (e^x-e^-x)/(e^x+e^-x).
4. max pooling : Subsamples the given input square matrix by applying filter of size stride*stride on input matrix which return the maximum value of the area where it is applied.
5. average pooling : Subsamples the given input square matrix by applying filter of size stride*stride on input matrix which return the average value of the area where it is applied.
6. sigmoid : It converts a input vector to a vector of probability by applying every element of input vector with function 1/(1+e^-x)
7. softmax : It converts a input vector to a vector of probability by applying every element of input vector with function e^x/sun(e^t) where t belongs to input vector.