#include<vector>
#include "subtitleio.h"
#include "srtio.h"
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
QString SubtitleIO::GetSupportedFiles()
{
    QString typeString="Subtitles (";
    for(auto title: typeMap)
    {
        typeString.append("*.");
        typeString.append(title.first);
        typeString.append(" ");
    }
    typeString.remove(typeString.length()-1, 1);
    typeString.append(")");
    return typeString;
}
std::multimap<QString, const SubtitleIO*> SubtitleIO::getMap()
{
    vec
    multimap<QString, const SubtitleIO*> ioMap;
    ioMap.insert(std::pair<QString, const SubtitleIO*>("srt", SrtIO::getInstance()));
    return ioMap;
}

const std::multimap<QString, const SubtitleIO*> SubtitleIO::typeMap=SubtitleIO::getMap();
