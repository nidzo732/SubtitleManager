#include "interval.h"

Interval::Interval(int milisStart, int milisEnd, int tSeq, int durationHint, int offsetHint)
{
    start=milisStart;
    end=milisEnd;
    seq=tSeq;
    this->durationHint=durationHint;
    this->offsetHint=offsetHint;
}
bool Interval::operator<(const Interval &i) const
{
    if(i.start!=UNKNOWN && start!=UNKNOWN)
    {
        return start<i.start;
    }
    if(i.end!=UNKNOWN && end!=UNKNOWN)
    {
        return end<i.end;
    }
    return seq<i.seq;
}

bool Interval::operator>(const Interval &i) const
{
    return !(*this==i) && !(*this<i);
}
bool Interval::operator ==(const Interval &i) const
{
    if(start!=UNKNOWN && end!=UNKNOWN &&
       i.start!=UNKNOWN && i.end!=UNKNOWN)
    {
        return start==i.start && end==i.end;
    }
    else
    {
        return seq!=UNKNOWN && i.seq!=UNKNOWN && i.seq==seq;
    }
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
int Interval::getDurationHint() const
{
    return durationHint;
}
int Interval::getOffsetHint() const
{
    return offsetHint;
}
