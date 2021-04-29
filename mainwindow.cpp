#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pda.h"

using State = int;

PDA pda;
QGraphicsScene* scene;

bool itemLess(const QGraphicsItem* item1, const QGraphicsItem* item2)
{
    return item1->sceneBoundingRect().center().y() < item2->sceneBoundingRect().center().y();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene();
    QGraphicsScene* scene2 = new QGraphicsScene();
    ui->graphicsView->setScene(scene);
    ui->pdaView->setScene(scene2);
    QImage* image = new QImage("D:/Letöltések/pda_q1.dot.svg");
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(*image));
    scene2->addItem(item);
    ui->pdaView->show();

    connect(ui->pushButton_close, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(doubleClickedOnConfigElement(QTreeWidgetItem*, int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*PDA pda_test_empty() {
    return PDA(
        { 1,2 },
        1,
        { 2 },
        {
            { 1,{},{2}, "", "S" },
            { 2,{},{2}, "S", "aSb" },
            { 2,{},{2}, "S", "BB" },
            { 2,{},{2}, "S", "A" },

            { 2,{},{2}, "B", "bB" },
            { 2,{},{2}, "B", "x" },

            { 2,{},{2}, "A", "aAa" },
            { 2,{},{2}, "A", "aSB" },
            { 2,{},{2}, "A", "a" },

            { 2,{'a'},{2}, "a", "" },
            { 2,{'b'},{2}, "b", "" },
            { 2,{'x'},{2}, "x", "" }
        }
        );
}*/

void MainWindow::setElements(QDomElement root, QString tagname, QString attribute, PDA *pda){
    QDomNodeList items = root.elementsByTagName(tagname);

   //qDebug() << "(Total items: " << items.count() << ")";

    bool convertOK;
    int data;

    for(int i = 0; i < items.count(); i++)
    {

        QDomNode itemnode = items.at(i);

        if(itemnode.isElement())
        {

            QDomElement itemele = itemnode.toElement();

            //qDebug() <<itemele.attribute(attribute);

            if(itemele.tagName() == "state")
            {
               data = itemele.attribute(attribute).toInt(&convertOK);

               if(convertOK)
               {
                   pda->setStates(data);
               }

            }
            else if(itemele.tagName() == "start_state")
            {
                convertOK = false;

                data = itemele.attribute(attribute).toInt(&convertOK);

                if(convertOK)
                {
                       pda->setStartState(data);
                       //qDebug() << "data(startState):" << data;
                }

             }

            else if(itemele.tagName() == "accept_state")
            {

                convertOK = false;

                data = itemele.attribute(attribute).toInt(&convertOK);

                if(convertOK)
                {
                       pda->setAcceptStates(data);
                       /*for(auto s : pda->getAcceptStates())
                         {
                           qDebug() << "pda->getAccept_states element: " << s;
                         }*/
                }
            }
        }
    }
}

void MainWindow::setTransitionElements(QDomElement root, QString tagname, PDA *pda){

    bool convertOK = false;
    int data;
    QDomNodeList items = root.elementsByTagName(tagname);

    for(int i = 0; i < items.count(); i++)
    {
        PDA::Transition t;
        QDomNode itemnode = items.at(i);

        if(itemnode.isElement())
        {

            QDomElement itemele = itemnode.toElement();

            data=itemele.attribute("from").toInt(&convertOK);
            if(convertOK)
            {
                t.from = data;
                //qDebug() << "from:" << t.from;
            }
            convertOK = false;

            t.symbols=itemele.attribute("symbol").toStdString();

            //qDebug() << "symbol:" << QString::fromStdString(t.symbols);

            data=itemele.attribute("to").toInt(&convertOK);
            if(convertOK)
            {
                t.to.push_back(data);
                //qDebug() << "to:" << t.to;
            }
            convertOK = false;

            t.pop=itemele.attribute("pop").toStdString();
            //qDebug() << "pop:" << QString::fromStdString(t.pop);
            t.push=itemele.attribute("push").toStdString();
            //qDebug() << "push:" << QString::fromStdString(t.push);
            pda->setTransitions(t);
        }
    }
}

void MainWindow::readFromXML(PDA *pda)
{
    QDomDocument document;
    QString selfilter = tr("XML files (*.xml)");
    QString filename = QFileDialog::getOpenFileName(this, "Open file", "C://", selfilter);
    QFile file(filename);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open file.";
    }
    else if(!document.setContent(&file))
    {
        qDebug() << "Failed to load document.";
        file.close();
    }
    else
    {
        QDomElement root = document.firstChildElement();

        //qDebug() << "States:";

        setElements(root,"state", "name", pda);

        //qDebug() << "Start state:";

        setElements(root, "start_state", "name", pda);

        //qDebug() << "Accept state(s):";

        setElements(root, "accept_state", "name", pda);

        //qDebug() << "Transitions:";

        setTransitionElements(root, "transition", pda);
    }

    //TODO:?
    /*if(pda == nullptr)
        QMessageBox::critical(this, "Error", "PDA could not be loaded from file.");
    else
        QMessageBox::information(this, "Loading file", "PDA successfully loaded from file.");*/
}

void MainWindow::on_pushButton_load_clicked()
{
    readFromXML(&pda);
    QMessageBox::information(this, "Loading file", "PDA successfully loaded from file.");

    /*qDebug() << "pda->getStartState: " << pda.getStartState() << "\n";
    qDebug() << "pda->getStates: ";
    for(auto a : pda.getStates()){
        qDebug() << a;
    }
    qDebug() << "\npda->getAcceptStates: ";
    for(auto a : pda.getAcceptStates()){
        qDebug() << a << "\n";
    }
    for(auto t : pda.getTransitions()){
        qDebug() << "from: " << t.from;
        for(auto u : t.to){
            qDebug() << "to: " << u;
        }
        qDebug() << "with: " << QString::fromStdString(t.symbols);
        qDebug() << "push: " << QString::fromStdString(t.push);
        qDebug() << "pop: " << QString::fromStdString(t.pop);
        qDebug() << "-------------";
    }*/
}

void MainWindow::on_pushButton_configs_clicked()
{
    QString word;
    word = ui->inputWord->text();
    TreeNode<PDA::Configuration> tree = pda.getConfigurationTree(word.toStdString());


    //qDebug() << "\nAll of the possible configurations:\n";
    tree.printRecursive();
    createTreeView(tree);
    qDebug() << word << ": " << (pda.isAccepted(word.toStdString()) ? "accepted" : "not accepted") << Qt::endl;
    if(pda.isAccepted(word.toStdString()))
    {
        ui->statusbar->showMessage("Word \"" + word + "\" is accepted by the automaton.");
    }
    else
    {
        ui->statusbar->showMessage("Word \"" + word + "\" is not accepted by the automaton.");
    }
   pda.toDot("pda_proba.dot");
   qDebug() << "dot file created";
   //pda.saveImageGV("pda_proba");
}

void MainWindow::doubleClickedOnConfigElement(QTreeWidgetItem* item, int col)
{
    int distance = 100;
    int childDistance = 100;
    scene->clear();
    QTreeWidgetItem* originalItem = item;    
    while(item!=viewRoot->parent())
    {
        //QGraphicsRectItem* actRect = new QGraphicsRectItem(0,0,100,30);
        //scene->addItem(actRect);
        QGraphicsTextItem* text = new QGraphicsTextItem(item->text(col));
        if(item == originalItem)
        {
            text->setHtml("<h1><b><div style='background-color:yellow; text-align: center'>"+item->text(col)+"</div></b></h1>");
        }
        else
        {
            text->setHtml("<h1><b><div style='background-color:#83f52c; text-align: center'>"+item->text(col)+"</div></b></h1>");
        }

        scene->addItem(text);
        //gyerekeit
        if(item == originalItem)
        {
            QList<QTreeWidgetItem*> children = item->takeChildren();
            for(auto child : children)
            {

              //QGraphicsRectItem* actChildRect = new QGraphicsRectItem(0,20,100,30);
              //actChildRect->setBrush(QBrush(Qt::red, Qt::SolidPattern));
              //scene->addItem(actChildRect);
                QGraphicsTextItem* actChildText = new QGraphicsTextItem(child->text(col));
                actChildText->setHtml("<h1><b><div style='background-color:red; text-align:center'>"+child->text(col)+"</div></b></h1>");
                scene->addItem(actChildText);
                actChildText->setDefaultTextColor(Qt::white);

                childDistance += 60;
                //actChildRect->setPos(0, 20+childDistance);
                actChildText->setPos(0, 40+childDistance);
            }
            item->addChildren(children);
        }
        //actRect->setPos(0, distance);
        text->setPos(0, distance);
        item = item->parent();
        distance -= 100;
    }
    QList<QGraphicsItem *> rects = scene->items(Qt::AscendingOrder);
    std::sort(rects.begin(), rects.end(), itemLess);
    QPoint start, end;
    for(int i = 0; i < rects.size()-1; i++)
    {
        //meg kell különböztetni a gyerekelemeket
        start = QPoint(rects.at(i)->scenePos().x()+65, rects.at(i)->scenePos().y()+rects.at(0)->boundingRect().height());
        end = QPoint(rects.at(i+1)->scenePos().x()+65, rects.at(i+1)->scenePos().y());
        scene->addLine(QLine(start, end));
    }
}

void MainWindow::createTreeView( TreeNode<PDA::Configuration> root, QTreeWidgetItem* parent )
{
    QTreeWidgetItem* tmp;
    if( parent == nullptr )
    {
        viewRoot = new QTreeWidgetItem(ui->treeWidget, QStringList(QString(root.data.createOutputString())));
        tmp = viewRoot;
    }
    else
    {
        tmp = new QTreeWidgetItem(parent, QStringList(QString(root.data.createOutputString())));
    }

    for (auto child : root.children)
    {
        createTreeView(child, tmp);
    }
}
