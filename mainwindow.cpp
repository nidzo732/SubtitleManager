#include<iostream>
#include<QFileDialog>
#include<QMessageBox>
#include<QCloseEvent>
#include<QRegExp>
#include<QFile>
#include<vector>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "subtitleio.h"
#include "paramquestion.h"
#include "titlemerge.h"
#include "titlesplit.h"
#include "titleshift.h"
#include "infowindow.h"

using namespace  std;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    tableModel(this)
{
    ui->setupUi(this);
    ui->display->setModel(&tableModel);
    ui->display->horizontalHeader()->setStretchLastSection(true);
    blockUi();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_filterButton_clicked()
{
    if(this->currentTitles!=nullptr)
    {
        if(ui->filterAfter->time().msecsSinceStartOfDay()==0 &&
                ui->filterBefore->time().msecsSinceStartOfDay()==0)
        {
            auto begin=currentTitles->begin();
            auto end = currentTitles->end();
            tableModel.Set(begin, end);
        }
        else
        {
            auto begin=currentTitles->after(ui->filterAfter->time().msecsSinceStartOfDay());
            auto end = currentTitles->before(ui->filterBefore->time().msecsSinceStartOfDay());
            tableModel.Set(begin, end);
        }
    }
}

void MainWindow::on_actionOpen_triggered()
{
    QString selectedFilter;
    if(currentTitles!=nullptr && unsavedChanges && !askForSave()) return;
    QString selected = QFileDialog::getOpenFileName(this, "Open file", "", SubtitleIO::GetSupportedFiles(), &selectedFilter);
    if(selected.length()==0) return;
    QFile file(selected);
    if(!file.open(QFile::ReadOnly|QFile::Text))
    {
        QMessageBox::critical(this, "Error", "Can't open file: "+file.errorString());
        return;
    }
    if(QFileInfo(selected).size()==0)
    {
        Subtitles *subtitles;
        subtitles=new Subtitles();
        file.close();
        if(validateNewTitles(subtitles))
        {
            format="Empty file";
            size=0;
        }
    }
    else
    {
        QTextStream stream(&file);
        stream.setCodec("UTF-8");
        stream.setAutoDetectUnicode(true);
        QString fileType=selected.right(selected.length()-selected.lastIndexOf(".")-1);
        auto io=SubtitleIO::GetIO(stream,fileType);
        if(!io)
        {
            QMessageBox::critical(this, "Error", "This file format is not supported");
            file.close();
            return;
        }
        vector<int> params;
        for(auto question:io->Questions())
        {
            int answer;
            if(!ParamQuestion::ask(this, question, answer))
            {
                file.close();
                return;
            }
            params.push_back(answer);
        }
        Subtitles titles=io->Read(stream, params);
        file.close();
        if(validateNewTitles(new Subtitles(titles)))
        {
            resetTimeFilter();
            format=io->GetDescriptiveName();
            size=QFileInfo(selected).size();
            unblockUi();
            ui->statusBar->showMessage("Opened", STATUSBAR_MESSAGE_DURATION);
            selectedForSave=selected;
            saveFilter=selectedFilter;
        }
    }
}

void MainWindow::on_actionSave_triggered()
{
    QString selectedFilter;
    QString selected=selectedForSave;
    if(selected.length()==0) selected=QFileDialog::getSaveFileName(this, "Save file","", SubtitleIO::GetSupportedFiles(), &selectedFilter);
    else selectedFilter=saveFilter;
    if(selected.length()==0) return;
    QString fileType=selected.right(selected.length()-selected.lastIndexOf(".")-1);
    auto io=SubtitleIO::GetIO(fileType, selectedFilter);
    if(io==nullptr)
    {
        QMessageBox::critical(this, "Error", "This file format is not supported");
        return;
    }
    QFile file(selected);
    if(!file.open(QFile::WriteOnly|QFile::Text))
    {
        QMessageBox::critical(this, "Error", "Can't open file: "+file.errorString());
        return;
    }
    vector<int> params;
    for(auto question:io->Questions())
    {
        int answer;
        if(!ParamQuestion::ask(this, question, answer))
        {
            return;
            file.close();
        }
        params.push_back(answer);
    }
    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    io->Write(stream, *currentTitles, params);
    file.close();
    format=io->GetDescriptiveName();
    size=QFileInfo(selected).size();
    unsavedChanges=false;
    ui->statusBar->showMessage("Saved", STATUSBAR_MESSAGE_DURATION);
    selectedForSave=selected;
    saveFilter=selectedFilter;
}
bool MainWindow::validateNewTitles(Subtitles *titles)
{
    Subtitles::ValidationProblem status;
    int toRepair=Subtitles::OK;
    while((status=titles->repairAndValidate(toRepair))!=Subtitles::OK)
    {
        if(status==Subtitles::PARSE_FAILURE)
        {
            QMessageBox::critical(this, "Error", "This file contains too many errors, parsing impossible");
            delete titles;
            return false;
        }
        if(status&toRepair)
        {
            delete titles;
            QMessageBox::critical(this, "Error", "Erors could not be fixed");
            return false;
        }
        auto button=QMessageBox::question(this, "Problem", "This file has a following problem: "+Subtitles::getProblemDescription(status)+"\nDo you want to try an automatic repair?", QMessageBox::Yes, QMessageBox::No);
        if(button==QMessageBox::Yes)
        {
            toRepair|=status;
        }
        else
        {
            delete titles;
            QMessageBox::critical(this, "Error", "Loading canceled");
            return false;
        }
    }
    if(this->currentTitles) delete this->currentTitles;
    this->currentTitles=titles;
    this->ui->actionSave->setEnabled(true);
    this->ui->actionInfo->setEnabled(true);
    this->tableModel.Set(currentTitles->begin(), currentTitles->end());
    this->selected=currentTitles->end();
    unsavedChanges=false;
    return true;
}

void MainWindow::on_actionInfo_triggered()
{
    if(currentTitles==nullptr) return;
    int totalLength;
    if(currentTitles->end()!=currentTitles->begin())
        totalLength=(*(currentTitles->end()-1)).getTime().getEnd()-(*(currentTitles->begin())).getTime().getStart();
    else
        totalLength=0;
    int count=currentTitles->end()-currentTitles->begin();
    InfoWindow::showInfo(this, selectedForSave, format, size, count, totalLength);
}

void MainWindow::on_display_clicked(const QModelIndex &index)
{
    int row=index.row();
    Subtitle &title= *(tableModel.getLow()+row);
    ui->editText->setText(title.getContent());
    ui->editTimeStart->setTime(QTime::fromMSecsSinceStartOfDay(title.getTime().getStart()));
    ui->editTimeEnd->setTime(QTime::fromMSecsSinceStartOfDay(title.getTime().getEnd()));
    selected=tableModel.getLow()+row;
}

void MainWindow::on_updateButton_clicked()
{
    if(currentTitles==nullptr || selected==currentTitles->end()) return;
    Subtitle oldTitle=*selected;
    Interval newTime(ui->editTimeStart->time().msecsSinceStartOfDay(), ui->editTimeEnd->time().msecsSinceStartOfDay());
    if(oldTitle.getTime()==newTime)
    {
        (*selected).setContent(ui->editText->toPlainText().replace("\n","<br/>"));
        tableModel.update(selected);
        unsavedChanges=true;
    }
    else
    {
        int selectedIndex=selected-currentTitles->begin();
        Subtitles *backup=new Subtitles(*currentTitles);
        currentTitles->remove(selected);
        Subtitle newTitle(newTime, ui->editText->toPlainText().replace("\n","<br/>"));
        Subtitles::iterator insertedAt;
        Subtitles::ValidationProblem status;
        if((status=currentTitles->insert(newTitle, true, insertedAt))!=Subtitles::OK)
        {
            delete currentTitles;
            currentTitles=backup;
            selected=currentTitles->begin()+selectedIndex;
            on_filterButton_clicked();
            QMessageBox::critical(this, "Problem", "Can't modify: "+Subtitles::getProblemDescription(status));
        }
        else
        {
            delete backup;
            selected=insertedAt;
            on_filterButton_clicked();
            unsavedChanges=true;
        }
    }
}

void MainWindow::on_deleteButton_clicked()
{
    if(currentTitles!=nullptr && selected!=currentTitles->end())
    {
        currentTitles->remove(selected);
        selected=currentTitles->end();
        unsavedChanges=true;
        on_filterButton_clicked();
    }
}

void MainWindow::on_insertButton_clicked()
{
    Subtitles *backup=new Subtitles(*currentTitles);
    Interval newTime(ui->editTimeStart->time().msecsSinceStartOfDay(), ui->editTimeEnd->time().msecsSinceStartOfDay());
    Subtitle newTitle(newTime, ui->editText->toPlainText().replace("\n","<br/>"));
    Subtitles::iterator insertedAt;
    Subtitles::ValidationProblem status;
    if((status=currentTitles->insert(newTitle, true, insertedAt))!=Subtitles::OK)
    {
        delete currentTitles;
        currentTitles=backup;
        selected=currentTitles->end();
        on_filterButton_clicked();
        QMessageBox::critical(this, "Problem", "Can't modify: "+Subtitles::getProblemDescription(status));
    }
    else
    {
        delete backup;
        selected=insertedAt;
        on_filterButton_clicked();
        unsavedChanges=true;
    }
}

void MainWindow::on_actionMerge_small_titles_triggered()
{
    if(currentTitles==nullptr) return;
    Subtitles *newOnes=TitleMerge::doProcess(this, currentTitles);
    if(newOnes!=nullptr)
    {
        delete currentTitles;
        currentTitles=newOnes;
        selected=currentTitles->end();
        on_filterButton_clicked();
        unsavedChanges=true;
    }
}

void MainWindow::on_actionSplit_large_titles_triggered()
{
    if(currentTitles==nullptr) return;
    Subtitles *newOnes=TitleSplit::doProcess(this, currentTitles);
    if(newOnes!=nullptr)
    {
        delete currentTitles;
        currentTitles=newOnes;
        selected=currentTitles->end();
        on_filterButton_clicked();
        unsavedChanges=true;
    }
}

void MainWindow::on_actionShift_triggered()
{
    if(currentTitles==nullptr || currentTitles->end()==currentTitles->begin()) return;
    if(currentTitles==nullptr) return;
    Subtitles *newOnes=TitleShift::doProcess(this, currentTitles);
    if(newOnes!=nullptr)
    {
        delete currentTitles;
        currentTitles=newOnes;
        selected=currentTitles->end();
        on_filterButton_clicked();
        unsavedChanges=true;
    }
}
bool MainWindow::askForSave()
{
    auto answer=QMessageBox::warning(this, "Unsaved changes", "Unsaved changes exist", QMessageBox::Save|QMessageBox::Discard|QMessageBox::Cancel);
    if(answer&QMessageBox::Cancel) return false;
    else if(answer&QMessageBox::Save) on_actionSave_triggered();
    return true;

}

void MainWindow::on_actionNew_triggered()
{
    if(currentTitles!=nullptr && unsavedChanges && !askForSave()) return;
    delete currentTitles;
    currentTitles=new Subtitles();
    selected=currentTitles->end();
    unblockUi();
    resetTimeFilter();
    on_filterButton_clicked();
    selectedForSave="";
}
void MainWindow::blockUi()
{
    ui->actionInfo->setEnabled(false);
    ui->actionSave->setEnabled(false);
    ui->actionMerge_small_titles->setEnabled(false);
    ui->actionShift->setEnabled(false);
    ui->actionSplit_large_titles->setEnabled(false);
    ui->updateButton->setEnabled(false);
    ui->insertButton->setEnabled(false);
    ui->deleteButton->setEnabled(false);
    ui->filterButton->setEnabled(false);
    ui->actionSave_as->setEnabled(false);
}

void MainWindow::unblockUi()
{
    ui->actionInfo->setEnabled(true);
    ui->actionSave->setEnabled(true);
    ui->actionMerge_small_titles->setEnabled(true);
    ui->actionShift->setEnabled(true);
    ui->actionSplit_large_titles->setEnabled(true);
    ui->updateButton->setEnabled(true);
    ui->insertButton->setEnabled(true);
    ui->deleteButton->setEnabled(true);
    ui->filterButton->setEnabled(true);
    ui->actionSave_as->setEnabled(true);
}
void MainWindow::resetTimeFilter()
{
    if(currentTitles!=nullptr && currentTitles->end()-currentTitles->begin()!=0)
    {
        ui->filterAfter->setTime(QTime::fromMSecsSinceStartOfDay((*(currentTitles->begin())).getTime().getStart()));
        ui->filterBefore->setTime(QTime::fromMSecsSinceStartOfDay((*(currentTitles->end()-1)).getTime().getEnd()));
    }
}
void MainWindow::closeEvent (QCloseEvent *event)
{
    if(currentTitles!=nullptr && unsavedChanges && !askForSave())
    {
        event->ignore();
    }
    else
    {
        event->accept();
    }
}

void MainWindow::on_actionSave_as_triggered()
{
    selectedForSave="";
    on_actionSave_triggered();
}
