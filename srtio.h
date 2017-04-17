#ifndef SRTIO_H
#define SRTIO_H
#include <QTextStream>
#include<QString>
#include "subtitleio.h"
#include"subtitles.h"
#include "subtitle.h"

class SrtIO : public SubtitleIO
{
public:
    virtual Subtitles Read(QTextStream &inputStream) const override;
    virtual bool Try(QTextStream &inputStream) const override;
    virtual void Write(QTextStream &outputStream, Subtitles subtitles) const override;
    virtual QStringList GetSupportedFormats() const override;
    virtual QString GetDescriptiveName() const override;
    virtual ~SrtIO(){}
    static int parseTime(QString time);
    static Interval parseInterval(QString timeline);
    static QString printTime(int milis);
    static const SrtIO* getInstance();
protected:
    SrtIO();
private:
    Subtitles Read(QTextStream &inputStream, bool tryOnly, bool &ok) const;
    static SrtIO* instance;
};

#endif // SRTIO_H
