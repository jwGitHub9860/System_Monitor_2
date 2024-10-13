#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <vector>

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
   float Prev_Idle_Total = 0;  // MUST BE INITIALIZED
   float Prev_Idle = 0;  // MUST BE INITIALIZED
   float Prev_Iowait = 0;  // MUST BE INITIALIZED
   
   float Prev_Active_Total = 0;  // MUST BE INITIALIZED
   float Prev_User = 0;  // MUST BE INITIALIZED
   float Prev_Nice = 0;  // MUST BE INITIALIZED
   float Prev_System = 0;  // MUST BE INITIALIZED
   float Prev_Irq = 0;  // MUST BE INITIALIZED
   float Prev_Softirq = 0;  // MUST BE INITIALIZED
   float Prev_Steal = 0;  // MUST BE INITIALIZED
   
   float Prev_Total = 0;  // MUST BE INITIALIZED
};

#endif