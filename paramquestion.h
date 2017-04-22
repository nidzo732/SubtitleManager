#ifndef PARAMQUESTION_H
#define PARAMQUESTION_H

#include <QDialog>

namespace Ui {
class ParamQuestion;
}

class ParamQuestion : public QDialog
{
    Q_OBJECT

public:
    explicit ParamQuestion(QWidget *parent, QString question, int minParam, int maxParam);
    static bool ask(QWidget *parent, QString question, int &returned, int minParam=1, int maxParam=100);
    ~ParamQuestion();

private slots:
    void on_buttons_accepted();

    void on_buttons_rejected();

    void on_ParamQuestion_finished(int result);

private:
    Ui::ParamQuestion *ui;
    int paramVal;
};

#endif // PARAMQUESTION_H
