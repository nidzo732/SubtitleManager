#ifndef SUBTITLES_H
#define SUBTITLES_H
#include <vector>
#include "subtitle.h"
#include "interval.h"

class Subtitles
{
public:
    enum ValidationProblem{PARSE_FAILURE=-1, OK=0, INTERVALS_OVERLAP=1, UNKNOWN_TIMES=2, INVALID_TIMES=4};
    class iterator
    {
    private:
        std::vector<Subtitle>::iterator myIter;
        iterator(std::vector<Subtitle>::iterator iter)
        {
            myIter=iter;
        }
    public:
        iterator(const iterator &i)=default;
        iterator& operator=(const iterator &i)=default;
        iterator& operator++()
        {
            ++myIter;
            return *this;
        }
        iterator operator++(int)
        {
            iterator myCopy=*this;
            ++myIter;
            return myCopy;
        }
        bool operator==(const iterator i)
        {
            return myIter==i.myIter;
        }
        bool operator!=(const iterator i)
        {
            return myIter!=i.myIter;
        }
        Subtitle& operator*()
        {
            return (*myIter);
        }
        friend class Subtitles;
    };

    Subtitles();
    iterator after(int milis);
    iterator before(int milis);
    iterator begin();
    iterator end();
    ValidationProblem insert(Subtitle s);
    void remove(iterator iter);
    ValidationProblem repairAndValidate(int fix=ValidationProblem::OK, int recurseTo=5);
private:
    std::vector<Subtitle> subtitles;
    ValidationProblem status=PARSE_FAILURE;
};

#endif // SUBTITLES_H
