#ifndef SUBTITLEIO_H
#define SUBTITLEIO_H
#include<QString>
#include<vector>
#include<map>
#include<QStringList>
#include <QTextStream>
#include "subtitles.h"
class SubtitleIO
{
public:
    static const SubtitleIO* GetIO(QTextStream &stream, QString extension);
    static const SubtitleIO* GetIO(QString extension, QString filter);
    static QString GetSupportedFiles();
    static std::multimap<QString, const SubtitleIO*> getMap();
    virtual Subtitles Read(QTextStream &inputStream, std::vector<int> params) const=0;
    virtual bool Try(QTextStream &inputStream) const=0;
    virtual void Write(QTextStream &outputStream, Subtitles &subtitles, std::vector<int> params) const=0;
    virtual QString GetDescriptiveName() const = 0;
    virtual QStringList GetSupportedFormats() const = 0;
    virtual QStringList Questions() const;
    virtual ~SubtitleIO(){}
    SubtitleIO(const SubtitleIO&)=delete;
    SubtitleIO& operator=(const SubtitleIO&)=delete;

protected:
    SubtitleIO();
private:
    static const std::multimap<QString, const SubtitleIO*> typeMap;
};

#endif // SUBTITLEIO_H
