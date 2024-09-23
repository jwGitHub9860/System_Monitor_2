#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::ifstream;
using std::istringstream;

//using namespace std;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization()
{
  float total_memory, free_memory;
  string key, line;
  float value;
  ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open())
  {
    while (getline(stream, line))  // read in line; while loop used due to multiple lines
    {
      istringstream linestream(line);    // creates string stream from "line"
      linestream >> key >> value;  // allows to pull tokens off stream     1st token - key     2nd token - value
      if (key == "MemTotal:")  // check if "line" holds cpu info (line = cpu / cpu...#)
      {
        total_memory = value;
      }
      else if (key == "MemFree:")
      {
        free_memory = value;
      }
    }
  }
  return (total_memory - free_memory) / (total_memory * 100);
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() 
{
  long uptime;  // 1ST LARGE VALUE    { 1st value -> system uptime 2nd value -> system idle time} <---(in Cmake)
  string line;
  ifstream stream(kProcDirectory + kUptimeFilename);  // input file stream from path for operating system kernel version          operating system kernel version - "proc directory + version file name"
  if (stream.is_open())
  {
    getline(stream, line);   // gets line from stream & stores it in "string line"
    istringstream linestream(line);    // creates string stream from "line"
    linestream >> uptime;  // allows to pull tokens off stream     1st token - uptime     2nd token - idle_time    78322.97 1119670.94 <---(in Cmake)
  }
  return uptime;  // if opening string or something else fails, return "uptime" as Blank String Default     MUST CHANGE "uptime" from 'string' to 'long' LAST or error will be returned
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies()   // TOTAL JIFFIES CPU executed
{
  long jiffies;
  vector<string> CPU_Info = CpuUtilization();   // inputs values into vector
  for (auto i : CPU_Info)    // iterates through entire /proc/stat file
  {
    jiffies += stol(i);   // JIFFIES ARE THE LARGE NUMBERS
  }
  return jiffies;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid)
{
  vector<string> jiffies;
  string value, line;
  ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);  // input file stream from path for operating system kernel version          operating system kernel version - "proc directory + pid + stat file name"

  if (stream.is_open())
  {
    getline(stream, line);   // gets line from stream & stores it in "string line"
    istringstream linestream(line);    // creates string stream from "line"
    for (int i = 0; i < 17; i++)
    {
      linestream >> value;  // allows to pull tokens off stream     first token - value
      jiffies.emplace_back(value);
    }
    return stol(jiffies[16]) + stol(jiffies[15]) + stol(jiffies[14]) + stol(jiffies[13]);  // for '/proc/[PID]/stat', Active Jiffies = cstime + cutime + stime + utime    Active Jiffies = index[16] + index[15] + index[14] + index[13]
  }
  return 0;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies()
{
  vector<string> jiffies = CpuUtilization();
  return stol(jiffies[kUser_]) + stol(jiffies[kNice_]) + stol(jiffies[kSystem_]) + stol(jiffies[kIRQ_]) + stol(jiffies[kSoftIRQ_]) + stol(jiffies[kSteal_]);  // Active Jiffies = user + nice + system + irq + softirq + steal +      ONLY NEED STATES PERTAINING TO CPU ITSELF (NOT GUEST)
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies()
{
  vector<string> jiffies = CpuUtilization();  // obtains "jiffies" from CpuUtilization() function
  return stol(jiffies[kIdle_]) + stol(jiffies[kIOwait_]);  // Idle Jiffies = idle + iowait
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization()  // finds lines that hold cpu info & inputs values into vector
{
  vector<string> jiffies;   // holds jiffies (Large Values)
  string line, value;
  ifstream stream(kProcDirectory + kStatFilename);  // input file stream from path for operating system kernel version          operating system kernel version - "proc directory + stat file name"
  if (stream.is_open())
  {
    getline(stream, line);   // gets line from stream & stores it in "string line"
    istringstream linestream(line);    // creates string stream from "line"

    while (linestream >> value)  // allows to pull tokens off stream     first token - value     pulls off numbers in cpu line, NOT cpu string; while loop used due to multiple lines     INCLUDING "cpu" ID
    {
      if (value != "cpu")   // checks if "value" holds numbers NOT "cpu / cpu...#" text     
      {
        jiffies.emplace_back(value);
      }
    }
  }
  return jiffies;  // if opening string or something else fails, return "jiffies" vector as Blank Vector String Default     MUST CHANGE "uptime" from 'string' to 'long' LAST or error will be returned
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses()
{
  int total_processes;
  string line;
  string key;
  ifstream stream(kProcDirectory + kStatFilename);  // input file stream from path for operating system kernel version          operating system kernel version - "proc directory + stat file name"
  if (stream.is_open())
  {
    while (getline(stream, line))   // gets line from stream & stores it in "string line"
    {
      istringstream linestream;  // input string stream
      while (linestream >> key >> total_processes)   // gets line from stream & stores it in "string line"     first token - processes    second token - " # "    78322.97 1119670.94 <---(in Cmake)
      {
        if (key == "processes")   // checks if line contains total number of processes
        {
          return total_processes;
        }
      }
    }
  }
  return total_processes;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses()
{
  int num_of_run_process;
  string line;
  string key;
  ifstream stream(kProcDirectory + kStatFilename);  // input file stream from path for operating system kernel version          operating system kernel version - "proc directory + stat file name"
  if (stream.is_open())
  {
    while (getline(stream, line))   // gets line from stream & stores it in "string line"
    {
      istringstream linestream(line);  // input string stream
      while (linestream >> key >> num_of_run_process)   // allows to pull tokens off stream     first token - procs_running    second token - " # "    78322.97 1119670.94 <---(in Cmake)
      {
        if (key == "procs_running")   // checks if line contains number of processes running
        {
          return num_of_run_process;
        }
      }
    }
  }
  return num_of_run_process;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid)
{
  string cmdline;
  ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);  // input file stream from path for operating system kernel version          operating system kernel version - "proc directory + pid + cmdline file name"
  if (stream.is_open())
  {
    getline(stream, cmdline);   // gets cmdline from stream & stores it in "string line"
    stream.close();
    return cmdline;
  }
  
  return string();
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid)
{
  string line, key, value;
  ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);  // input file stream from path for operating system kernel version          operating system kernel version - "proc directory + pid + status file name"
  if (stream.is_open())
  {
    while (getline(stream, line))  // gets line from stream & stores it in "string line"; while loop used due to multiple lines
    {
      istringstream linestream(line);  // input string stream
      while (linestream >> key >> value)  // allows to pull tokens off stream     1st token - key     2nd token - value
      {
        if (key == "VmRSS")   // checks if "key" contains 'VmRSS' (process RAM memory usage)      'VmSize' - total process VIRTUAL memory
        {
          return value;
        }
      }
    }
  }
  return value;  // if opening string or something else fails, return "value" as Blank String Default
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid)
{
  string line, username, password, user_ID;
  ifstream stream(kPasswordPath);  // input file stream from path for operating system kernel version          operating system kernel version - "password path directory"
  if (stream.is_open())
  {
    getline(stream, line);   // gets line from stream & stores it in "string line"
    istringstream linestream(line);  // input string stream
    linestream >> username >> password >> user_ID;  // allows to pull tokens off stream     1st token - username     2nd token - password     3rd token - user_ID
  }
  return user_ID;  // if opening string or something else fails, return "user_ID" as Blank String Default
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid)
{
  string line, username, password, user_ID, group_ID, gecos, home_directory, login_system;    // Must include ALL or user_ID will include user_ID, group_ID, gecos, home_directory, login_system
  string Uid = LinuxParser::Uid(pid);   // function that returns USER
  ifstream stream(kPasswordPath);  // input file stream from path for operating system kernel version          operating system kernel version - "password path directory"
  if (stream.is_open())
  {
    while (getline(stream, line))  // read in line; while loop used due to multiple lines
    {
      replace(line.begin(), line.end(), ':', ' ');
      istringstream linestream(line);  // input string stream
      while (linestream >> username >> password >> user_ID >> group_ID >> gecos >> home_directory >> login_system)  // allows to pull tokens off stream     1st token - username     2nd token - password     3rd token - user_ID     4th token - group_ID     5th token - gecos     6th token - home_directory     7th token - login_system
      {
        if (user_ID == Uid)  // checks if "user_ID" matches "Uid"
        {
          return username;
        } 
      }
    }
  }
  return username;  // if opening string or something else fails, return "username" as Blank String Default
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid)
{
  long uptime;
  string line;
  ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);  // input file stream from path for operating system kernel version          operating system kernel version - "proc directory + pid + STAT file name"          use Same stream as "TotalProcesses()" & "RunningProcesses()" since both deal with processes
  if (stream.is_open())
  {
    getline(stream, line);   // gets line from stream & stores it in "string line"
    istringstream linestream(line);  // input string stream
    for (int i = 0; i < 22; i++)  // iterates through all lines until 'line 21' holds "processes" line - number of PROCESSES & threads created
    {
      linestream >> uptime;  // allows to pull tokens off stream     1st token - uptime     2nd token - idle_time    78322.97 1119670.94 <---(in Cmake Example)
    }
    return uptime / sysconf(_SC_CLK_TCK);   // sysconf(_SC_CLK_TCK) - returns number of clock ticks per second, amount of time measured in USER_HZ
  }
  return uptime;  // if opening long int or something else fails, return "uptime" as Blank Long Int Default
}
