#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <vector>

using namespace std;

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
   float prev_total_time = 0;
   float prev_active_time = 0;
};

#endif