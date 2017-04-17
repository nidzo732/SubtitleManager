#include "subtitle.h"

Subtitle::Subtitle(Interval time, QString content)
    :time(time),content(content)
{
}

QString& Subtitle::getContent()
{
    return content;
}

void Subtitle::setContent(QString content)
{
    this->content=content;
}

Interval& Subtitle::getTime()
{
    return this->time;
}

void Subtitle::setTime(Interval time)
{
    this->time=time;
}
