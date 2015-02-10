#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>
#include <iostream>
#include <QGraphicsItem>

#ifndef POS
#define POS "pos"
#endif //pos
#ifndef NEG
#define NEG "neg"
#endif //neg

class Node
{
    protected:
        std::vector<Node*> posConnections;
        std::vector<Node*> negConnections;
        std::string cluster;
        int tolerance;
        int x;
        int y;
    private:
        QGraphicsRectItem* visRep;

    public:
        std::vector<Node*> getPosConnections(){return posConnections;}
        void setCluster(std::string c){cluster = c;}
        std::string getCluster(){return cluster;}
        QGraphicsRectItem* getVisual(){return visRep;}
        void setTolerance(int i){tolerance = i;}
        int getTolerance(){return tolerance;}
        int getX(){return x;}
        void setX(int ex){x=ex;}
        int getY(){return y;}
        void setY(int why){y=why;}
        void setVisual(QGraphicsRectItem* qgri){visRep = qgri;}
        Node(){tolerance=0; x=0; y=0;}
        ~Node(){delete visRep;}
        void addConnection(Node* n, std::string type)
        {
            if(type == POS)
            {
                posConnections.push_back(n);
                return;
            }
            else if(type == NEG)
            {
                negConnections.push_back(n);
            }
            else
            {
                std::cerr<< "CONNECTION NEVER ADDED!  DID NOT SPECIFY TYPE (pos/neg)!";
            }
        }
};

#endif // NODE_H
