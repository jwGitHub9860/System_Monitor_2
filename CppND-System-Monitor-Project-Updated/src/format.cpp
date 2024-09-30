#include <string>
#include <vector>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>

#include "format.h"

using std::string;
using std::vector;
using std::to_string;
using std::ostringstream;
using std::setw;
using std::setfill;

//using namespace std;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds)    // calculates Uptime into { Hr:Min:Sec }
{
    int hours = seconds / 3600;
    seconds %= 3600;
    int minutes = seconds / 60;
    seconds %= 60;
    ostringstream stream;
    stream << setw(2) << setfill('0') << hours << ":"
            << setw(2) << setfill('0') << minutes << ":"
            << setw(2) << setfill('0') << seconds;
    return stream.str();
}