#include <cmath>
#include <iostream>

#include "processor.h"
#include "linux_parser.h"
#include "process.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization()
{
    // CANNOT use "LinuxParser::Jiffies() & LinuxParser::ActiveJiffies()" because Previous values are needed

    Prev_Idle_Total = Prev_Idle + Prev_Iowait;
    float current_Idle = LinuxParser::kIdle_ + LinuxParser::kIOwait_;
    Prev_Idle = LinuxParser::kIdle_;    // make Current active time, Previous active time
    Prev_Iowait = LinuxParser::kIOwait_;    // make Current active time, Previous active time
    
    Prev_Active_Total = Prev_User + Prev_Nice + Prev_System + Prev_Irq + Prev_Softirq + Prev_Steal;
    float current_Active = LinuxParser::kUser_ + LinuxParser::kNice_ + LinuxParser::kSystem_ + LinuxParser::kIRQ_ + LinuxParser::kSoftIRQ_ + LinuxParser::kSteal_;
    Prev_User = LinuxParser::kUser_;    // make Current active time, Previous active time
    Prev_Nice = LinuxParser::kNice_;    // make Current total time, Previous total time
    Prev_System = LinuxParser::kSystem_;    // make Current total time, Previous total time
    Prev_Irq = LinuxParser::kIRQ_;    // make Current total time, Previous total time
    Prev_Softirq = LinuxParser::kSoftIRQ_;    // make Current total time, Previous total time
    Prev_Steal = LinuxParser::kSteal_;    // make Current total time, Previous total time

    Prev_Total = Prev_Idle_Total + Prev_Active_Total;
    float current_Total = current_Active - current_Idle;

    float Total = current_Total - Prev_Total;
    float Idle = current_Idle - Prev_Idle_Total;
    
    return (Total - Idle) / Total;     // CPU usage percent = (Total_CPU_time - Idle_CPU_time) / Total_CPU_time
}