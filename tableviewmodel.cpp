#include "tableviewmodel.h"

TableViewModel::TableViewModel(QObject *parent)
    :QAbstractTableModel(parent)
{
}

void TableViewModel::Set(Subtitles::iterator low, Subtitles::iterator high)
{
    this->low=low;
    this->high=high;
    QModelIndex topLeft=createIndex(0, 0);
    QModelIndex bottomRight=createIndex(rowCount(), 2);
    emit dataChanged(topLeft, bottomRight);
    emit layoutChanged();
}

int TableViewModel::rowCount(const QModelIndex&) const
{
    return high-low;
}
int TableViewModel::columnCount(const QModelIndex&) const
{
    return 3;
}
QVariant TableViewModel::data(const QModelIndex &index, int role) const
{
    if(role==Qt::DisplayRole)
    {
        Subtitle& current=*(low+index.row());
        switch(index.column())
        {
            case 0:
                return QString(current.getTime().toStringStart());
            case 1:
                return QString(current.getTime().toStringEnd());
            case 2:
                QString text=current.getContent();
                bool theresMore=false;
                if(text.contains("<br/>"))
                {
                    theresMore=true;
                    text=text.left(text.indexOf("<br/>"));
                }
                if(text.length()>DISPLAY_LENGTH_LIMIT)
                {
                    theresMore=true;
                    text=text.left(DISPLAY_LENGTH_LIMIT);
                }
                if(theresMore) text+="...";
                return text;
        }
    }
    return QVariant();
}
QVariant TableViewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation==Qt::Horizontal && role==Qt::DisplayRole)
    {
        switch(section)
        {
            case 0:
                return QString("Start");
            case 1:
                return QString("End");
            case 2:
                return QString("Text");
        }
    }
    return QVariant();
}
Subtitles::iterator TableViewModel::getLow() const
{
    return this->low;
}
Subtitles::iterator TableViewModel::getHigh() const
{
    return this->high;
}
void TableViewModel::update(Subtitles::iterator iter)
{
    int row=(iter)-this->low;
    if(row<0) return;
    if(row>=this->rowCount()) return;
    QModelIndex topLeft=createIndex(row, 0);
    QModelIndex bottomRight=createIndex(row, 2);
    emit dataChanged(topLeft, bottomRight);
}
void TableViewModel::removed()
{
    if(high!=low) high--;
    QModelIndex topLeft=createIndex(0, 0);
    QModelIndex bottomRight=createIndex(rowCount(), 2);
    emit dataChanged(topLeft, bottomRight);
}
