#include<QRegExp>
#include<QRegularExpression>

#include<QString>
#include<string>
#include "srtio.h"

static const QString NUMBERLINE_REGEX="\\d+";
static const QString TIME_REGEX=R"(((?:\d+\:\d+\:\d+\,\d+)?)((?:\-*\>?)?)((?:\d+\:\d+\:\d+\,\d+)?))";
static QRegExp numberline(NUMBERLINE_REGEX);
static QRegExp timeline(TIME_REGEX);
enum ReadStep {BEGAN, READ_NUMBER, READ_EMPTY, READ_TIME, READ_SENTENCE};


SrtIO::SrtIO(){}


Subtitles SrtIO::Read(QTextStream &inputStream, bool tryOnly, bool &ok, std::vector<int>) const
{
    Subtitles toReturn;
    Subtitle current;
    Subtitles::iterator dummy;
    ReadStep status=BEGAN;
    int prevSeq=Interval::UNKNOWN;
    ok=false;
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
        else if(numberline.exactMatch(trimmed))
        {
            if(status==READ_EMPTY || status==READ_NUMBER || status==BEGAN)
            {
                status=READ_NUMBER;
                prevSeq=trimmed.toInt();
            }
            else
            {
                status=READ_SENTENCE;
                current.getContent()+=line+"<br/>";
            }
        }
        else if(timeline.exactMatch(trimmed))
        {
            if(status==BEGAN || status==READ_EMPTY || status==READ_NUMBER)
            {
                if(current.getContent().length()>0)
                {
                    toReturn.insert(current, false, dummy);
                }
                status=READ_TIME;
                current=Subtitle();
                current.setTime(parseInterval(trimmed, prevSeq));
                prevSeq=Interval::UNKNOWN;
                if(tryOnly)
                {
                    ok=true;
                    return toReturn;
                }
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
Subtitles SrtIO::Read(QTextStream &inputStream, std::vector<int> params) const
{
    bool dummyOk;
    return Read(inputStream, false, dummyOk, params);
}

bool SrtIO::Try(QTextStream &inputStream) const
{
    bool ok;
    Read(inputStream, true, ok, std::vector<int>());
    inputStream.seek(0);
    return ok;
}

void SrtIO::Write(QTextStream &outputStream, Subtitles &subtitles,std::vector<int>) const
{
    int id=1;
    for(auto title: subtitles)
    {
        outputStream<<id<<'\n';
        outputStream<<printTime(title.getTime().getStart())<<" --> "<<printTime(title.getTime().getEnd())<<'\n';
        outputStream<<QString(title.getContent()).replace("<br/>","\n")<<'\n';
        if(!(title.getContent().endsWith("\n") ||title.getContent().endsWith("<br/>")))
        {
            outputStream<<'\n';
        }
        id++;
    }
}
int SrtIO::parseTime(QString time)
{
    if(time.length()==0) return Interval::UNKNOWN;
    int hh,mm;
    float ss;
    QStringList parts=time.split(":");
    hh=parts[0].toInt();
    mm=parts[1].toInt();
    ss=parts[2].replace(',',".").toFloat();
    return (int)((hh*3600+mm*60+ss)*1000);
}

Interval SrtIO::parseInterval(QString interval, int seq)
{
    timeline.indexIn(interval);
    return Interval(parseTime(timeline.cap(1)), parseTime(timeline.cap(3)), seq);
}
QString SrtIO::printTime(int time)
{
    if(time<0) return "00:00:00,000";
    int milis=time%1000;
    time/=1000;
    int seconds=time%60;
    time/=60;
    int minutes=time%60;
    time/=60;
    int hours=time;
    QString hoursString = QString::number(hours);
    while(hoursString.length()<2) hoursString="0"+hoursString;
    QString minutesString = QString::number(minutes);
    while(minutesString.length()<2) minutesString="0"+minutesString;
    QString secondsString = QString::number(seconds);
    while(secondsString.length()<2) secondsString="0"+secondsString;
    QString milisString = QString::number(milis);
    while(milisString.length()<3) milisString="0"+milisString;
    return hoursString+":"+minutesString+":"+secondsString+","+milisString;
}

const SrtIO* SrtIO::getInstance()
{
    if(instance) return instance;
    else
    {
        instance=new SrtIO();
        return instance;
    }
}
SrtIO* SrtIO::instance=nullptr;
QStringList SrtIO::GetSupportedFormats() const
{
    return QStringList({"srt"});
}
QString SrtIO::GetDescriptiveName() const
{
    return "SRT subtitle";
}
