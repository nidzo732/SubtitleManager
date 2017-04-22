#ifndef TITLESPLIT_H
#define TITLESPLIT_H

#include <QDialog>
#include "subtitleprocessing.h"

namespace Ui {
class TitleSplit;
}

class TitleSplit : public QDialog, public SubtitleProcessing
{
    Q_OBJECT

public:
    explicit TitleSplit(QWidget *parent = 0);
    void doWork();
    static Subtitles* doProcess(QWidget *parent, Subtitles *target);
    ~TitleSplit();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::TitleSplit *ui;
    bool accepted;
    int maxLength;
};

#endif // TITLESPLIT_H
