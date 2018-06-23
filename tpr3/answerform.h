#ifndef ANSWERFORM_H
#define ANSWERFORM_H
#include <QDialog>
#include <QMap>
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
#include <QPlainTextEdit>

class AnswerForm : public QDialog
{
    Q_OBJECT
public:
    explicit AnswerForm(QMap<QString, float>report, QWidget *parent = 0);
    bool isOpen();
private:
    void build(QMap<QString, float> report);
    int result;//-1 или 0 в зависимости какая кнопка была нажата


    QPushButton* agree;
    QPushButton* cancel;
private slots:
     void closeEvent(QCloseEvent *e);
     void agreeReleased();
     void cancelReleased();
};

#endif // ANSWERFORM_H
