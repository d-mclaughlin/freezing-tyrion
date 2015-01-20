#include <iostream>
#include <vector>
#include <math.h>

std::vector< std::vector<float> > inputMatrix(int n);

//Test function. Prints out contents of a "vector of float vectors".
void viewVector(std::vector< std::vector<float> > A);

//Returns cofactor matrix for element in the given position of matrix A.
std::vector< std::vector<float> > coMatrix(std::vector< std::vector<float> > A, int i, int j);

//Returns factor for a cofactor matrix. Based on the element and its position in a matrix.
float coFactor(int element, int i, int j);

//Returns the determinant of a given matrix
float determinant(std::vector< std::vector<float> > A);

//Returns the transpose of a.
std::vector< std::vector<float> > transpose(std::vector< std::vector<float> > a);

//Returns the inverse matrix of a.
std::vector< std::vector<float> > inverse(std::vector< std::vector<float> > a);

int main(){
  using namespace std;

  int matrixSize=0;
  cout<<"Enter dimension of square matrix"<<endl;
  cin>>matrixSize;

  std::vector< std::vector<float> > a= inputMatrix(matrixSize);

  viewVector(inverse(a));

}

                         //INPUTMATRIX


std::vector< std::vector<float> > inputMatrix(int n){
  std::vector< std::vector<float> > matrix;
  matrix = std::vector< std::vector<float> >(n,std::vector<float>(n));

  for(int row=0;row<n;row++){
    for(int column=0;column<n;column++){
      std::cout<<"Enter value for row "<<row+1<<" column "<<column+1<<std::endl;
      std:: cin>>matrix[row][column];
}

}
    return matrix;

}

                             //VIEWVECTOR


void viewVector(std::vector< std::vector<float> > A){
  for(int row=0;row<A.size();row++){
    std::cout<<"Row "<<row+1<<"  ";
    for(int column=0;column<A.size();column++){
      std::cout<<A[row][column]<<"  ";
}
    std::cout<<std::endl;
}
}


                               //COMATRIX


std::vector< std::vector<float> > coMatrix(std::vector< std::vector<float> > A, int i, int j){

  std::vector< std::vector<float> > output;



  for(int row=0;row<A.size();row++){

    if(row!=i){
    //Reference to current row being added to
      std::vector<float> currentVector;

    for(int column=0;column<A.size();column++){
      //Skips any element in the excluded column
      if(column!=j){
	//Adds element to next free space in the row vector
	float element=A[row][column];
	currentVector.push_back(element);}
    }
    output.push_back(currentVector);}
}
  return output;
}

                
                       //COFACTOR



float coFactor(float element, int i, int j){

  //Output
  float baseToPower=-1;
  
  //Indexes in our program are numbered from zero, but assumed to be numbered from 1 in the the relationship (-1)^(i+j)
  //To account for this, 2 is added. One to shift i and the other for j.
  int power = i+j+2;
  float base = -1;

  for(int count = 1;count<power;count++){
    baseToPower=baseToPower*base;
}
  return baseToPower*element;
}


                       //DETERMINANT


float determinant(std::vector< std::vector<float> > a){
  float total=0;
  int aSize = a.size();

  //Terminating condition
  if(aSize==1){return a[0][0];}

  
  for(int i=0;i<a.size();i++){
    std::vector< std::vector<float> > currentCoimh;
    float currentElement=a[0][i];
    float factor;

    //Just always goes along the 0th row. Can optimise this but felt was beyond scope of the task
    currentCoimh=coMatrix(a,0,i);
    factor = coFactor(currentElement,0,i);

    total+= factor*determinant(currentCoimh);
}
  return total;
}

                     
                            //TRANSPOSE

std::vector< std::vector<float> > transpose(std::vector< std::vector<float> > a){
  
  int  n = a.size();
  std::vector< std::vector<float> > output;
  output = std::vector< std::vector<float> >(n,std::vector<float>(n));


  for(int row=0;row<n;row++){
    for(int column=0;column<n;column++){
      float element = a[row][column];
      output[column][row]=element;

}
}
  return output;

}

                          //INVERSE

std::vector< std::vector<float> > inverse(std::vector< std::vector<float> > a){
  using namespace std;

  //Our output
  std::vector< std::vector<float> > inverseMatrix;

  int matrixSize=a.size();
  float oneOverDet = 1.0/determinant(a);


  //Construct matrix made up of dets of cofactor matrices.
  for(int row=0;row<matrixSize;row++){

    //Blank row to be filled each iteration. Added to final output
    std::vector<float> emptyRow;


    //For every element in the input:
    //(1)Finds it's coMatrix
    //(2)Takes the determinant of the coMatrix.
    //(3)Multiplies it by inverse of determinant of input matrix a.
    //(4)Adds result to empty row.
    for(int column=0;column<matrixSize;column++){
      float element = a[row][column];
      std::vector< std::vector<float> > coimh=coMatrix(a,row,column);

      //Multiplying through by 1/det(A) as we go along. Saves having to do it later
      std::cout<<"det: "<<determinant(coimh)<<" factor: "<<oneOverDet<<std::endl;

      float inverseElement=determinant(coimh)*oneOverDet;

      emptyRow.push_back(inverseElement);

}
    inverseMatrix.push_back(emptyRow);
  }

  std::vector< std::vector<float> >  aInverse=transpose(inverseMatrix);
  return aInverse;
}
