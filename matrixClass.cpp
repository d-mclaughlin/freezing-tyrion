//Main aim of class is to generate and store the matrix containing the relationships between sample points.

#include <vector>
#include <iostream>

class Matrix{

//Information held by class

private:
    //Number of sample points desired in x and y directions.
    int xNumber,yNumber;
    //Placeholder. Our final matrix a.
    std::vector<std::vector<int> > a;

//Used to input boundary conditions.
//Current design: input two vectors, each contains information about the boundary conditions.
//boundaryPos contains a tuple giving the position of a boundary.
//boundaryValue also contains a tuple - this time giving the value and type of a boundary.
//1 indicates Dirichlett boundary, 2 Von Neumann.
//e.g boundaryPos=[[0,0]],boundaryValue=[[10,1]] indicates a Dirichlett boundary at bottom left of grid.


//Very crude implementation. Pain if you want a whole region to have a certain value as each individual point must be defined.
//Thought about creating function to generate these two vectors automatically.
    std::vector<std::vector<int> > boundaryPos;
    std::vector<std::vector<float> > boundaryValue;

//Used to generate a. Returns 1 if current point should be Dirichlett boundary and 2 if Von Neumann.
    int isBoundary(int xCount,int yCount);

public:
//constructors
    Matrix (int xNum,int yNum);
    Matrix (int xNum,int yNum,std::vector<std::vector<int> > boundaryP,std::vector<std::vector<float> > boundaryV);
//Getter methods
    int getX();
    int getY();
    std::vector<std::vector<int> > getA();
    std::vector<std::vector<int> > getBoundaryPos();
    std::vector<std::vector<float> > getBoundaryValue();

    void viewVector();
};

int Matrix::getX(){return xNumber;}
int Matrix::getY(){return yNumber;}
  std::vector<std::vector<int> > Matrix::getA(){return a;}
  std::vector<std::vector<int> > Matrix::getBoundaryPos(){return boundaryPos;}
  std::vector<std::vector<float> > Matrix::getBoundaryValue(){return boundaryValue;}


//Uses hard coded boundary conditions. Using this until boundary input system complete.
Matrix::Matrix (int xNum,int yNum){
    xNumber = xNum;
    yNumber = yNum;

//Every point on grid occupies its own row in a. As there are y rows of x in the grid, total number of points is just xNumber*yNumber
    a=std::vector<std::vector<int> >(xNumber*yNumber,std::vector<int>(xNumber*yNumber));

    for(int y=1;y<=yNumber;y++){
        for(int x=1;x<=xNumber;x++){
            //Is the current point at a boundary?
            int boundary = isBoundary(x,y);

            //Index of the current point in the matrix 'a'
            int currentIndex = (x-1)+xNumber*(y-1);

//NOTE TO SELF: Maybe change and have boundary() check if Von Neumann at left or right. Could use "case" 1,2,3,4 here.

            //Dirichlett boundary
            if(boundary==1){a[currentIndex][currentIndex]=1;}
            //Von Neumann boundary
            else if(boundary==2){
                //Need to know if we are at the left or right hand boundary

                //Left
                if(x==1){
                    a[currentIndex][currentIndex]=-1;
                    a[currentIndex][currentIndex+1]=1;
                }

                //Right
                else if(x==xNumber){
                    a[currentIndex][currentIndex]=-1;
                    a[currentIndex][currentIndex-1]=1;
                }

            }
        //Interior points
            else{
                //Related to other points by "square" in grid.
                a[currentIndex][currentIndex]=-4;

                //Grid points to left and right
                a[currentIndex][currentIndex-1]=1;
                a[currentIndex][currentIndex+1]=1;

                //Grid points above and below.
                a[currentIndex][currentIndex+xNumber]=1;
                a[currentIndex][currentIndex-xNumber]=1;

            }
        }
    }


}


//TODO(mark): CHANGE TO USE INPUT BOUNDARY CONDITIONS
int Matrix::isBoundary(int xCount,int yCount){
    //Describes grid where top and bottom are Dirichlett boundaries and sides are Von Neumann boundaries
    if (yCount == 1 || yCount == yNumber){
        return 1;
    } else if (xCount == 1 || xCount == xNumber) {
        return 2;
    } else{
        return 0;
    }
}


//Test function. Outputs contents of given vector to the console
void Matrix::viewVector(){

    for(int row=0;row<a.size();row++){
        std::cout<<"Row "<<row+1<<"  ";
        for(int column=0;column<a.size();column++){
            std::cout<<a[row][column]<<"  ";
        }
        std::cout<<std::endl;
    }
}




//Test code

int main(int argc, char *argv[]){
    xNumber = atoi(argv[1]);
    yNumber = atoi(argv[2]);
    Matrix test = Matrix(xNumber, yNumber);
    std::vector<int> boundaries = std::vector<int> (xNumber * yNumber);

    for (int row=0; row < xNumber; row++) {
        for (int col=0; col < yNumber; col++) {
            // Initialise this element of product to 0
            product[xNumber][yNumber] = 0;            
            // Multiply the row of A by the column of B
            for (int inner=0; inner < yNumber; inner++) {
                product[row][col] += matrixA[row][inner] *
                                             matrixB[inner][col];
            }
        }
    }
    test.viewVector();
    boundaries.viewVector();

    for (int element=0; element < (xNumber * yNumber); element++) {
        std::cout << boundaries[element] << "\n";
    }
}
