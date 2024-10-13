#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

//using namespace std;

Process::Process(int pid) : pid_(pid) {}     // constructor (initializer list) ---> DEFINES SPECIFIC "int pid" OF SPECIFIC PROCESS

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization()
{
    long process_uptime = LinuxParser::UpTime(pid_);
    long process_jiffies = LinuxParser::ActiveJiffies(pid_);
    long total_uptime = LinuxParser::UpTime();
    utilization_ = static_cast<float>(process_jiffies) / static_cast<float>(total_uptime - process_uptime);     // "static_cast<float>()" is SAFER than "float()"
    return utilization_;
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime() - LinuxParser::UpTime(pid_); }    // Age of process & Uptime of process are NOT the SAME

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { return this->ram_usage < a.ram_usage; }