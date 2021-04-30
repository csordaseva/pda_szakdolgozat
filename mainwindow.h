#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QMap>
#include <QPair>
#include <QFileDialog>
#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QList>
#include <QtCore>
#include <QtGui>
#include <QTreeWidget>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include "pda.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void readFromXML(PDA *pda);
    void setTransitionElements(QDomElement root, QString tagname, PDA *pda);
    void setElements(QDomElement root, QString tagname, QString attribute, PDA *pda);

private slots:
    void on_pushButton_load_clicked();

    void on_pushButton_configs_clicked();

    void clickedOnConfigElement(QTreeWidgetItem* item, int col);

    void doubleClickedOnConfigElement(QTreeWidgetItem* item, int col);

    void on_pushButton_configs_manual_clicked();

private:
    Ui::MainWindow *ui;
    QTreeWidgetItem* viewRoot{nullptr};

    //QVector<QPair<QString, QVector<QString> > > myTree;

    void createTreeView( TreeNode<PDA::Configuration> root, QTreeWidgetItem* parent = nullptr );
    void createTreeViewByClicking( TreeNode<PDA::Configuration> root, QTreeWidgetItem* parent);
    //void buildTree( TreeNode<PDA::Configuration> root );
    TreeNode<PDA::Configuration> searchByConfigName(TreeNode<PDA::Configuration> node, QString config);
};
#endif // MAINWINDOW_H
