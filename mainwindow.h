#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "pda.h"

#include <QTreeWidgetItem>
#include <QMap>
#include <QPair>
#include <QFileDialog>

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

    void testSlot(QTreeWidgetItem* item, int col);

private:
    Ui::MainWindow *ui;
    QTreeWidgetItem* viewRoot{nullptr};

    QVector<QPair<QString, QVector<QString> > > myTree;

    void createTreeView( TreeNode<PDA::Configuration> root, QTreeWidgetItem* parent = nullptr );
    //void buildTree( TreeNode<PDA::Configuration> root );
};
#endif // MAINWINDOW_H
