#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QSortFilterProxyModel>
#include <QTimer>
#include "share_files.h"
#include "delegate.h"
#include "dir_model.h"
#include "file_model.h"

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

    void on_treeView_customContextMenuRequested(const QPoint &pos);

private:
    Ui::MainWindow *ui;
    Session m_sf;
    DirectoryModel* m_model;
    FileModel*      m_fileModel;
    QSortFilterProxyModel* m_sorter;
    QSortFilterProxyModel* m_dir_sorter;
    QMenu*      m_dir_menu;
    QAction*    shareDir;
    QAction*    shareDirR;
    QAction*    unshareDir;
    QAction*    unshareDirR;
    QAction*    removeD;
    QAction*    addFile;
    CheckBoxDelegate* m_cbd;

    QMenu*     m_file_menu;
    QAction*   removeF;
    Delay      m_delay;
private slots:
    void shareDirectory();
    void shareDirectoryR();
    void unshareDirectory();
    void unshareDirectoryR();
    void removeFile();
    void removeDir();
    void processaddFile();
    void on_deleteButton_clicked();
    void on_addButton_clicked();
    void on_tableView_customContextMenuRequested(const QPoint &pos);
    void on_pushButton_clicked();

    void testOnDeleteSlot(const QModelIndex&, int, int);
    void sortChanged(int, Qt::SortOrder);
    void dir_sortChanged(int, Qt::SortOrder);
    void on_pushButton_2_clicked();
};

#endif // MAINWINDOW_H
