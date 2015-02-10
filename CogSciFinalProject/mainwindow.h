#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QApplication>
#include <QtWidgets>
#include <map>
#include <string>
#include "Node.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QWidget* centralWidget;
    QGraphicsView* graphicsView;
    QPushButton* exitButton;
    QPushButton* queryButton;
    QLineEdit* queryText;
    QLabel* outputLabel;

    std::map<std::string, std::map<std::string, Node*> >* categories_map;
    std::vector<Node*>* centralNodes;

    int tolerance;


public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QGraphicsView* getGraphicsView(){return graphicsView;}
    void setMap(std::map<std::string, std::map<std::string, Node*> >* map){categories_map = map;}
    void setCentral(std::vector<Node*>* vec){centralNodes = vec;}
    void setTolerance(int i){tolerance = i;}
    int getTolerance(){return tolerance;}
    void query(std::string text);

private slots:
    void on_queryButton_clicked();

    void on_exitButton_clicked();
};

#endif // MAINWINDOW_H
