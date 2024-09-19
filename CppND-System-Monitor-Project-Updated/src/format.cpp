#include <string>
#include <vector>
#include <cmath>
#include <iostream>

#include "format.h"

using std::string;
using std::vector;
using std::to_string;

//using namespace std;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds)    // calculates Uptime into { Hr:Min:Sec }
{
    long hr = seconds / 3600;
    seconds = seconds % 3600;
    long min = seconds / 60;
    long sec = seconds % 60;
    return to_string(hr) + ":" + to_string(min) + ":" + to_string(sec);
}