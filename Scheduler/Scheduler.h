//
//  Scheduler.h
//  Scheduler
//
//  Created by Haoran Jia on 2/26/15.
//  Copyright (c) 2015 Haoran Jia. All rights reserved.
//

#ifndef __Scheduler__Scheduler__
#define __Scheduler__Scheduler__

#include <stdio.h>
#include <string>
#include <deque>
#include <vector>

#include "Event.h"
#include "Process.h"

using namespace std;

class Scheduler
{
public:
    string type;
    int quantum;
    
    Scheduler();
    
    virtual Event Pick(deque<Event>& readydeque, vector<Process>& processList);
    
    Event PickFirst(deque<Event> &readyQueue, vector<Process>& processList);
};

class FCFS : public Scheduler
{
public:
    FCFS();
    
    Event Pick(deque<Event>& readydeque, vector<Process>& processList);
};

class LCFS : public Scheduler
{
public:
    LCFS();
    
    Event Pick(deque<Event>& readydeque, vector<Process>& processList);
};

class SJF : public Scheduler
{
public:
    SJF();
    
    Event Pick(deque<Event>& readydeque, vector<Process>& processList);
};

class RR : public Scheduler
{
public:
    RR(int _quantum);
    
    Event Pick(deque<Event>& readydeque, vector<Process>& processList);
};

class PRIO : public Scheduler
{
public:
    PRIO(int _quantum);
    
    Event Pick(deque<Event>& readydeque, vector<Process>& processList);
};
#endif /* defined(__Scheduler__Scheduler__) */
