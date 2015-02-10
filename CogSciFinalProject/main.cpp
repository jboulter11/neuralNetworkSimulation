#include "mainwindow.h"
#include "Node.h"
#include <QApplication>
#include <fstream>
#include <map>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <QGraphicsView>

typedef unsigned int uint;

void connect(Node* a, Node* b, std::string type)
{
    a->addConnection(b, type);
    b->addConnection(a, type);
}

uint getClusterHeight(uint i)
{
    return i/2;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    std::ifstream iStr(argv[1]);

    if(!iStr){std::cout<< "Not Gucci."; exit(1);}
    else {std::cout<< "It's Gucci.\n";}

    std::string word;
    //Map of categories to their nodes (ie jobs to cashier, programmer, engineer)
    //Inner map is node text(cashier to cashier node) to Node* obj
    std::map<std::string, std::map<std::string, Node*> > categories_map;
    std::vector<std::string> cats; //keeps order of the categories
    std::vector<Node*> centralNodes;

    //take in the types of nodes we'll be getting
    while(iStr >> word)
    {
        std::cerr << "Reading in cats\n";
        if(word != ".END.")//if we've taken in all the categories
        {
            cats.push_back(word);
        }
        else
        {
            break;
        }
    }

    bool done = 0;
    std::vector<Node*> line; //the terms to connect at the end of the line
    //take in nodes and tie them together with a central node representing the entity
    while(!done)
    {
        std::cerr<< "Reading in lines of data\n";
        for(uint i=0; i<cats.size(); ++i)
        {
            std::cerr<< "Inner for loop of data read 1\n";
            iStr >> word;
            std::cerr<< word << "\n";
            if(i == 0 && word == ".END.") //we're done with taking in data
            {
                std::cerr<< "Finished taking in nodes!\n";
                done = 1;
                break;
            }
            else if(i != 0 && word == ".END.")
            {
                std::cerr<< "Lines don't match up w/ num of categories!\n";
                exit(1);
            }
            //we're still taking in data
            //make a node for each term that comes in
            Node* n = categories_map[cats[i]][word];
            if(n == NULL)
            {
                categories_map[cats[i]][word] = new Node;
                categories_map[cats[i]][word]->setCluster(cats[i]);
            }
            line.push_back(categories_map[cats[i]][word]);
        }
        if(done){break;}
        //connect Nodes from the line to central nodes
        centralNodes.push_back(new Node);
        centralNodes.back()->setCluster("CENTER");
        for(uint i=0; i<line.size(); ++i)
        {
            std::cerr<< "Inner for loop of data read 2\n";
            connect(line[i], centralNodes.back(), POS);
        }
        line.clear();
    }

    //Display our nodes//

    const int bigRows = getClusterHeight(cats.size());
    int bigvOffset = 500;
    int bighOffset = 700;
    QGraphicsScene* scene = new QGraphicsScene();
    std::vector<int> clusterHeights;
    std::vector<int> clusterWidths;
    QSize size = w.getGraphicsView()->size();
    const int widthOfContainer = size.width();
    int currentRow = 1;
    for(uint i=0; i<cats.size(); ++i)
    {
        //for each cluster, assign a position (circle-like)
        clusterHeights.push_back(currentRow*bigvOffset + bigvOffset);
        if(i % 2 == 0)
        {
            if(currentRow > bigRows / 2)
            {
                clusterWidths.push_back(widthOfContainer / 2 - bighOffset*i
                                        + (((currentRow - (bigRows / 2)) * 2*bighOffset)-bighOffset));
            }
            else
                clusterWidths.push_back(widthOfContainer / 2 - bighOffset*i);
        }
        else
        {
            if(currentRow > bigRows / 2)
            {
                clusterWidths.push_back(widthOfContainer / 2 + bighOffset*i
                                        - (((currentRow - (bigRows / 2)) * 2*bighOffset)-bighOffset));
            }
            else
                clusterWidths.push_back(widthOfContainer / 2 + bighOffset*i);
            ++currentRow;
        }
    }
    int centralHeight = (bigRows) * bigvOffset + bigvOffset/2;
    int centralWidth = (bigRows/2) * bighOffset - bighOffset/4;

    //Start placing each rectItem
    for(uint i=0; i<cats.size(); ++i)
    {
        std::cerr<<"Past taking in Data.\n";
        int vOffset = 40;
        int hOffset = 70;
        currentRow = 0;
        std::map<std::string, Node*>* nodeMap = &categories_map[cats[i]];
        const int rows = getClusterHeight(nodeMap->size());
        int startX = clusterWidths[i];
        int startY = clusterHeights[i]-(rows/2*vOffset);
        int x = startX;
        int y = startY;

        uint j=0;
        for(std::map<std::string, Node*>::iterator itr = nodeMap->begin();
            itr != nodeMap->end(); ++itr, ++j)
        {
            std::cerr<<"Inner Loop, making rects.\n";

            y = startY + currentRow*vOffset;
            x = startX - j*hOffset;
            if(j % 2 == 0)
            {
                x = startX - currentRow*hOffset - hOffset;
                if(currentRow > rows / 2)
                {
                    x+= (((currentRow - (rows / 2))) * 2*hOffset)-hOffset;
                }
            }
            else
            {
                x = startX + currentRow*hOffset + hOffset;
                if(currentRow > rows / 2)
                {
                    x-= (((currentRow - (rows / 2))) * 2*hOffset)-hOffset;
                }
                ++currentRow;
            }
            itr->second->setVisual(new QGraphicsRectItem(x, y, hOffset-10, vOffset-10));
            itr->second->getVisual()->setBrush(QBrush(Qt::green, Qt::SolidPattern));
            itr->second->setX(x);
            itr->second->setY(y);
            scene->addItem(itr->second->getVisual());

            QGraphicsTextItem* t = new QGraphicsTextItem();
            t->setPos(x, y);
            t->setPlainText(QString((itr->first).c_str()));
            scene->addItem(t);
        }
    }

    currentRow = 0;
    for(uint i=0; i<centralNodes.size(); ++i)
    {
        std::cerr<<"Placing central node\n";
        int vOffset = 40;
        int hOffset = 70;
        const int rows = getClusterHeight(centralNodes.size());
        int startX = centralWidth;
        int startY = centralHeight-(rows/2*vOffset);
        int x = startX;
        int y = startY;

        y = startY + currentRow*vOffset;
        x = startX - i*hOffset;
        if(i % 2 == 0)
        {
            x = startX - currentRow*hOffset - hOffset;
            if(currentRow > rows / 2)
            {
                x+= (((currentRow - (rows / 2))) * 2*hOffset)-hOffset;
            }
        }
        else
        {
            x = startX + currentRow*hOffset + hOffset;
            if(currentRow > rows / 2)
            {
                x-= (((currentRow - (rows / 2))) * 2*hOffset)-hOffset;
            }
            ++currentRow;
        }
        centralNodes[i]->setVisual(new QGraphicsRectItem(x, y, hOffset-10, vOffset-10));
        centralNodes[i]->getVisual()->setBrush(QBrush(Qt::blue, Qt::SolidPattern));
        scene->addItem(centralNodes[i]->getVisual());
        centralNodes[i]->setX(x);
        centralNodes[i]->setY(y);

        for(uint j=0; j<centralNodes[i]->getPosConnections().size(); ++j)
        {
            int endX = centralNodes[i]->getPosConnections()[j]->getX();
            int endY = centralNodes[i]->getPosConnections()[j]->getY();
            QGraphicsLineItem* line = new QGraphicsLineItem(x, y, endX, endY);
            line->setPen(QPen(Qt::red));
            scene->addItem(line);
        }
    }
    w.getGraphicsView()->setScene(scene);
    scene->setBackgroundBrush(QBrush(Qt::black, Qt::SolidPattern));
    w.setMap(&categories_map);
    w.setCentral(&centralNodes);

    return a.exec();
}
