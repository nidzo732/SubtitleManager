#include<vector>
#include<algorithm>
#include "subtitles.h"

#define DEFAULT_LENGTH 3000
using namespace std;

Subtitles::Subtitles()
{

}

Subtitles::iterator Subtitles::after(int milis)
{
    return lower_bound(subtitles.begin(), subtitles.end(), Subtitle(Interval(milis, Interval::UNKNOWN, Interval::UNKNOWN)));
}

Subtitles::iterator Subtitles::before(int milis)
{
    return upper_bound(subtitles.begin(), subtitles.end(), Subtitle(Interval(milis+1, Interval::UNKNOWN, Interval::UNKNOWN)));
}

Subtitles::ValidationProblem Subtitles::insert(Subtitle s, bool validate, iterator &position)
{
    subtitles.push_back(s);
    if(validate)
    {
        for(auto iter=subtitles.begin();iter!=subtitles.end();iter++)
        {
            if((*iter).getTime()==s.getTime())
            {
                position=iterator(iter);
            }
        }
        ValidationProblem status=repairAndValidate();
        position=iterator(find_if(this->subtitles.begin(), this->subtitles.end(), [&s](Subtitle q)->bool{return q.getTime()==s.getTime();}));
        return status;
    }
    else
    {
        return ValidationProblem::OK;
    }
}

void Subtitles::remove(Subtitles::iterator iter)
{
    subtitles.erase(iter.myIter);
}

Subtitles::ValidationProblem Subtitles::repairAndValidate(int ignore, int recurseTo)
{
    int fixcount=0;
    sort(subtitles.begin(), subtitles.end());
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
                if(currentTitle.getTime().getOffsetHint()!=Interval::UNKNOWN)
                {
                    currentTitle.getTime().setStart((*prev).getTime().getEnd()+currentTitle.getTime().getOffsetHint());
                }
                else
                {
                    currentTitle.getTime().setStart((*prev).getTime().getEnd());
                }
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
            if(currentTitle.getTime().getDurationHint()!=Interval::UNKNOWN)
            {
                (*current).getTime().setEnd((*current).getTime().getStart()+currentTitle.getTime().getDurationHint());
            }
            else if(next!=subtitles.end() && (*next).getTime().getEnd()!=Interval::UNKNOWN)
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
    sort(subtitles.begin(), subtitles.end());
    if(fixcount==0)
    {
        for(Subtitle& title:subtitles)
        {
            QString content=title.getContent();
            while(content.endsWith("\n") || content.endsWith(" ") || content.endsWith("\r"))
            {
                content=content.left(content.length()-1);
            }
            while(content.endsWith("<br/>"))
            {
                content=content.left(content.length()-5);
            }
            title.setContent(content);
        }
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
Subtitles::iterator Subtitles::operator[](int index)
{
    return iterator(subtitles.begin()+index);
}
QString Subtitles::getProblemDescription(Subtitles::ValidationProblem problem)
{
    switch(problem)
    {
        case Subtitles::INTERVALS_OVERLAP:
            return "sentences overlap in time";
            break;
        case Subtitles::INVALID_TIMES:
            return "some time marks are invalid";
            break;
        case Subtitles::UNKNOWN_TIMES:
            return "some sentences have unknown appearance times";
            break;
        case Subtitles::PARSE_FAILURE:
            return "failed to parse subtitles";
            break;
        case Subtitles::OK:
        default:
            return "everything OK";
            break;
    }
}
