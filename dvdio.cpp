#include<QRegExp>
#include<QRegularExpression>

#include<QString>
#include<string>
#include "dvdio.h"

static const QString LINE_REGEX=R"(\s*((?:\{\s*\d+\s*\})?)\s*((?:\{\s*\d+\s*\})?)(.*))";
static QRegExp lineline(LINE_REGEX);


DvdIO::DvdIO(){}


Subtitles DvdIO::Read(QTextStream &inputStream, bool tryOnly, bool &ok, std::vector<int> params) const
{
    Subtitles toReturn;
    Subtitles::iterator dummy;
    const int framerate = params.size()>0?params[0]:25;
    const int milisecs_per_frame=1000/framerate;
    ok=false;
    int seq=0;
    while(!inputStream.atEnd())
    {
        QString line=inputStream.readLine();
        while(line.endsWith("\n") || line.endsWith("\n")) line=line.left(line.length()-1);
        QString trimmed=QString(line).replace(QRegularExpression("\\s"), "");
        if(trimmed.startsWith("#")) continue;
        if(trimmed.length()==0) continue;
        if(lineline.exactMatch(line) && lineline.cap(3).length()>0)
        {
            if(tryOnly && lineline.cap(1).length()>0 && lineline.cap(2).length()>0)
            {
                ok=true;
                return toReturn;
            }
            int start=Interval::UNKNOWN;
            int end=Interval::UNKNOWN;
            QString timeStart=lineline.cap(1);
            QString timeEnd=lineline.cap(2);
            timeStart.replace("{","").replace("}","");
            timeEnd.replace("{","").replace("}","");
            if(timeStart.length()>0)
            {
                start=timeStart.toInt()*milisecs_per_frame;
            }
            else
            {
                int a=6;
                a++;
            }
            if(timeEnd.length()>0)
            {
                end=timeEnd.toInt()*milisecs_per_frame;
            }
            else
            {
                int a=6;
                a++;
            }
            QString text=lineline.cap(3);
            text.replace("|", "<br/>");
            toReturn.insert(Subtitle(Interval(start, end, seq++), text), false, dummy);
        }
        else
        {
            int a=6;
            a++;
        }

    }
    ok=false;
    return toReturn;
}
Subtitles DvdIO::Read(QTextStream &inputStream, std::vector<int> params) const
{
    bool dummyOk;
    return Read(inputStream, false, dummyOk, params);
}

bool DvdIO::Try(QTextStream &inputStream) const
{
    bool ok;
    Read(inputStream, true, ok,std::vector<int>());
    inputStream.seek(0);
    return ok;
}

void DvdIO::Write(QTextStream &outputStream, Subtitles &subtitles, std::vector<int> params) const
{
    const int framerate = params.size()>0?params[0]:25;
    const int milisecs_per_frame=1000/framerate;
    for(auto title:subtitles)
    {
        outputStream<<"{"<<title.getTime().getStart()/milisecs_per_frame<<"}";
        outputStream<<"{"<<title.getTime().getEnd()/milisecs_per_frame<<"}";
        outputStream<<QString(title.getContent()).replace("<br/>", "|");
        outputStream<<'\n';
    }
}
const DvdIO* DvdIO::getInstance()
{
    if(instance) return instance;
    else
    {
        instance=new DvdIO();
        return instance;
    }
}
DvdIO* DvdIO::instance=nullptr;
QStringList DvdIO::GetSupportedFormats() const
{
    return QStringList({"sub", "txt"});
}
QString DvdIO::GetDescriptiveName() const
{
    return "MicroDVD subtitle";
}
QStringList DvdIO::Questions() const
{
    return QStringList({"Select framerate:"});
}
