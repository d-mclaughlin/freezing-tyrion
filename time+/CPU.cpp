//Written by Martynas, so if u have any questions just ask.
#include "main.h"
using namespace std;

double CPUCalc()
 
{

  //Extracting the initial CPU values whne starting the program
  fstream myfile("CPUStart.dat", ios_base::in);
  long int a, b, c, d, e, f, g;
 myfile >> a >> b >> c >> d >> e >> f >>g;
 //cout<< "START " << a <<" "<< b << " " << c << " " << d << " "<< e<< " " << f << " " << g<<endl;
 myfile.close();


 //Extracting the end CPU values after the program has finished running.
fstream myfile2("CPUEnd.dat", ios_base::in);
 long int h, i, j, k, l, m, n;
 myfile2 >> h >> i >> j >> k >> l >> m >>n;
 //cout<<"END " << h <<" "<< i << " " << j << " " << k << " "<< l<< " " << m << " " << n<<endl;
 myfile2.close();


 long int total_start, total_end, work_start, work_end;
 double CPU_percent, work_over_time, total_over_time;


 //Calculating the percentage of CPU used
 total_start=a+b+c+d+e+f+g;
 total_end=h+i+j+k+l+m+n;
 work_start=a+b+c;
 work_end=h+i+j;

 total_over_time= total_end -total_start;
 work_over_time= work_end- work_start;

 CPU_percent= 100 * (work_over_time/total_over_time);


 //the starting time of the program
fstream myfile3("time_Start.dat", ios_base::in);
double x;
 myfile3 >> x;
 myfile3.close();

 //end time of the program
fstream myfile4("time_End.dat", ios_base::in);
 double z;
 myfile4 >> z;
 myfile4.close();




 cout <<"Time used by the program is " << z-x << "s"<<endl;
 cout <<"CPU used "<< CPU_percent<<"%"<<endl;
 return CPU_percent;
}

