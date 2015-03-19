//
//  Simulator.cpp
//  Scheduler
//
//  Created by Haoran Jia on 2/26/15.
//  Copyright (c) 2015 Haoran Jia. All rights reserved.
//

#include "Simulator.h"
#include <iostream>
#include <algorithm>

using namespace std;

Simulator::Simulator()
{
    processTotalNum = 0;
    randTotalNum = 0;
    ofs = 0;
    lastTimeIdleCPU = 0;
    lastTimeIdleIO =0;
    FT = 0;
    processLeft = 0;
    idleCT = 0;
    idleIO = 0;
    totalTT = 0;
    totalCW = 0;  //time in Ready
    throughput = 0;
    verbose = false;
    pickFlag = true;
    expiredLabel = 0;
    
}

int Simulator::myrandom(int burst)
{
    if (ofs == randTotalNum) {
        ofs = 0;
    }
    return 1 + randVals[ofs++] % burst;
}

void Simulator::InitProcessList(string inputFileName)
{
    ifstream readFile;
    int AT, TC, CB, IO;
    int c = 0;
        
    readFile.open(inputFileName.c_str());
    while (readFile >> AT >> TC >> CB >> IO) {
        ++processTotalNum;
        processList.push_back(*new Process(AT, TC, CB, IO, 0, c));
        activeList.push_back(*new Process(AT, TC, CB, IO, 0, c));
        c++;
    }
    processLeft = processTotalNum;
    readFile.close();
}

void Simulator::PrioDecrement(Event event)
{
    activeList[event.pid].dynamicPriority --;
    activeList[event.pid].expiredOrder = ++expiredLabel;
    if (activeList[event.pid].dynamicPriority > -1) {
        readyQueue.push_back(*new Event(event.endTime, event.endTime, event.pid, 1));
    }
}
void Simulator::InitRandValues(string randFileName)
{
    ifstream readFile;
    int rand;
    
    readFile.open(randFileName.c_str());
    readFile >> randTotalNum;
    
    while (readFile >> rand) {
        randVals.push_back(rand);
    }
    
    for (int i = 0; i < processTotalNum; ++i) {
        processList[i].staticPriority = myrandom(4)-1;
        activeList[i].staticPriority = processList[i].staticPriority;
        activeList[i].dynamicPriority = processList[i].staticPriority;
    }
}

void Simulator::InitScheduler(string schedulerType)
{
    if (schedulerType == "F") { scheduler = new FCFS(); }
    if (schedulerType == "L") { scheduler = new LCFS(); }
    if (schedulerType == "S") { scheduler = new SJF(); }
    if (schedulerType[0] == 'R')
    {
        string qString = schedulerType.substr(1, schedulerType.length() - 1);
        int quantum = 0;
        for (int i=0; i<qString.length(); ++i) {
            quantum = quantum * 10 + qString[i] - '0';
        }
        scheduler = new RR(quantum);
    }
    if (schedulerType[0] == 'P')
    {
        string qString = schedulerType.substr(1, schedulerType.length() - 1);
        int quantum = 0;
        for (int i=0; i<qString.length(); ++i) {
            quantum = quantum * 10 + qString[i] - '0';
        }
        scheduler = new PRIO(quantum);
    }
}

void Simulator::Insert_eventQueue(int start, int end, int id, int transition)
{
    eventQueue.push_back(*new Event(start, end, id, transition));
    int n = (int)eventQueue.size();
    for (int i = n-1; i > 0 ; --i) {
        if (eventQueue[i-1] < eventQueue[i]) {
            Event temp(0, 0, 0, 0);
            temp = eventQueue[i]; eventQueue[i] = eventQueue[i-1]; eventQueue[i-1] = temp;
        } else {
            break;
        }
    }

}

void Simulator::InitEventQueue() //Created --> Ready
{
    for (int i=0; i < processTotalNum; ++i) {
        Insert_eventQueue(processList[i].AT, processList[i].AT, i, -1);
    }
}

