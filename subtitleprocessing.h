#ifndef SUBTITLEPROCESSING_H
#define SUBTITLEPROCESSING_H
#include "subtitles.h"

class SubtitleProcessing
{
public:
    Subtitles::ValidationProblem proces(Subtitles *target, Subtitles* &s);
    virtual void doWork()=0;
    void makeMeACopy();
private:
    Subtitles *targetTitles;
protected:
    Subtitles *resultTitles=nullptr;
};

#endif // SUBTITLEPROCESSING_H
