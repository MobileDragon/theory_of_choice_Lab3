#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWindow>
#include <QMainWindow>
#include "criterionform.h"
#include <QDebug>
#include <QLineEdit>
#include "mylineedit.h"
#include "answerform.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QVector< CriterionForm* > wMarksObject;//формочки с вариантами
    CriterionForm *wArgument;//формочка с оценками для аргументов--

private slots:
    void on_pushButton_released();
    void criterionDestroy(int result);
    void argumentDestroy(int result);
    void answerDestroy(int result);

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_comboBox_2_currentIndexChanged(const QString &arg1);
    void lineChanged1(int id, QString s);
    void lineChanged(int id, QString s);

private:
    Ui::MainWindow *ui;
    void build();
    bool isCorrect();//корректность заполнения формы
    int kolTables;//число критериев
    int kolVariants;//число вариантов

    QVector <QString> alternativ;
    QVector <QString> kriterion;
    QVector< MyLineEdit* > linesKriterion;
    QVector< MyLineEdit* > linesAlternativ;
};

#endif // MAINWINDOW_H
