#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);     // constructor ---> DEFINES SPECIFIC "int pid" OF SPECIFIC PROCESS
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
 private:
  int pid_;
  
  int process_ID;

  float prev_active_jiffies = 0;  // MUST BE INITIALIZED
  float prev_idle_jiffies = 0;  // MUST BE INITIALIZED
  float prev_User = 0;  // MUST BE INITIALIZED
  float prev_Nice = 0;  // MUST BE INITIALIZED
  float prev_System = 0;  // MUST BE INITIALIZED
  float prev_Idle = 0;  // MUST BE INITIALIZED
  float prev_Iowait = 0;  // MUST BE INITIALIZED
  float prev_Irq = 0;  // MUST BE INITIALIZED
  float prev_Softirq = 0;  // MUST BE INITIALIZED

  int ram_usage;
};

#endif