//
//  Process.h
//  Scheduler
//
//  Created by Haoran Jia on 2/26/15.
//  Copyright (c) 2015 Haoran Jia. All rights reserved.
//

#ifndef __Scheduler__Process__
#define __Scheduler__Process__

#include <stdio.h>

class Process
{
public:
    int AT;
    int TC;
    int CB;
    int IO;
    int staticPriority;
    int dynamicPriority;
    int processID;
    int isBlocked;
    int lastReady;
    int expiredOrder;
    int oldTC;
    int remainCB;
    int FT;
    int TT;
    int IT;
    int CW;
    
    Process(int _AT, int _TC, int _CB, int _IO, int _staticPriority, int _processID);
    bool operator<(const Process &right) const;
};



#endif /* defined(__Scheduler__Process__) */
