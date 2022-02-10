#include<iostream>
#include<string>
#include<vector>
#include<math.h>
#include<fstream>
#include<algorithm>

using namespace std;

typedef vector<vector<float>> vc2d;
typedef vector<float> vc1d;

// Function to take input matrix from a file
vc2d input_matrix(string file_name){
    int c,r;
    ifstream myfile;
    myfile.open(file_name);
    if(myfile.fail()){
        cerr<<"File doen't exist\n";
        exit(0);
    }
    myfile >> c >> r;
    vc2d inp(r, vc1d(c,0));
    for(int j=0; j<c; j++){
        for(int i=0; i<r; i++){
            float t;
            myfile >> t;
            inp[i][j] = t;
        }
    }
    myfile.close();
    return inp;
    
}

// Function to take input vector from a file
vc1d input_vector(string filename){
    vc1d v;
    ifstream myfile;
    myfile.open(filename);
    int a;
    myfile >> a;
    for(int i=0;i<a;i++){
        float t;
        myfile >> t;
        v.push_back(t);
    }
    myfile.close();
    return v;
}

// Function to write a output matrix in column major order in a file
void ouput_matrix(vc2d v, string filename){
    ofstream myfile(filename, ofstream::trunc);
    if(myfile.is_open()){
        myfile << v[0].size() <<"\n";
        myfile << v.size() << "\n";
        for(int i=0;i<v[0].size();i++){
            for(int j=0;j<v.size();j++){
                myfile << v[j][i] << "\n";
            }
        }
    }
}

// Function to write a output vector in a file
void ouput_vector(vc1d v, string filename){
    ofstream myfile(filename, ofstream::trunc);
    if(myfile.is_open()){
        myfile << v.size() << "\n";
        for(int i=0;i<v.size();i++){
            myfile << v[i] << "\n";
        }
    }
}

// Function to calculate inner product of two matrices and add bias vector.
vc2d FClayer(vc2d &input_matrix, vc2d &weight_matrix, vc2d &bias_vector){
    int n = input_matrix[0].size();
    int m = weight_matrix.size();
    int a = input_matrix.size();
    int b = weight_matrix[0].size();
    vc2d final(a, vc1d(b,0));
    for(int i=0;i<a;i++){
        for(int j=0;j<b;j++){
            for(int k=0;k<n;k++){
                final[i][j] += input_matrix[i][k]*weight_matrix[k][j];
            }
            final[i][j] += bias_vector[i][j];
        }
    }
    
    return final;
}

// Function applies every element of the matrix with relu activation function
vc2d relu(vc2d input_matrix){
    vc2d v(input_matrix.size(), vc1d (input_matrix[0].size(),0));
    for(int i=0;i<input_matrix.size();i++){
        for(int j=0;j<input_matrix[i].size();j++){
            if(input_matrix[i][j] < 0.0f) v[i][j]=0.0f;
            else v[i][j] = input_matrix[i][j];
        }
    }
    return v;
}

// Function applies every element of the matrix with tanh function
vc2d my_tanh(vc2d &inp){
    vc2d v(inp.size(), vc1d (inp[0].size(),0));
    for(int i=0;i<inp.size();i++){
        for(int j=0;j<inp[i].size();j++){
             float k = inp[i][j];
             v[i][j] = (float)(exp(k)-exp(-k))/(exp(k)+exp(-k));
        }
    }
    return v;
}

// Return the max element from a sub matrix starting from indexing a and b.
float max_matrix(vc2d v, int a, int b, int s){
    float m = v[a][b];
    for(int i=a;i<a+s;i++){
        for(int j=b;j<b+s;j++){
            if(v[i][j]>m) m=v[i][j];
        }
    }
    return m;
}

// Function to subsample any square matrix using max pooling.
vc2d max_pool(vc2d &inp, int s){
    int a = inp.size();
    vc2d out(a/s, vc1d(a/s,0));
    for(int i=0;i<a/s;i++){
        for(int j=0;j<a/s;j++){
            out[i][j] = max_matrix(inp, i*s, j*s, s);
        }
    }
    return out;
}

