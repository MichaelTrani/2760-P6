/*  Author: Michael Trani
    May 2022
    Process Master      */

#include "p6.h"

static std::string logfile = "logfile";
static FILE* fptr;

int main(int argc, char* argv[]) {

    // FOR ERROR HANDLING
    std::string error_msg; // Errors
    std::string exe_name; // Name of this executable

    // Gets the executable name and removes "./"
    exe_name = argv[0];

    fptr = fopen(logfile.c_str(), "w+");

    int opt;
    while ((opt = getopt(argc, argv, "m:h")) != -1) {
        switch (opt) {
        case 'm':
            break;
        case 'h':
            std::cout << "Your program should take in a command line option -m x, which accepts either a 0 or a 1, which determines how child processes will perform their memory access\n";
            return 0;
        default:
            break;
        }
    }


    return 0;
}