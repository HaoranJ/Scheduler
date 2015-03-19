//
//  Process.cpp
//  Scheduler
//
//  Created by Haoran Jia on 2/26/15.
//  Copyright (c) 2015 Haoran Jia. All rights reserved.
//

#include "Process.h"

Process::Process(int _AT, int _TC, int _CB, int _IO, int _staticPriority, int _processID)
{
    AT = _AT;
    TC = _TC;
    CB = _CB;
    IO = _IO;
    staticPriority = _staticPriority;
    processID = _processID;
    dynamicPriority = staticPriority;
    isBlocked = 0;
    lastReady = _AT;
    expiredOrder = 0;
    oldTC = TC;
    remainCB = 0;
    FT = 0;
    TT = 0;
    IT = 0;
    CW = 0;
}

bool Process::operator<(const Process &right) const
{
    return expiredOrder < right.expiredOrder;
}