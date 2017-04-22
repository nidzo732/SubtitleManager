#ifndef SubIO_H
#define SubIO_H
#include <QTextStream>
#include<QString>
#include "subtitleio.h"
#include"subtitles.h"
#include "subtitle.h"

class SubIO : public SubtitleIO
{
public:
    virtual Subtitles Read(QTextStream &inputStream,  std::vector<int> params) const override;
    virtual bool Try(QTextStream &inputStream) const override;
    virtual void Write(QTextStream &outputStream, Subtitles &subtitles, std::vector<int> params) const override;
    virtual QStringList GetSupportedFormats() const override;
    virtual QString GetDescriptiveName() const override;
    virtual ~SubIO(){}
    static Interval parseInterval(QString timeline, int &prevEnd, int seq);
    static const SubIO* getInstance();
protected:
    SubIO();
private:
    Subtitles Read(QTextStream &inputStream, bool tryOnly, bool &ok, std::vector<int> params) const;
    static SubIO* instance;
};

#endif // SubIO_H
