#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
using std::less;

//using namespace std;

// TODO: Return the system's CPU
Processor& System::Cpu() { return this->cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes()
{
    processes_.clear();
    vector<int> process_Pids = LinuxParser::Pids();     // holds ALL process ID's
    for (auto i : process_Pids)    // iterates through ALL process ID's
    {
        Process process_(i);     // constructor
        process_.CpuUtilization();
        process_.UpTime();
        processes_.emplace_back(process_);
    }
    sort(processes_.begin(), processes_.end());      // less<> - boolean using "<" for POINTERS      type of "less<>()" MUST BE 'Process' because vector<type> is 'Process'
    return processes_;
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return string(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return 0.0; }

// TODO: Return the operating system name
std::string System::OperatingSystem() { return string(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return 0; }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return 0; }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return 0; }