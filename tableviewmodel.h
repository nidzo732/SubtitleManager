#ifndef TABLEVIEWMODEL_H
#define TABLEVIEWMODEL_H
#include<QAbstractTableModel>
#include "subtitles.h"

#define DISPLAY_LENGTH_LIMIT 50

class TableViewModel : public QAbstractTableModel
{
public:
    TableViewModel(QObject *parent);
    void Set(Subtitles::iterator low, Subtitles::iterator high);
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Subtitles::iterator getLow() const;
    Subtitles::iterator getHigh() const;
    void update(Subtitles::iterator);
    void removed();
private:
    Subtitles::iterator low;
    Subtitles::iterator high;
};


#endif // TABLEVIEWMODEL_H
