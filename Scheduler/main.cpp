//
//  main.cpp
//  Scheduler
//
//  Created by Haoran Jia on 2/26/15.
//  Copyright (c) 2015 Haoran Jia. All rights reserved.
//

#include <iostream>
#include <unistd.h>
#include <string>

#include "Simulator.h"

using namespace::std;

int main(int argc, char * argv[])
{
    int c;
    bool verbose = false;
    string sValue = "";
    
    
    //e.g ./Simulator -v -sR5 inputfile rfile
    while ((c = getopt (argc, argv, "vs:")) != -1)
    {
        if (c == 'v')
        {
            verbose = true;
        }
        else if (c == 's')
        {
            sValue = optarg;
        }
    }
    
    string inputFileName = argv[optind];
    string randFileName = argv[optind + 1];
    
    Simulator simulator;
    simulator.Run(inputFileName, randFileName, verbose, sValue);
    simulator.Print();
    
    return 0;
}