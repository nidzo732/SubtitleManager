#include "titlemerge.h"
#include "ui_titlemerge.h"
#include<QMessageBox>

TitleMerge::TitleMerge(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TitleMerge)
{
    ui->setupUi(this);
}

TitleMerge::~TitleMerge()
{
    delete ui;
}
void TitleMerge::doWork()
{
    makeMeACopy();
    for(Subtitles::iterator iter=resultTitles->begin();iter!=resultTitles->end();)
    {
        Subtitles::iterator next=iter+1;
        int nextindex=next-resultTitles->begin();
        if(next!=resultTitles->end())
        {
            if(((*iter).getContent().length()<minLength || (*next).getContent().length()<minLength)
                    && ((*iter).getContent().length()+(*next).getContent().length()<maxLength))
            {
                if((*next).getTime().getStart()-(*iter).getTime().getEnd()<maxDistance)
                {
                    QString content1=(*iter).getContent();
                    QString content2=(*next).getContent();
                    while(content1.endsWith("<br/>")) content1=content1.left(content1.length()-5);
                    while(content2.endsWith("<br/>")) content2=content2.left(content2.length()-5);
                    while(content1.startsWith("<br/>")) content1=content1.right(content1.length()-5);
                    while(content2.startsWith("<br/>")) content2=content2.right(content2.length()-5);
                    (*iter).setContent(content1+"<br/>"+content2);
                    (*iter).getTime().setEnd((*next).getTime().getEnd());
                    resultTitles->remove(next);
                }
            }
        }
        iter=resultTitles->begin()+nextindex;
    }
}

Subtitles* TitleMerge::doProcess(QWidget *parent, Subtitles *target)
{
    TitleMerge merger(parent);
    merger.exec();
    while(merger.accepted)
    {
        Subtitles *result;
        Subtitles::ValidationProblem status;
        status=merger.proces(target, result);
        if(status==Subtitles::OK) return result;
        QMessageBox::critical(parent, "Problem", "Can't modify: "+Subtitles::getProblemDescription(status));
        merger.accepted=false;
        merger.exec();
    }
    return nullptr;
}

void TitleMerge::on_buttonBox_accepted()
{
    minLength=ui->minLength->value();
    maxDistance=ui->maxDistance->value();
    maxLength=ui->maxLength->value();
    accepted=true;
}

void TitleMerge::on_minLength_valueChanged(int value)
{
    ui->maxLength->setMinimum(value);
}
