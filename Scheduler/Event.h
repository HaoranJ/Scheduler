//
//  Event.h
//  Scheduler
//
//  Created by Haoran Jia on 2/26/15.
//  Copyright (c) 2015 Haoran Jia. All rights reserved.
//

#ifndef __Scheduler__Event__
#define __Scheduler__Event__

#include <stdio.h>
#include <string>

using namespace std;

class Event
{
public:
    static int maxEventNum;
    
    int startTime;
    int endTime;
    int eventNum;
    int pid;
    
    /*status: -1: created --> ready
              0:blocked --> ready,
              1:ready-->runing, 
              2:running-->blocked.
              3:running->ready
     */
    int status;
    
    Event();
    Event(int _startTime, int _endTime, int _pid, int _status);
    bool operator<(const Event &right) const;
};

#endif /* defined(__Scheduler__Event__) */
