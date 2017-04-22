#ifndef TITLEMERGE_H
#define TITLEMERGE_H

#include <QDialog>
#include "subtitleprocessing.h"

namespace Ui {
class TitleMerge;
}

class TitleMerge : public QDialog, public SubtitleProcessing
{
    Q_OBJECT

public:
    explicit TitleMerge(QWidget *parent = 0);
    void doWork();
    static Subtitles* doProcess(QWidget *parent, Subtitles *target);
    ~TitleMerge();

private slots:
    void on_buttonBox_accepted();

    void on_minLength_valueChanged(int arg1);

private:
    Ui::TitleMerge *ui;
    int maxDistance;
    int minLength;
    int maxLength;
    bool accepted=false;
};

#endif // TITLEMERGE_H