void Simulator::Run(string inputFileName, string randFileName, bool _verbose, string schedulerType)
{
    verbose = _verbose;
    InitProcessList(inputFileName);
    InitRandValues(randFileName);
    InitScheduler(schedulerType);
    InitEventQueue(); //created --> ready  eventQueue
    
    if(scheduler->type != "PRIO") { //for FCFS, LCFS, SJF and RR
        while (true)
        {
            if (pickFlag && !readyQueue.empty())
            {
                if (eventQueue.empty() || eventQueue.front() < scheduler->PickFirst(readyQueue, processList))
                {
                    Event event = scheduler->Pick(readyQueue, processList);
                    if (event.endTime > lastTimeIdleCPU) {
                        idleCT += event.endTime - lastTimeIdleCPU;
                        lastTimeIdleCPU = event.endTime;
                    }
                    pickFlag = false;
                    Insert_eventQueue(event.endTime, lastTimeIdleCPU, event.pid, 1);
                }
            }
            if (!eventQueue.empty()) {
                ProcessEvent(eventQueue.front());
                eventQueue.erase(eventQueue.begin());
            }
            else{ break; }
        }
    }
    if (scheduler->type == "PRIO") {
        while (true) {
            if (pickFlag) {
                if (readyQueue.empty()) {
                    for (vector<Process>::iterator it = activeList.begin(); it != activeList.end(); ++it) {
                        if (it->dynamicPriority == -1 && it->TC > 0) {
                            it->dynamicPriority = it->staticPriority;
                            if (it->isBlocked != 1) {
                                stableQueue.push_back(* it);
                            }
                        }
                    }
                    stable_sort(stableQueue.begin(), stableQueue.end());
                    for (vector<Process>::iterator j = stableQueue.begin(); j != stableQueue.end(); ++j) {
                        readyQueue.push_back(* new Event(j->lastReady, j->lastReady, j->processID, 1));
                    }
                    stableQueue.clear();
                }
                if (!readyQueue.empty()) {
                    if (eventQueue.empty() || eventQueue.front() < scheduler->PickFirst(readyQueue, processList))
                    {
                        Event event = scheduler->Pick(readyQueue, activeList);
                        if (event.endTime > lastTimeIdleCPU) {
                            idleCT += event.endTime - lastTimeIdleCPU;
                            lastTimeIdleCPU = event.endTime;
                        }
                        pickFlag = false;
                        Insert_eventQueue(event.endTime, lastTimeIdleCPU, event.pid, 1);
                    }
                }
            }
            if(!eventQueue.empty()){
                ProcessEvent(eventQueue.front());
                eventQueue.erase(eventQueue.begin());
            }else { break; }
        }
    }
}

