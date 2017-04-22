#include "paramquestion.h"
#include "ui_paramquestion.h"

ParamQuestion::ParamQuestion(QWidget *parent, QString question, int minParam, int maxParam) :
    QDialog(parent),
    ui(new Ui::ParamQuestion)
{
    ui->setupUi(this);
    ui->param->setMaximum(maxParam);
    ui->param->setMinimum(minParam);
    ui->label->setText(question);
    setWindowTitle(question);
}

ParamQuestion::~ParamQuestion()
{
    delete ui;
}
bool ParamQuestion::ask(QWidget *parent, QString question, int &returned, int minParam, int maxParam)
{
    ParamQuestion p(parent, question, minParam, maxParam);
    p.exec();
    returned = p.paramVal;
    return p.paramVal>=minParam;
}

void ParamQuestion::on_buttons_accepted()
{
    paramVal=ui->param->value();
    this->close();
}

void ParamQuestion::on_buttons_rejected()
{
    paramVal=ui->param->minimum()-1;
    this->close();
}

void ParamQuestion::on_ParamQuestion_finished(int)
{
    paramVal=-1;
}
