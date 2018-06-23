#ifndef CRITERIONFORM_H
#define CRITERIONFORM_H
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableView>
#include <QHeaderView>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QLabel>
#include <QScrollArea>
#include <QDebug>
#include <QPushButton>
#include <QEvent>
#include <QCloseEvent>
#include <QtMath>

class CriterionForm : public QDialog
{
    Q_OBJECT
public:
    explicit CriterionForm(QVector<QString> variants1, QString name1, QWidget *parent = 0);
    QVector<float> getVectors();
    bool isOpen();
private:
     QTableView* tableCreterion;
     QStandardItemModel  *model;
     QVector< QVector<float> > tableContent;
     QVector< float >ownVectors;//векторы
     float indeksS;//индекы согласованости
     QVector< float >matrixS;

     void calculateVectors();

     void buildTable();
     void buildForm(QVector <QString> variants1, QString name1);
     int colColumn;
     int colRow;
     bool valueChanged;
     bool isFull();
     QVBoxLayout* mainVLayout;
     QPushButton* agree;
     QPushButton* cancel;
     QLabel *ratio;
     QVector <QString> variants;
     QString name;
     QVector <QVector<int>> cells;
     QSize getSize();
     int result;//-1 или 0 в зависимости какая кнопка была нажата


     QString inversValue(QString value);
     float getNum(QString value);

     bool ansver;



     int state;//

private slots:
     void tabChanged(QStandardItem* item);
     void agreeReleased();
     void cancelReleased();
     void closeEvent(QCloseEvent *e);
};

#endif // CRITERIONFORM_H
