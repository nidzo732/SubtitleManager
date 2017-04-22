#include<vector>
#include "subtitleio.h"
#include "srtio.h"
#include "dvdio.h"
#include "subio.h"
using namespace  std;

SubtitleIO::SubtitleIO()
{

}

const SubtitleIO* SubtitleIO::GetIO(QTextStream &stream, QString extension)
{
    if(typeMap.find(extension)!=typeMap.end())
    {
        auto range=typeMap.equal_range(extension);
        for(auto parser=range.first;parser!=range.second;++parser)
        {
            if(parser->second->Try(stream))
            {
                stream.seek(0);
                return parser->second;
            }
            stream.seek(0);
        }
    }
    stream.seek(0);
    for(auto parser: typeMap)
    {
        if(parser.second->Try(stream)) return parser.second;
    }
    return nullptr;
}
const SubtitleIO* SubtitleIO::GetIO(QString extension, QString filter)
{
    if(typeMap.find(extension)!=typeMap.end())
    {
        auto range=typeMap.equal_range(extension);
        for(auto parser=range.first;parser!=range.second;++parser)
        {
            if(filter.startsWith(parser->second->GetDescriptiveName()))
            {
                return parser->second;
            }
        }
    }
    return nullptr;
}

QString SubtitleIO::GetSupportedFiles()
{
    QString typeString="";
    for(auto title: typeMap)
    {
        typeString.append(title.second->GetDescriptiveName()+" (");
        auto types=title.second->GetSupportedFormats();
        QString format = title.first;
        typeString.append("*."+format);
        typeString.append(")");
        typeString.append(";;");
    }
    typeString.remove(typeString.length()-2,2);
    return typeString;
}
std::multimap<QString, const SubtitleIO*> SubtitleIO::getMap()
{
    vector<const SubtitleIO*> parsers;
    parsers.push_back(SrtIO::getInstance());
    parsers.push_back(DvdIO::getInstance());
    parsers.push_back(SubIO::getInstance());
    multimap<QString, const SubtitleIO*> ioMap;
    for(auto parser: parsers)
    {
        for(auto format:parser->GetSupportedFormats())
        {
            ioMap.insert(std::pair<QString, const SubtitleIO*>(format, parser));
        }
    }
    return ioMap;
}
QStringList SubtitleIO::Questions() const
{
    return QStringList();
}

const std::multimap<QString, const SubtitleIO*> SubtitleIO::typeMap=SubtitleIO::getMap();
