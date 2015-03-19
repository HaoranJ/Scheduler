//
//  Simulator.h
//  Scheduler
//
//  Created by Haoran Jia on 2/26/15.
//  Copyright (c) 2015 Haoran Jia. All rights reserved.
//

#ifndef __Scheduler__Simulator__
#define __Scheduler__Simulator__

#include <cstdio>
#include <string>
#include <vector>
#include <fstream>
#include <queue>
#include <deque>
#include <cstdlib>

#include "Process.h"
#include "Scheduler.h"
#include "Event.h"

using namespace std;

class Simulator
{
public:
    int processTotalNum;
    int randTotalNum;
    int ofs;
    int lastTimeIdleCPU;
    int lastTimeIdleIO;
    int FT;
    int processLeft;
    int expiredLabel;
    double idleCT;
    double idleIO;
    double totalTT;
    double totalCW;
    double throughput;
    bool verbose;
    bool pickFlag;
    
    Scheduler * scheduler;
    
    vector<Process> processList;
    vector<Process> activeList;
    vector<Process> stableQueue;
    vector<int> randVals;
    
    deque<Event> readyQueue;
    vector<Event> eventQueue;

    int myrandom(int burst);
    
    Simulator();
    
    void InitProcessList(string inputFileName);
    void PrioDecrement(Event event);
    void InitRandValues(string randFileName);
    void Insert_eventQueue(int start, int end, int id, int transition);
    void InitScheduler(string schedulerType);
    void InitEventQueue();
    void InitReadyQueue();
    
    int GetMinEventNum(deque<Event> &readyQueue);
    
    void Run(string inputFileName, string randFileName, bool _verbose, string schedulerType);
    void beVerbose(Event event, Process& p, Process& pa, int io);
    void ProcessEvent(Event event);
    void Print();
    
};

#endif /* defined(__Scheduler__Simulator__) */
