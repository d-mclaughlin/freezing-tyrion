//Written by Martynas, so if u have any questions just ask.
#include "main.h"
#include <iostream>

// Calculate the time taken the CPU usage of the program.
 
void cpu_calc(void) {
  // Extract the initial CPU values when starting the program
  std::fstream CPU_start("misc/cpu_start.dat", std::ios_base::in);
  long int a, b, c, d, e, f, g;
  CPU_start >> a >> b >> c >> d >> e >> f >> g;
  CPU_start.close();

  // Extract the end CPU values after the program has finished running.
  std::fstream CPU_end("misc/cpu_end.dat", std::ios_base::in);
  long int h, i, j, k, l, m, n;
  CPU_end >> h >> i >> j >> k >> l >> m >>n;

  CPU_end.close();

  long int total_start, total_end, work_start, work_end;
  float CPU_percent, work_over_time, total_over_time;

  // Calculate the percentage of CPU used
  total_start = a+b+c+d+e+f+g;
  total_end   = h+i+j+k+l+m+n;
  work_start  = a+b+c;
  work_end    = h+i+j;

  total_over_time = total_end - total_start;
  work_over_time = work_end - work_start;

  CPU_percent = 100 * (work_over_time / total_over_time);

  //the starting time of the program
  std::fstream time_start("misc/time_start.dat", std::ios_base::in);
  double x;
  time_start >> x;
  time_start.close();

   //end time of the program
  std::fstream time_end("misc/time_end.dat", std::ios_base::in);
  double z;
  time_end >> z;
  time_end.close();

  std::cout << "Time used by the program is " << z-x << "s" << "\n";
  std::cout << "CPU usage: " << CPU_percent << '%' << "\n";
}