// Return the mean element of a sub matrix starting from indexing a and b.
float avg_matrix(vc2d v, int a, int b, int s){
    float m = 0;
    for(int i=a;i<a+s;i++){
        for(int j=b;j<b+s;j++){
            m = m + v[i][j];
        }
    }
    return m/(s*s);
}

// Function to subsample any square matrix using average pooling.
vc2d avg_pool(vc2d &inp, int s){
    int a = inp.size();
    vc2d out(a/s, vc1d(a/s,0));
    for(int i=0;i<a/s;i++){
        for(int j=0;j<a/s;j++){
            out[i][j] = avg_matrix(inp, i*s, j*s, s);
        }
    }
    return out;
}

// Applies every element of a vector with sigmoid function
vc1d sigmoid(vc1d &v){
    vc1d a(v.size(),0);
    for(int i=0;i<v.size();i++){
        a[i] = 1/(1+exp(-v[i]));
    }
    return a;

}

// Applies every element of a vector with softmax function
vc1d softmax(vc1d &v){
    vc1d a(v.size(),0);
    float s;
    for(int i=0;i<v.size();i++){
        s = s + exp(v[i]);
    }
    for(int i=0;i<v.size();i++){
        a[i] = exp(v[i])/s;
    }
    return a;
}


int main(int argc,char **argv)
{
    string s = argv[1];              // Reads the second argument from command line
    if(s.compare("fullyconnected")==0 && argc==6){
        string a = argv[2], b= argv[3], c= argv[4], d= argv[5];
        cout<<a<<"\n";
        vc2d abc = input_matrix(a);       // Reads the input matrix from filename in var a
        vc2d weight = input_matrix(b);    // Reads the wight matrix from filename in var a
        vc2d bias = input_matrix(c);      // Reads the bias matrix from filename in var a
        vc2d out = FClayer(abc, weight, bias);    
        ouput_matrix(out,d);         // writes the matrix in File
    }
    else if(s.compare("activation")==0 && argc==5){
        string a = argv[2], b= argv[3], c= argv[4];
        if(a.compare("relu")==0){
            vc2d abc = input_matrix(b);  // Reads the input matrix from filename in var b
            vc2d out = relu(abc); 
            ouput_matrix(out,c);      // writes the matrix in File 
        }
        else if(a.compare("tanh")==0){
            vc2d abc = input_matrix(b); // Reads the input matrix from filename in var b
            vc2d out = my_tanh(abc);
            ouput_matrix(out,c);   // writes the matrix in File 
        }
        else{
        cerr << "Wrong arguments passed\n";
        }
    }
    else if(s.compare("pooling")==0 && argc==6){
        string a = argv[2], b= argv[3], c = argv[5];
        if(a.compare("max")==0){
            vc2d abc = input_matrix(b);    // Reads the input matrix from filename in var b
            vc2d out = max_pool(abc, atoi(argv[4]));
            ouput_matrix(out,c);           // writes the matrix in File 
        }
        else if(a.compare("average")==0){
            vc2d abc = input_matrix(b);
            vc2d out = avg_pool(abc, atoi(argv[4]));
            ouput_matrix(out,c);
        }
        else{
        cerr << "Wrong arguments passed\n";
        }
    }

    else if(s.compare("probability")==0 && argc==5){
        string a = argv[2], b= argv[3], c= argv[4];
        if(a.compare("softmax")==0){
            vc1d abc = input_vector(b);       // Reads the input matrix from filename in var b
            vc1d out = softmax(abc);
            ouput_vector(out, c);              // writes the matrix in File
        }
        else if(a.compare("sigmoid")==0){
            vc1d abc = input_vector(b);       // Reads the input matrix from filename in var b
            vc1d out = sigmoid(abc);
            ouput_vector(out, c);             // writes the matrix in File
        }
        else{
        cerr << "Wrong arguments passed\n";
        }
        
    }

    else{
        cerr << "Wrong arguments passed\n";
    }
    return 0;
}


