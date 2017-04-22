#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "subtitles.h"
#include "tableviewmodel.h"

#define STATUSBAR_MESSAGE_DURATION 5000

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void closeEvent(QCloseEvent *event) override;



private slots:
    void on_filterButton_clicked();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionInfo_triggered();

    void on_display_clicked(const QModelIndex &index);

    void on_updateButton_clicked();

    void on_deleteButton_clicked();

    void on_insertButton_clicked();

    void on_actionMerge_small_titles_triggered();

    void on_actionSplit_large_titles_triggered();

    void on_actionShift_triggered();

    void on_actionNew_triggered();

    void on_actionSave_as_triggered();

private:
    Ui::MainWindow *ui;
    Subtitles *currentTitles=new Subtitles();
    bool validateNewTitles(Subtitles *titles);
    bool askForSave();
    void resetTimeFilter();
    TableViewModel tableModel;
    QString format="Unsaved";
    QString selectedForSave="";
    QString saveFilter;
    int size;
    Subtitles::iterator selected;
    bool unsavedChanges=false;
    void blockUi();
    void unblockUi();
};

#endif // MAINWINDOW_H
