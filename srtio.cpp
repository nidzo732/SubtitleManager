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


Subtitles SrtIO::Read(QTextStream &inputStream, bool tryOnly, bool &ok) const
{
    Subtitles toReturn;
    Subtitle current;
    ReadStep status=BEGAN;
    ok=false;
    while(!inputStream.atEnd())
    {
        QString line=inputStream.readLine();
        QString trimmed=QString(line).replace(QRegularExpression("\\s"), "");
        if(trimmed.startsWith("#")) continue;
        if(trimmed.length()==0)
        {
            if(current.getContent().length()>0)
            {
                toReturn.insert(current);
            }
            status=READ_EMPTY;
            current=Subtitle();
        }
        else if(numberline.exactMatch(trimmed))
        {
            if(status==READ_EMPTY || status==READ_NUMBER || status==BEGAN)
            {
                status=READ_NUMBER;
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
                    toReturn.insert(current);
                }
                status=READ_TIME;
                current=Subtitle();
                current.setTime(parseInterval(trimmed));
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
        toReturn.insert(current);
    }
    auto validationStatus=toReturn.repairAndValidate(Subtitles::ValidationProblem::OK, 0);
    ok=validationStatus==Subtitles::ValidationProblem::OK;
    return toReturn;
}
Subtitles SrtIO::Read(QTextStream &inputStream) const
{
    bool dummyOk;
    return Read(inputStream, false, dummyOk);
}

bool SrtIO::Try(QTextStream &inputStream) const
{
    bool ok;
    Read(inputStream, true, ok);
    inputStream.seek(0);
    return ok;
}

void SrtIO::Write(QTextStream &outputStream, Subtitles subtitles) const
{
    int id=1;
    for(auto title: subtitles)
    {
        outputStream<<id<<'\n';
        outputStream<<printTime(title.getTime().getStart())<<" --> "<<printTime(title.getTime().getEnd())<<'\n';
        outputStream<<QString(title.getContent()).replace("<br/>","\n")<<"\n\n";
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

Interval SrtIO::parseInterval(QString interval)
{
    timeline.indexIn(interval);
    return Interval(parseTime(timeline.cap(1)), parseTime(timeline.cap(3)));
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
    return QString::number(hours)+":"+QString::number(minutes)+":"+QString::number(seconds)+","+QString::number(milis);
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
    return "SRT file";
}
