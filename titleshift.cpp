#include "titleshift.h"
#include "ui_titleshift.h"
#include <QMessageBox>

TitleShift::TitleShift(QWidget *parent, Subtitles *target) :
    QDialog(parent),
    ui(new Ui::TitleShift)
{
    start1=start2=(*(target->begin())).getTime().getStart();
    end1=end2=(*(target->end()-1)).getTime().getEnd();
    length1=end1-start1;
    length2=end2-start2;
    ui->setupUi(this);
    ui->start1->setTime(QTime::fromMSecsSinceStartOfDay((*(target->begin())).getTime().getStart()));
    ui->start2->setTime(QTime::fromMSecsSinceStartOfDay((*(target->begin())).getTime().getStart()));
    ui->end1->setTime(QTime::fromMSecsSinceStartOfDay((*(target->end()-1)).getTime().getEnd()));
    ui->end2->setTime(QTime::fromMSecsSinceStartOfDay((*(target->end()-1)).getTime().getEnd()));
    start1=ui->start1->time().msecsSinceStartOfDay();
    start2=ui->start2->time().msecsSinceStartOfDay();
}

TitleShift::~TitleShift()
{
    delete ui;
}
void TitleShift::doWork()
{
    makeMeACopy();
    Subtitles::iterator start=resultTitles->after(start1);
    Subtitles::iterator end=resultTitles->before(end1);
    for(Subtitles::iterator iter=start;iter!=end;iter++)
    {
        Interval time=(*iter).getTime();
        Interval newTime(transfer(time.getStart()), transfer(time.getEnd()));
        (*iter).setTime(newTime);
    }

}

Subtitles* TitleShift::doProcess(QWidget *parent, Subtitles *target)
{
    TitleShift splitter(parent, target);
    splitter.exec();
    while(splitter.accepted)
    {
        Subtitles *result;
        Subtitles::ValidationProblem status;
        status=splitter.proces(target, result);
        if(status==Subtitles::OK) return result;
        QMessageBox::critical(parent, "Problem", "Can't modify: "+Subtitles::getProblemDescription(status));
        splitter.accepted=false;
        splitter.exec();
    }
    return nullptr;
}

void TitleShift::on_buttonBox_accepted()
{
    accepted=true;
}

void TitleShift::on_start1_timeChanged(const QTime &time)
{
    ui->end1->setMinimumTime(time);
    if(ui->maintain1->isChecked())
    {
        ui->end1->setTime(time.addMSecs(length1));
    }
    else
    {
        length1=ui->end1->time().msecsSinceStartOfDay()-time.msecsSinceStartOfDay();
    }
    start1=ui->start1->time().msecsSinceStartOfDay();
    end1=ui->end1->time().msecsSinceStartOfDay();
}

void TitleShift::on_end1_timeChanged(const QTime &time)
{
    ui->start1->setMaximumTime(time);
    if(ui->maintain1->isChecked())
    {
        int newStart=time.msecsSinceStartOfDay()-length1;
        if(newStart<0)
        {
            ui->start1->setTime(QTime::fromMSecsSinceStartOfDay(start1));
            ui->end1->setTime(QTime::fromMSecsSinceStartOfDay(end1));
        }
        else
        {
            ui->start1->setTime(time.addMSecs(-length1));
        }
    }
    else
    {
        length1=ui->end1->time().msecsSinceStartOfDay()-time.msecsSinceStartOfDay();
    }
    start1=ui->start1->time().msecsSinceStartOfDay();
    end1=ui->end1->time().msecsSinceStartOfDay();
}

void TitleShift::on_start2_timeChanged(const QTime &time)
{
    ui->end2->setMinimumTime(time);
    if(ui->maintain2->isChecked())
    {
        ui->end2->setTime(time.addMSecs(length2));
    }
    else
    {
        length2=ui->end2->time().msecsSinceStartOfDay()-time.msecsSinceStartOfDay();
    }
    start2=ui->start2->time().msecsSinceStartOfDay();
    end2=ui->end2->time().msecsSinceStartOfDay();
}

void TitleShift::on_end2_timeChanged(const QTime &time)
{
    ui->start2->setMaximumTime(time);
    if(ui->maintain2->isChecked())
    {
        int newStart=time.msecsSinceStartOfDay()-length2;
        if(newStart<0)
        {
            ui->start2->setTime(QTime::fromMSecsSinceStartOfDay(start2));
            ui->end2->setTime(QTime::fromMSecsSinceStartOfDay(end2));
        }
        else
        {
            ui->start2->setTime(time.addMSecs(-length2));
        }
    }
    else
    {
        length2=ui->end2->time().msecsSinceStartOfDay()-time.msecsSinceStartOfDay();
    }
    start2=ui->start2->time().msecsSinceStartOfDay();
    end2=ui->end2->time().msecsSinceStartOfDay();
}
int TitleShift::transfer(int t)
{
    double center1=((double)(end1+start1))/2;
    double center2=((double)(end2+start2))/2;
    double length1=end1-start1;
    double length2=end2-start2;
    double ratio = length2/length1;
    double distance = t-center1;
    return center2+distance*ratio;
}
