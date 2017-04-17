#include "interval.h"

Interval::Interval(int milisStart, int milisEnd)
{
    start=milisStart;
    end=milisEnd;
}
bool Interval::operator<(const Interval &i) const
{
    if(i.start!=UNKNOWN)
    {
        if(start!=UNKNOWN) return start<i.start;
        else if(end!=UNKNOWN) return end<i.start;
        return true;
    }
    if(i.end!=UNKNOWN)
    {
        if(end!=UNKNOWN) return end<i.end;
        else if(start!=UNKNOWN) return start<i.end;
        return true;
    }
    return true;
}

bool Interval::operator>(const Interval &i) const
{
    return !(*this==i) && !(*this<i);
}
bool Interval::operator ==(const Interval &i) const
{
    return start==i.start && end==i.end;
}

QString Interval::toStringEnd() const
{
    return timeToString(end);
}
QString Interval::toStringStart() const
{
    return timeToString(start);
}
int Interval::getStart() const
{
    return start;
}
void Interval::setStart(int time)
{
    start=time;
}

int Interval::getEnd() const
{
    return end;
}
void Interval::setEnd(int time)
{
    end=time;
}

QString Interval::timeToString(int milis)
{
    int ms=milis%1000;
    milis/=1000;
    int ss=milis%60;
    milis/=60;
    int mm=milis%60;
    milis/=60;
    int hh=milis;
    QString milisS=QString::number(ms);
    QString secondsS=QString::number(ss);
    QString minutesS=QString::number(mm);
    QString hoursS=QString::number(hh);
    while(milisS.length()<3) milisS="0"+milisS;
    while(secondsS.length()<2) secondsS="0"+secondsS;
    while(minutesS.length()<2) minutesS="0"+minutesS;
    while(hoursS.length()<2) hoursS="0"+hoursS;
    return hoursS+":"+minutesS+":"+secondsS+"."+milisS;
}
