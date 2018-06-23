#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    build();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::build()
{
    ui->pushButton->setEnabled(false);
    //ui->centralWidget->com
    for(int i=2; i<11; ++i)
    {
        ui->comboBox->addItem(QString().setNum(i,10) );
        ui->comboBox_2->addItem(QString().setNum(i,10) );
    }
}

bool MainWindow::isCorrect()
{
    foreach (QString s, alternativ)
    {
        if(s.length()==0)
            return false;
    }
    foreach (QString s, kriterion)
    {
        if(s.length()==0)
            return false;
    }
    return true;
    //проверка на повторение
}

void MainWindow::on_pushButton_released()
{
    //int colCriterion=ui->comboBox_2->itemText(ui->comboBox_2->currentIndex()).toInt();
    wArgument = new CriterionForm(kriterion, "Установите приоритет критериев");
    connect(wArgument, SIGNAL(finished(int)) ,this,SLOT(argumentDestroy(int) ));
    wArgument->show();

    this->hide();

}

void MainWindow::criterionDestroy(int result)
{
    qDebug() << "[+] Connected to Database File ";
    qDebug() << result;
    if(result<0)
    {
        foreach (CriterionForm *form, wMarksObject)
        {
            form->close();
            form->deleteLater();
        }
        this->show();
    }
    else
    {
        qDebug() << "[{{{{{{{{{{{{{[[[";
        for(int i=0; i<wMarksObject.size(); i++)//все ли окна закрыты
        {
            if(wMarksObject[i]->isOpen())
                return;
        }

        qDebug() << "[{{{{{{{{{{{{{[[[[[[[[[[[[[[[[[[[]]]]]]]]]]]]]]]]]]] ";
        QMap<QString, float>weightVariant;
        //отображение формы с результатами подсчетов
        //подсчеты
        QVector< float >vectorsCriterion = wArgument->getVectors();
        for(int i=0; i<wMarksObject.size(); i++)//по числу критериев, перебор форм с альтернативами
        {
            QVector< float >vectorsVariant = wMarksObject[i]->getVectors();

            for(int j=0; j<kolVariants; j++)//по числу вариантов
            {
                float curWeight = weightVariant[alternativ[j]] + vectorsVariant[j]*vectorsCriterion[i];
                weightVariant.insert(alternativ[j], curWeight);
            }



        }
        AnswerForm *answer = new AnswerForm(weightVariant);
        connect(answer, SIGNAL(finished(int)) ,this,SLOT(answerDestroy(int) ));
        answer->show();
        foreach (QString key, weightVariant.keys())
        {
            qDebug() << key <<weightVariant[key] ;
        }


        //

    }
    wMarksObject.clear();
    wArgument->deleteLater();
    //this->show();

}

void MainWindow::argumentDestroy(int result)
{
    wArgument->hide();
    if(result<0)
        this->show();
    else
    {
        //ПОСЛЕ
        kolTables=ui->comboBox->itemText(ui->comboBox->currentIndex()).toInt();
        kolVariants=ui->comboBox_2->itemText(ui->comboBox_2->currentIndex()).toInt();
        for(int i=0; i<kolTables; ++i)
        {
            CriterionForm *form = new CriterionForm(alternativ,QString("Укажите оценку вариантов по критерию %1").arg(kriterion[i]) );
            connect(form, SIGNAL(finished(int)) ,this,SLOT(criterionDestroy(int) ));
            wMarksObject.append(form);//массив вариантов, название критерия
            wMarksObject[i]->show();

        }
    }


}

void MainWindow::answerDestroy(int result)
{
    if(result<0)
        this->close();
    else
        this->show();

}

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    ui->pushButton->setEnabled(false);
    int kol=arg1.toInt();
    foreach (MyLineEdit *line, linesKriterion)
    {
        line->deleteLater();
    }
    linesKriterion.clear();
    kriterion.clear();
    for(int i=0; i<kol; i++)
    {
        MyLineEdit *line = new MyLineEdit(i);
        linesKriterion.append(line);

        connect(line, SIGNAL(MySignal(int,QString)) ,this,SLOT(lineChanged1(int, QString) ));
        ui->verticalLayout_2->addWidget(linesKriterion[i]);
        kriterion.append("");
    }
    this->adjustSize();
}

void MainWindow::on_comboBox_2_currentIndexChanged(const QString &arg1)
{
    ui->pushButton->setEnabled(false);
    int kol=arg1.toInt();
    foreach (MyLineEdit *line, linesAlternativ)
    {
        line->deleteLater();
    }
    linesAlternativ.clear();
    alternativ.clear();
    for(int i=0; i<kol; i++)
    {
        MyLineEdit *line = new MyLineEdit(i);
        //line->setUserData();
        linesAlternativ.append(line);
        ui->verticalLayout_3->addWidget(linesAlternativ[i]);
        connect(linesAlternativ[i], SIGNAL(MySignal(int,QString)) ,this,SLOT(lineChanged(int, QString) ));
        alternativ.append("");
    }
    this->adjustSize();
}

void MainWindow::lineChanged1(int id, QString s)
{
    kriterion[id]=s;
    if(isCorrect())
        ui->pushButton->setEnabled(true);
    else
        ui->pushButton->setEnabled(false);
}

void MainWindow::lineChanged(int id, QString s)
{
    alternativ[id]=s;
    if(isCorrect())
        ui->pushButton->setEnabled(true);
    else
        ui->pushButton->setEnabled(false);
}
