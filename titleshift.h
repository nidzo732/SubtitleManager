#ifndef TitleShift_H
#define TitleShift_H

#include <QDialog>
#include "subtitleprocessing.h"

namespace Ui {
class TitleShift;
}

class TitleShift : public QDialog, public SubtitleProcessing
{
    Q_OBJECT

public:
    explicit TitleShift(QWidget *parent, Subtitles *target);
    void doWork();
    static Subtitles* doProcess(QWidget *parent, Subtitles *target);
    ~TitleShift();

private slots:
    void on_buttonBox_accepted();

    void on_start1_timeChanged(const QTime &time);

    void on_end1_timeChanged(const QTime &time);

    void on_start2_timeChanged(const QTime &time);

    void on_end2_timeChanged(const QTime &time);

private:
    int transfer(int t);
    Ui::TitleShift *ui;
    bool accepted;
    int start1, end1;
    int start2, end2;
    int length1, length2;
};

#endif // TitleShift_H
