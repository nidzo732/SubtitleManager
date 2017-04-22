#ifndef SUBTITLE_H
#define SUBTITLE_H
#include <QString>
#include "interval.h"

class Subtitle
{
public:
    Subtitle(Interval time=Interval(Interval::UNKNOWN, Interval::UNKNOWN, Interval::UNKNOWN), QString content=QString(""));
    QString& getContent();
    void setContent(QString content);
    Interval& getTime();
    void setTime(Interval t);
    bool operator<(const Subtitle &s2) const
    {
        return time<s2.time;
    }
    bool operator>(const Subtitle &s2) const
    {
        return time>s2.time;
    }

private:
    Interval time;
    QString content;
};

#endif // SUBTITLE_H
