#include <QTime>
#include "infowindow.h"
#include "ui_infowindow.h"


InfoWindow::InfoWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InfoWindow)
{
    ui->setupUi(this);
}

InfoWindow::~InfoWindow()
{
    delete ui;
}
void InfoWindow::showInfo(QWidget *parent, QString filename, QString fileFormat, int size, int sentences, int duration)
{
    InfoWindow win(parent);
    QString fileNameTooltip=filename;
    QString fileNameText=filename;
    if(fileNameText.length()>MAX_FILENAME_LENGTH) fileNameText=fileNameText.left(MAX_FILENAME_LENGTH-3)+"...";
    win.ui->file->setText(fileNameText);
    win.ui->file->setToolTip(fileNameTooltip);
    win.ui->format->setText(fileFormat);
    win.ui->size->setText(QString::number(size)+" B");
    win.ui->sentences->setText(QString::number(sentences));
    win.ui->duration->setText(QTime::fromMSecsSinceStartOfDay(duration).toString("hh:mm:ss.zzz"));
    win.exec();
}

