#include "mainwindow.h"
#include <QtWidgets>
#include <queue>

void delay(int i)
{
    QTime dieTime= QTime::currentTime().addSecs(i);
    while( QTime::currentTime() < dieTime )
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    centralWidget = new QWidget(this);
    this->setCentralWidget(centralWidget);
    QGridLayout* layout = new QGridLayout(centralWidget);
    graphicsView = new QGraphicsView();
    layout->addWidget(graphicsView, 0, 0);

    exitButton = new QPushButton("Exit");
    queryButton = new QPushButton("Query");
    queryText = new QLineEdit();
    outputLabel = new QLabel();
    layout->addWidget(outputLabel, 1, 0);
    layout->addWidget(queryButton, 3,0);
    layout->addWidget(queryText, 2, 0);
    layout->addWidget(exitButton, 4, 0);

    connect(exitButton, SIGNAL(clicked()), this, SLOT(on_exitButton_clicked()));
    connect(queryButton, SIGNAL(clicked()), this, SLOT(on_queryButton_clicked()));

    tolerance = 20;

    setWindowTitle("Neural Network");
}

void MainWindow::query(std::string text)
{
    std::queue<Node*> nodes;
    for(std::map<std::string, std::map<std::string, Node*> >::iterator itr = categories_map->begin();
        itr != categories_map->end(); ++itr)
    {
        std::map<std::string, Node*>::iterator jtr = itr->second.find(text);
        if(jtr != itr->second.end())
        {
            nodes.push(jtr->second);
            std::cerr<<"add to queue (initial)\n";
        }
    }

    int max = 0;
    std::vector<Node*> maxNodes;
    std::queue<int> nodesThatAdded;
    nodesThatAdded.push(1);//initial weight of one
    //while we're still looking for maxNodes
    while(nodes.size() > 0 && (max <= getTolerance() || maxNodes.size() < categories_map->size()))//EDIT ME FOR MORE MAXNODES
    {
        Node* n = nodes.front();//get node to propagate
        nodes.pop();

        int weight;
        if(nodesThatAdded.empty()){weight=1;}
        else{weight = nodesThatAdded.front();}
        nodesThatAdded.pop();

        n->setTolerance(n->getTolerance()+weight);//increase tolerance

        if(n->getTolerance() > getTolerance() && std::find(maxNodes.begin(), maxNodes.end(), n) == maxNodes.end() && n->getCluster() != "CENTER")
        {
            std::cerr<<"foundMaxNode: " << n->getX() << "," << n->getY() << "\n";
            max = n->getTolerance();
            maxNodes.push_back(n);
        }

        std::vector<Node*> toAdd = n->getPosConnections();
        for(unsigned int i=0; i<toAdd.size(); ++i)
        {
            if(toAdd[i]->getTolerance() > -(getTolerance()))
            {
                std::cerr<<"add to queue POS" << weight << "\n";
                nodes.push(toAdd[i]);
                nodesThatAdded.push(n->getTolerance());
            }
        }

        if(n->getCluster() != "CENTER")
        {
            std::map<std::string, std::map<std::string, Node*> >::iterator itr = categories_map->find(n->getCluster());
            for(std::map<std::string, Node*>::iterator jtr=itr->second.begin(); jtr!=itr->second.end(); ++jtr)
            {
                if(jtr->second->getTolerance() > -(getTolerance()))
                {
                    std::cerr<<"add to queue NEG\n";
                    nodes.push(jtr->second);
                    int w = n->getTolerance();
                    if(w>0){w=-w;}
                    nodesThatAdded.push(w);
                }
            }
        }
    }
    std::cerr<<maxNodes.size();
    for(unsigned int i=0; i<maxNodes.size(); ++i)
    {
        maxNodes[i]->getVisual()->setBrush(QBrush(Qt::red, Qt::SolidPattern));
        if(maxNodes[i]->getCluster() == "CENTER")
        {
            for(unsigned int j=0; j<maxNodes[i]->getPosConnections().size(); ++j)
            {
                if(maxNodes[i]->getPosConnections()[j]->getCluster() == "NAME")
                {
                    maxNodes[i]->getPosConnections()[j]->getVisual()->setBrush(QBrush(Qt::yellow, Qt::SolidPattern));
                    break;
                }
            }
        }
    }
}


MainWindow::~MainWindow()
{
}

void MainWindow::on_queryButton_clicked()
{
    query(queryText->text().toStdString());
}

void MainWindow::on_exitButton_clicked()
{
    //EXIT BUTTON CLICKED

    for(uint i=0; i<centralNodes->size(); ++i)
    {
        delete (*centralNodes)[i];
     }
    for(std::map<std::string, std::map<std::string, Node*> >::iterator itr=categories_map->begin();
        itr != categories_map->begin(); ++itr)
    {
        for(std::map<std::string, Node*>::iterator jtr=itr->second.begin(); jtr!=itr->second.end(); ++jtr)
        {
            delete jtr->second;
        }
    }
    QApplication::quit();
}
