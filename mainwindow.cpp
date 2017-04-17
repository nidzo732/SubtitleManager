#include<iostream>
#include<QFileDialog>
#include<QMessageBox>
#include<QRegExp>
#include<QFile>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "subtitleio.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_filterButton_clicked()
{
}

void MainWindow::on_actionOpen_triggered()
{
    QString selected = QFileDialog::getOpenFileName(this, "Open file", "", SubtitleIO::GetSupportedFiles());
    QFile file(selected);
    file.open(QFile::ReadOnly|QFile::Text);
    QTextStream stream(&file);
    QString fileType=selected.right(selected.length()-selected.lastIndexOf(".")-1);
    auto io=SubtitleIO::GetIO(stream,fileType);
    auto subittles=io->Read(stream);
    subittles.repairAndValidate(Subtitles::ValidationProblem::INTERVALS_OVERLAP|Subtitles::ValidationProblem::UNKNOWN_TIMES);
    currentTitles=new Subtitles(subittles);
    /*QString TIME_REGEX=R"(((?:\d+\:\d+\:\d+\,\d+)?)((?:\-*\>?)?)((?:\d+\:\d+\:\d+\,\d+)?))";
    QRegExp rx(TIME_REGEX);
    QString target="----->00:00:43,280";
    int z=rx.indexIn(target);
    QString r1=rx.cap(0), r2=rx.cap(1), r3=rx.cap(2), r4=rx.cap(3);
    z=z+1;*/
}

void MainWindow::on_actionSave_triggered()
{
    //QString selected=QFileDialog::getSaveFileName()
}
