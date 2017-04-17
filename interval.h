#ifndef INTERVAL_H
#define INTERVAL_H
#include<QString>

class Interval
{
public:
    static const int UNKNOWN=-1;
    Interval(int milsStart, int milisEnd);
    bool operator<(const Interval &i) const;
    bool operator>(const Interval &i) const;
    bool operator==(const Interval &i) const;
    QString toStringEnd() const;
    QString toStringStart() const;
    int getStart() const;
    void setStart(int time);
    int getEnd() const;
    void setEnd(int time);
private:
    static QString timeToString(int milis);
    int start, end;
};

#endif // INTERVAL_H
