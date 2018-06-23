#include "answerform.h"

AnswerForm::AnswerForm(QMap<QString, float> report, QWidget *parent):
    QDialog(parent)
{
    build(report);
}

bool AnswerForm::isOpen()
{
    if(result==-1)
        return true;
    return false;
}

void AnswerForm::build(QMap<QString, float> report)
{
    result=-1;
    cancel = new QPushButton("Закрыть программу");
    agree = new QPushButton("Продолжить");
    QHBoxLayout *HBox = new QHBoxLayout();
    HBox->addWidget(cancel);
    HBox->addWidget(agree);
    connect(agree, SIGNAL(released()) ,this,SLOT(agreeReleased() ));
    connect(cancel, SIGNAL(released()) ,this,SLOT(cancelReleased() ));
    //
    QVBoxLayout *mainVLayout = new QVBoxLayout();
    //
    QString answer="";
    QString maxName="";
    float maxValue=0;
    foreach (QString key, report.keys())
    {
        if(report[key] > maxValue)
        {
            maxValue = report[key];
            maxName = key;
        }
        answer.append(  QString("%1: %2\n").arg(key).arg(report[key]) );
    }
    answer.append( QString("Таким образом, найболее предпочитаемым вариантом яаляется %1: с значением %2\n").arg(maxName).arg(maxValue) );

    QPlainTextEdit *plaintText = new QPlainTextEdit(answer);
    //QLabel *sdf = new QLabel( name );
    mainVLayout->addWidget(plaintText );
    mainVLayout->addLayout(HBox);
    this->setLayout(mainVLayout);
}

void AnswerForm::closeEvent(QCloseEvent *e)
{
    this->finished(result);
    e->accept();
}

void AnswerForm::agreeReleased()
{
    result=0;
    this->close();
}

void AnswerForm::cancelReleased()
{
    result=-1;
    this->close();
}