void Simulator::beVerbose(Event event, Process& p, Process& pa, int io)
{
    printf("%d %d %d:", event.endTime, event.pid, event.endTime - event.startTime);
    switch (event.status) {
        case -1:
        {   cout<<" CREATED -> READY\n"; break; }
        case 1:
        {   cout<<" READY -> RUNNG ";
            printf("cb=%d rem=%d prio=%d\n", p.remainCB, p.TC, pa.dynamicPriority);
            break; }
        case 2:
        {   cout<<" RUNNG -> BLOCK ";
            printf("ib=%d rem=%d prio=%d blocked=%d\n", io, pa.TC, pa.dynamicPriority, activeList[event.pid].isBlocked);
            break; }
        case 3:
        {   cout<<" RUNNG -> READY ";
            printf("cb=%d rem=%d prio=%d\n", p.remainCB, p.TC, pa.dynamicPriority);
            break; }
        case 0:
        {   printf(" BLOCK -> READY\n"); break; }
        default:
            break;
    }

}
void Simulator::ProcessEvent(Event event)
{
    Process& p = processList[event.pid];
    Process& pa = activeList[event.pid];
    
    //created -> ready
    if (event.status == -1) {
        readyQueue.push_back(*new Event(event.startTime, event.endTime, event.pid, 1));
        if (verbose) { beVerbose(event, p, pa, 0); }
    }
    
    //blocked --> ready
    if (event.status == 0)
    {
        p.IT += event.endTime - event.startTime;  //time in Blocked
        if (scheduler->type == "PRIO") {
            pa.isBlocked = 0;
            pa.dynamicPriority = pa.staticPriority;
            pa.lastReady = event.endTime;
            readyQueue.push_back(*new Event(event.endTime, event.endTime, event.pid, 1));
        }
        else{
            readyQueue.push_back(*new Event(event.endTime, event.endTime, event.pid, 1));
        }
        if (verbose) { beVerbose(event, p, pa, 0); }
    }
    
    //ready --> running
    if (event.status == 1) {
        p.CW += event.endTime - event.startTime;  // time in Ready
        if (scheduler->type == "FCFS" || scheduler->type =="LCFS" || scheduler->type =="SJF" )
        {
            int newCB = myrandom(p.CB);
            if (newCB > p.TC) {
                newCB = p.TC;
            }
            Insert_eventQueue(event.endTime, event.endTime + newCB, event.pid, 2);
            //clock = event.endTime + newCB;
        }
        if (scheduler->type == "RR")
        {
            if (p.remainCB == 0)
            {
                p.remainCB = myrandom(p.CB);
                if (p.remainCB > p.TC) { p.remainCB = p.TC; }
            }
            
            if (p.remainCB <= scheduler->quantum)
            {
                Insert_eventQueue(event.endTime, event.endTime + p.remainCB, event.pid, 2 );  //running -> blocked
            }
            else{
                //preemptive, remainCB > 0,
                Insert_eventQueue(event.endTime, event.endTime + scheduler->quantum, event.pid, 3 );  //running -> ready
            }
        }

        if (scheduler->type == "PRIO")
        {
            if (p.remainCB == 0)
            {
                p.remainCB = myrandom(p.CB);
                if (p.remainCB > p.TC) { p.remainCB = p.TC; }
            }
            
            if (p.remainCB <= scheduler->quantum)
            {
                Insert_eventQueue(event.endTime, event.endTime + p.remainCB, event.pid, 2 );  //running -> blocked
            }
            else{
                //preemptive, remainCB > 0,
                Insert_eventQueue(event.endTime, event.endTime + scheduler->quantum, event.pid, 3 );  //running -> ready
            }
        }
        if (verbose) { beVerbose(event, p, pa, 0); }
    }
    
    // running --> blocked  ||  running --> ready
    if (event.status == 2 || event.status == 3)
    {
        p.TC -= event.endTime - event.startTime; //Total CPU time
        if (scheduler->type == "PRIO") { pa.TC -= event.endTime - event.startTime; }
        pickFlag = true;
        lastTimeIdleCPU = event.endTime;
        if (p.TC == 0) {
            p.FT = event.endTime;
            p.TT = p.FT - p.AT;
            if (verbose) {
                printf("%d %d %d: done\n", event.endTime, event.pid, event.endTime - event.startTime);
            }
            return;
        }
        
        if (scheduler->type == "RR" || scheduler->type == "PRIO") {
            p.remainCB -= event.endTime - event.startTime;
        }
        
        //2: running -> block
        if (p.remainCB == 0)
        {
            int rand = myrandom(p.IO);
            Insert_eventQueue(event.endTime, event.endTime + rand, event.pid, 0);
            pa.isBlocked = 1;
            
            if (lastTimeIdleIO < event.endTime)
            {
                idleIO += event.endTime - lastTimeIdleIO;
            }
            
            if (lastTimeIdleIO < event.endTime + rand)
            {
                lastTimeIdleIO = event.endTime + rand;
            }
            if (verbose) { beVerbose(event, p, pa, rand); }
        }
        else //3: running -> ready, a quantum expired
        {
            pa.lastReady = event.endTime;
            if(scheduler->type == "PRIO"){ PrioDecrement(event); }
            else { readyQueue.push_back(*new Event(event.endTime, event.endTime, event.pid, 1)); }
            if (verbose) { beVerbose(event, p, pa,0); }
        }
    }
}

void Simulator::Print()
{
    if(scheduler->quantum == 0) //FCFS, LCFS, SJF
    {
        printf("%s\n", scheduler->type.c_str());
    }
    else
    {
        printf("%s %d\n", scheduler->type.c_str(), scheduler->quantum);
    }
    
    for (int i = 0; i < processTotalNum; ++i)
    {
        Process& p = processList[i];
        printf("%04d: %4d %4d %4d %4d %1d | %5d %5d %5d %5d\n", i, p.AT, p.oldTC, p.CB, p.IO, (p.staticPriority+1), p.FT, p.TT, p.IT, p.CW);
        if(p.FT > FT){ FT = p.FT; }
        totalTT += p.TT;
        totalCW += p.CW;
    }
    idleIO += FT - lastTimeIdleIO;
    printf("SUM: %d %.2lf %.2lf %.2lf %.2lf %.3lf\n", FT, (FT-idleCT) * 100/ FT, (FT - idleIO) * 100 / FT, totalTT / processTotalNum, totalCW / processTotalNum, 100.0 * processTotalNum / FT);
}
