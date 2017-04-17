#include<vector>
#include<algorithm>
#include "subtitles.h"

#define DEFAULT_LENGTH 5000
using namespace std;

Subtitles::Subtitles()
{

}

Subtitles::iterator Subtitles::after(int milis)
{
    return lower_bound(subtitles.begin(), subtitles.end(), Subtitle(Interval(milis, Interval::UNKNOWN)));
}

Subtitles::iterator Subtitles::before(int milis)
{
    return upper_bound(subtitles.begin(), subtitles.end(), Subtitle(Interval(milis, Interval::UNKNOWN)));
}

Subtitles::ValidationProblem Subtitles::insert(Subtitle s)
{
    subtitles.push_back(s);
    sort(subtitles.begin(), subtitles.end());
    return ValidationProblem::OK;
}

void Subtitles::remove(Subtitles::iterator iter)
{
    subtitles.erase(iter.myIter);
}

Subtitles::ValidationProblem Subtitles::repairAndValidate(int ignore, int recurseTo)
{
    int fixcount=0;
    vector<Subtitle>::iterator prev=subtitles.end(), next=subtitles.end(), current=subtitles.begin();
    next=current;
    if(next!=subtitles.end()) next++;
    for(;current!=subtitles.end();current++,next!=subtitles.end()?next++:next)
    {
        Subtitle &currentTitle=*current;
        if(currentTitle.getTime().getStart()==Interval::UNKNOWN)
        {
            fixcount++;
            if(!(ignore&ValidationProblem::UNKNOWN_TIMES))
            {
                status=ValidationProblem::UNKNOWN_TIMES;
                return ValidationProblem::UNKNOWN_TIMES;
            }
            if(prev!=subtitles.end())
            {
                currentTitle.getTime().setStart((*prev).getTime().getEnd());
            }
            else
            {
                currentTitle.getTime().setStart(0);
            }
        }
        if(currentTitle.getTime().getEnd()==Interval::UNKNOWN)
        {
            fixcount++;
            if(!(ignore&ValidationProblem::UNKNOWN_TIMES))
            {
                status=ValidationProblem::UNKNOWN_TIMES;
                return ValidationProblem::UNKNOWN_TIMES;
            }
            if(next!=subtitles.end())
            {
                (*current).getTime().setEnd((*next).getTime().getStart());
            }
            else
            {
                (*current).getTime().setEnd((*current).getTime().getStart()+DEFAULT_LENGTH);
            }
        }
        prev=current;
    }
    prev=subtitles.end();
    next=subtitles.end();
    current=subtitles.begin();
    next=current;
    if(next!=subtitles.end()) next++;
    for(;current!=subtitles.end();current++,next!=subtitles.end()?next++:next)
    {
        if((*current).getTime().getStart()>(*current).getTime().getEnd())
        {
            fixcount++;
            if(!(ignore&ValidationProblem::INVALID_TIMES))
            {
                status=ValidationProblem::INVALID_TIMES;
                return ValidationProblem::INVALID_TIMES;
            }
            if(prev!=subtitles.end())
            {
                (*current).getTime().setStart((*prev).getTime().getEnd());
            }
            else
            {
                (*current).getTime().setStart(0);
            }
            if(next!=subtitles.end())
            {
                (*current).getTime().setEnd((*next).getTime().getStart());
            }
            else
            {
                (*current).getTime().setEnd((*current).getTime().getStart()+DEFAULT_LENGTH);
            }
        }
        prev=current;
    }
    prev=subtitles.end();
    next=subtitles.end();
    current=subtitles.begin();
    next=current;
    if(next!=subtitles.end()) next++;
    for(;current!=subtitles.end();current++,next!=subtitles.end()?next++:next)
    {
        if(next!=subtitles.end() && (*next).getTime().getStart()<(*current).getTime().getEnd())
        {
            fixcount++;
            if(!(ignore&ValidationProblem::INTERVALS_OVERLAP))
            {
                status=ValidationProblem::INTERVALS_OVERLAP;
                return ValidationProblem::INTERVALS_OVERLAP;
            }
            (*current).getTime().setEnd((*next).getTime().getStart());
        }
        prev=current;
    }
    /*vector<Subtitle> newMap;
    for(auto title:subtitles)
    {
        newMap[title.second.getTime()]=title.second;
    }
    subtitles=newMap;*/
    sort(subtitles.begin(), subtitles.end());
    if(fixcount==0)
    {
        this->status=ValidationProblem::OK;
        return ValidationProblem::OK;
    }
    if(recurseTo>1)
    {
        return repairAndValidate(ignore, recurseTo-1);
    }
    else
    {
        return repairAndValidate(ValidationProblem::OK, recurseTo-1);
    }
}
Subtitles::iterator Subtitles::begin()
{
    return iterator(subtitles.begin());
}

Subtitles::iterator Subtitles::end()
{
    return iterator(subtitles.end());
}
