#ifndef INTERVAL_H
#define INTERVAL_H
#include<QString>

class Interval
{
public:
    static const int UNKNOWN=-1;
    Interval(int milsStart, int milisEnd, int seq=UNKNOWN, int durationHint=UNKNOWN, int offsetHint=UNKNOWN);
    bool operator<(const Interval &i) const;
    bool operator>(const Interval &i) const;
    bool operator==(const Interval &i) const;
    QString toStringEnd() const;
    QString toStringStart() const;
    int getStart() const;
    void setStart(int time);
    int getEnd() const;
    void setEnd(int time);
    int getDurationHint() const;
    int getOffsetHint() const;
private:
    static QString timeToString(int milis);
    int start, end;
    int seq;
    int durationHint;
    int offsetHint;
};

#endif // INTERVAL_H
