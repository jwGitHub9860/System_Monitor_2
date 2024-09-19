#include <cmath>
#include <iostream>

#include "processor.h"
#include "linux_parser.h"
#include "process.h"

using namespace std;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization()
{
    long total_jiffies = LinuxParser::Jiffies();
    long active_jiffies = LinuxParser::ActiveJiffies();
    
    float current_total_time = total_jiffies;   // change long to float
    float current_active_time = active_jiffies;   // change long to float

    float total_time = current_total_time - prev_total_time;
    float active_time = current_active_time - prev_active_time;

    float prev_total_time = total_time;    // make Current total time, Previous total time
    float prev_active_time = active_time;    // make Current active time, Previous active time
    
    return (active_time / total_time) * total_time;     // CPU usage percent = (Active_CPU_time / Total_CPU_time) * 100         total_time = 100%
}