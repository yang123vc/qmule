#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "share_files.h"
#include "tree.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_treeView_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    Session m_sf;
    TreeModel* m_model;
    TreeModel* m_fileModel;
};

#endif // MAINWINDOW_H
