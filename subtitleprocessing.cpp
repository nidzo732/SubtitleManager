#include "subtitles.h"
#include "subtitleprocessing.h"

Subtitles::ValidationProblem SubtitleProcessing::proces(Subtitles *target, Subtitles* &s)
{
    this->targetTitles=target;
    doWork();
    Subtitles::ValidationProblem status=this->resultTitles->repairAndValidate();
    if(status!=Subtitles::OK)
    {
        delete this->resultTitles;
        this->resultTitles=nullptr;
    }
    s=this->resultTitles;
    return status;
}
void SubtitleProcessing::makeMeACopy()
{
    this->resultTitles=new Subtitles(*targetTitles);
}
