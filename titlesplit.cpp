#include "titlesplit.h"
#include "ui_titlesplit.h"
#include <QMessageBox>

TitleSplit::TitleSplit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TitleSplit)
{
    ui->setupUi(this);
}

TitleSplit::~TitleSplit()
{
    delete ui;
}
void TitleSplit::doWork()
{
    makeMeACopy();
    for(Subtitles::iterator iter=resultTitles->begin();iter!=resultTitles->end();)
    {
        const QRegularExpression spaceOrNew(QRegularExpression("(?:\\s|\\n|<br/>)"));
        int index=iter-resultTitles->begin();
        if(QString((*iter).getContent()).replace(spaceOrNew, "").length()==0){iter++;continue;}
        if((*iter).getContent().length()<=maxLength){iter++; continue;}
        if((*iter).getContent().indexOf(' ')==-1) {iter++;continue;}
        QStringList words=QString((*iter).getContent()).split(spaceOrNew, QString::SkipEmptyParts);
        int totalDuration=(*iter).getTime().getEnd()-(*iter).getTime().getStart();
        int newDuration = totalDuration/words.length();
        int currentStart=(*iter).getTime().getStart();
        QString currentString="";
        Subtitles::iterator dummy;
        for(QString word: words)
        {
            if(word.length()+currentString.length()>maxLength)
            {
                if(currentString.length()==0)
                {
                    resultTitles->insert(Subtitle(Interval(currentStart, currentStart+newDuration), word), false, dummy);
                    currentStart+=newDuration;
                }
                else
                {
                    QString emptyTest=QString(currentString).replace(spaceOrNew,"");
                    if(emptyTest!="<br/>" && emptyTest!="")
                    {
                        resultTitles->insert(Subtitle(Interval(currentStart, currentStart+newDuration), currentString.left(currentString.length()-1)), false, dummy);
                        currentStart+=newDuration;
                    }
                    currentString=word+" ";
                }
            }
            else
            {
                currentString+=word+" ";
            }
        }
        if(currentString.length()>0)
        {
            QString emptyTest=QString(currentString).replace(spaceOrNew,"");
            if(emptyTest!="<br/>" && emptyTest!="")
            {
                resultTitles->insert(Subtitle(Interval(currentStart, currentStart+newDuration), currentString.left(currentString.length()-1)), false, dummy);
            }
        }
        iter=resultTitles->begin()+index;
        resultTitles->remove(iter);
        if(index>resultTitles->end()-resultTitles->begin())
        {
            iter=resultTitles->end();
        }
        else
        {
            iter=resultTitles->begin()+index;
        }
    }
}

Subtitles* TitleSplit::doProcess(QWidget *parent, Subtitles *target)
{
    TitleSplit splitter(parent);
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

void TitleSplit::on_buttonBox_accepted()
{
    maxLength=ui->maxLength->value();
    accepted=true;
}
