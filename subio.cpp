#include<QRegExp>
#include<QRegularExpression>

#include<QString>
#include<string>
#include "SubIO.h"

static const QString TIME_REGEX=R"(\s*((?:[0-9]*\.?[0-9]*)?)\s*((?:[0-9]*\.?[0-9]*)?)\s*$)";
static QRegExp timeline(TIME_REGEX);
enum ReadStep {BEGAN, READ_EMPTY, READ_TIME, READ_SENTENCE};


SubIO::SubIO(){}


Subtitles SubIO::Read(QTextStream &inputStream, bool tryOnly, bool &ok, std::vector<int>) const
{
    Subtitles toReturn;
    Subtitle current;
    Subtitles::iterator dummy;
    ReadStep status=BEGAN;
    int seq=1;
    ok=false;
    int prevTime=0;
    while(!inputStream.atEnd())
    {
        QString line=inputStream.readLine();
        while(line.endsWith("\n") || line.endsWith("\n")) line=line.left(line.length()-1);
        QString trimmed=QString(line).replace(QRegularExpression("\\s"), "");
        if(trimmed.startsWith("#")) continue;
        if(trimmed.length()==0)
        {
            if(current.getContent().length()>0)
            {
                toReturn.insert(current, false, dummy);
            }
            status=READ_EMPTY;
            current=Subtitle();
        }
        else if(timeline.exactMatch(line))
        {
            if(status==BEGAN || status==READ_EMPTY)
            {
                if(current.getContent().length()>0)
                {
                    toReturn.insert(current, false, dummy);
                }
                if(tryOnly && READ_EMPTY && timeline.cap(1).length()>0 && timeline.cap(2).length()>0)
                {
                    ok=true;
                    return toReturn;
                }
                status=READ_TIME;
                current=Subtitle();
                current.setTime(parseInterval(line, prevTime, seq++));
            }
            else
            {
                status=READ_SENTENCE;
                current.getContent()+=line+"<br/>";
            }
        }
        else
        {
            if(status==READ_SENTENCE || status==READ_TIME)
            {
                current.getContent()+=line+"<br/>";
            }
            else
            {
                return toReturn;
            }
        }
    }
    if(current.getContent().length()>0)
    {
        toReturn.insert(current, false, dummy);
    }
    ok=false;
    return toReturn;
}
Subtitles SubIO::Read(QTextStream &inputStream, std::vector<int> params) const
{
    bool dummyOk;
    return Read(inputStream, false, dummyOk, params);
}

bool SubIO::Try(QTextStream &inputStream) const
{
    bool ok;
    Read(inputStream, true, ok, std::vector<int>());
    inputStream.seek(0);
    return ok;
}

void SubIO::Write(QTextStream &outputStream, Subtitles &subtitles,std::vector<int>) const
{
    int prevTime=0;
    for(auto title: subtitles)
    {
        int offset=title.getTime().getStart()-prevTime;
        int duration=title.getTime().getEnd()-title.getTime().getStart();
        int offsetSecs=offset/1000;
        QString offsetMilis=QString::number(offset%1000);
        while(offsetMilis.length()<3) offsetMilis="0"+offsetMilis;
        int durationSecs=duration/1000;
        QString durationMilis=QString::number(duration%1000);
        while(durationMilis.length()<3) durationMilis="0"+durationMilis;
        outputStream<<offsetSecs<<"."<<offsetMilis<<" "<<durationSecs<<"."<<durationMilis<<'\n';
        outputStream<<QString(title.getContent()).replace("<br/>","\n")<<'\n';
        if(!(title.getContent().endsWith("\n") ||title.getContent().endsWith("<br/>")))
        {
            outputStream<<'\n';
        }
        prevTime=title.getTime().getEnd();
    }
}

Interval SubIO::parseInterval(QString interval, int &prevTime, int seq)
{
    timeline.indexIn(interval);
    int offset=Interval::UNKNOWN;
    int duration=Interval::UNKNOWN;
    if(timeline.cap(1).length()>0)
    {
        QStringList parts=timeline.cap(1).split(".", QString::SkipEmptyParts);
        int offsetSec=parts[0].toInt();
        int offsetMilis=0;
        if(parts.length()>1)
        {
            offsetMilis=parts[1].left(3).toInt();
            for(int i=0;i<3-parts[1].length();i++) offsetMilis*=10;
        }
        offset=offsetSec*1000+offsetMilis;
    }
    if(timeline.cap(2).length()>0)
    {
        QStringList parts=timeline.cap(2).split(".", QString::SkipEmptyParts);
        int durationSec=parts[0].toInt();
        int durationMilis=0;
        if(parts.length()>1)
        {
            durationMilis=parts[1].left(3).toInt();
            for(int i=0;i<3-parts[1].length();i++) durationMilis*=10;
        }
        duration=durationSec*1000+durationMilis;
    }
    int start=Interval::UNKNOWN;
    int end=Interval::UNKNOWN;
    if(prevTime!=Interval::UNKNOWN && offset!=Interval::UNKNOWN) start=prevTime+offset;
    if(start!=Interval::UNKNOWN && duration!=Interval::UNKNOWN) end=start+duration;
    prevTime=end;
    return Interval(start, end, seq, duration, offset);
}

const SubIO* SubIO::getInstance()
{
    if(instance) return instance;
    else
    {
        instance=new SubIO();
        return instance;
    }
}
SubIO* SubIO::instance=nullptr;
QStringList SubIO::GetSupportedFormats() const
{
    return QStringList({"sub"});
}
QString SubIO::GetDescriptiveName() const
{
    return "MPlayer subtitle";
}
