//Written by Martynas, so if u have any questions just ask.
#include "main.h"
#include <iostream>

// Calculate the time taken the CPU usage of the program.
 
void cpu_calc(void) {
  // Extract the initial CPU values when starting the program
  std::ifstream CPU_start("misc/cpu_start.dat");
  long int a, b, c, d, e, f, g;
  CPU_start >> a >> b >> c >> d >> e >> f >> g;
  CPU_start.close();

  // Extract the end CPU values after the program has finished running.
  std::ifstream CPU_end("misc/cpu_end.dat");
  long int h, i, j, k, l, m, n;
  CPU_end >> h >> i >> j >> k >> l >> m >> n;

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

  // Start time of the program
  std::ifstream time_start("misc/time_start.dat");
  double x;
  time_start >> x;
  time_start.close();

  // End time of the program
  std::ifstream time_end("misc/time_end.dat");
  double z;
  time_end >> z;
  time_end.close();

  // End memory of the program
  std::ifstream ramy("misc/memory.dat");
  double xz;
  ramy >> xz;
  ramy.close();

  std::cout << "Time used by the program is " << z-x << "s" << "\n";
  std::cout << "Memory used: " << (xz*100)/(40*1024*1024) << '%' << "\n";
  std::cout << "CPU usage: " << CPU_percent << '%' << "\n";
}

