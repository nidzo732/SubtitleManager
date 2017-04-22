#ifndef DvdIO_H
#define DvdIO_H
#include <QTextStream>
#include<QString>
#include "subtitleio.h"
#include"subtitles.h"
#include "subtitle.h"

class DvdIO : public SubtitleIO
{
public:
    virtual Subtitles Read(QTextStream &inputStream, std::vector<int> params) const override;
    virtual bool Try(QTextStream &inputStream) const override;
    virtual void Write(QTextStream &outputStream, Subtitles &subtitles, std::vector<int> params) const override;
    virtual QStringList GetSupportedFormats() const override;
    virtual QString GetDescriptiveName() const override;
    virtual QStringList Questions() const override;
    virtual ~DvdIO(){}
    static const DvdIO* getInstance();
protected:
    DvdIO();
private:
    Subtitles Read(QTextStream &inputStream, bool tryOnly, bool &ok, std::vector<int> params) const;
    static DvdIO* instance;
};

#endif // DvdIO_H
