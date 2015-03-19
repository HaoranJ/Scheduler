//
//  Event.cpp
//  Scheduler
//
//  Created by Haoran Jia on 2/26/15.
//  Copyright (c) 2015 Haoran Jia. All rights reserved.
//

#include "Event.h"

using namespace::std;

int Event::maxEventNum = 0;

Event::Event()
{
    
}

Event::Event(int _startTime, int _endTime, int _pid, int _status)
{
    startTime = _startTime;
    endTime = _endTime;
    pid = _pid;
    status = _status;
    eventNum = maxEventNum++;
}

// < --- right  this
bool Event::operator<(const Event &right) const
{
    if (endTime > right.endTime) {
        return true;
    }
    if (endTime == right.endTime) {
        return eventNum > right.eventNum;
    }
    return false;
}

