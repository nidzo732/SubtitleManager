#ifndef INFOWINDOW_H
#define INFOWINDOW_H

#include <QDialog>
#include <QString>
#include <QWidget>

#define MAX_FILENAME_LENGTH 40

namespace Ui {
class InfoWindow;
}

class InfoWindow : public QDialog
{
    Q_OBJECT

public:
    explicit InfoWindow(QWidget *parent = 0);
    static void showInfo(QWidget *parent, QString filename, QString fileFormat, int size, int sentences, int duration);
    ~InfoWindow();

private:
    Ui::InfoWindow *ui;
};

#endif // INFOWINDOW_H
