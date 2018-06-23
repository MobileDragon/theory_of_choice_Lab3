#include "criterionform.h"

CriterionForm::CriterionForm(QVector <QString> variants1, QString name1, QWidget *parent):
    QDialog(parent)
{
    buildForm(variants1, name1);
    variants = variants1;
    name=name1;
    name="ghjgj";
    //this->setSizePolicy(QSizePolicy::MinimumExpanding);
}

QVector<float> CriterionForm::getVectors()
{
    return ownVectors;
}

bool CriterionForm::isOpen()
{
    if(result==-1)
        return true;
    return false;
}

void CriterionForm::buildForm(QVector <QString> variants1, QString name1)
{
    result=-1;
    valueChanged=false;
    ansver=false;
    variants = variants1;
    name=name1;
    matrixS={0, 0, 0.58, 0.9, 1.12, 1.24, 1.32, 1.41, 1.45, 1.49};

    mainVLayout = new QVBoxLayout();
    QLabel *sdf = new QLabel( name );
    mainVLayout->addWidget(sdf );
    buildTable();
    mainVLayout->addWidget(tableCreterion);

    //

    cancel = new QPushButton("Изменить условие");
    agree = new QPushButton("Принять");


    QHBoxLayout *HBox = new QHBoxLayout();
    HBox->addWidget(cancel);
    HBox->addWidget(agree);
    agree->autoDefault();
    agree->setEnabled(false);
    agree->setDefault(true);
    HBox->setAlignment(cancel,Qt::AlignLeft);
    HBox->setAlignment(agree,Qt::AlignRight);
    connect(agree, SIGNAL(released()) ,this,SLOT(agreeReleased() ));
    connect(cancel, SIGNAL(released()) ,this,SLOT(cancelReleased() ));

    //отношение согласованости=
    ratio = new QLabel( "Отношение согласованости = " );
    mainVLayout->addWidget(ratio);

    mainVLayout->addLayout(HBox);
    //
    this->setLayout(mainVLayout);

}

void CriterionForm::calculateVectors()
{
    //ownVectors
    ownVectors.clear();
    //tableContent
    QVector< QVector<float> > tableNormal;
    int kol = colColumn-1;
    //сумма столбцов
    QVector<float> sumColumn;
    for(int i=0; i < kol; i++)
    {
        float summ=0;
        for(int j=0; j < kol; j++)
        {
            summ += tableContent[j][i];
        }
        sumColumn.append(summ);
    }

    //нормализация
    QVector<float> normalColumn;
    for(int i=0; i < kol; i++)
    {
        for(int j=0; j < kol; j++)
        {
            normalColumn.append(tableContent[j][i]/sumColumn[i]);
            //normalColumn.append();
        }
        tableNormal.append(normalColumn);//строки это столбцы
        normalColumn.clear();
    }
    //среднее

    for(int i=0; i < kol; i++)
    {
        float midle=0;
        for(int j=0; j < kol; j++)
        {
            midle+=tableNormal[j][i];
        }
        ownVectors.append(midle/kol);
    }
    //идекс соглвсованости
    float midleMumnoj=0;
    for(int i=0; i < kol; i++)
    {
        float mumnoj=0;

        for(int j=0; j < kol; j++)
        {
            mumnoj+=tableContent[i][j]*ownVectors[j];
        }
        midleMumnoj+=mumnoj/ownVectors[i];
    }
    qDebug() << kol;
    qDebug() << midleMumnoj;
    midleMumnoj = midleMumnoj/kol;
    if(midleMumnoj-kol>0.0001)
        indeksS=((midleMumnoj-kol)/(kol-1))/matrixS[kol-1]*100;
    else
        indeksS=0;


    ratio->setText( QString("Отношение согласованости = %1\%").arg(indeksS));
}

void CriterionForm::buildTable()
{

    colColumn=variants.size()+1;
    colRow=colColumn;

    tableCreterion=new QTableView();
    tableCreterion->setFont(QFont("Arial",10,5000));
    tableCreterion->verticalHeader()->hide();

    tableCreterion->horizontalHeader()->hide();
    //
    //tableCreterion->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    //tableCreterion->setAcceptDrops(true);

    model = new QStandardItemModel;
    QStandardItem *item;
    //��������������

    item = new QStandardItem("");
    item->setEditable(false);
    item->setCheckable(false);
    item->setEnabled(false);

    item->setBackground(QBrush(QColor(250,230,170)) );
    item->setFont(QFont("Arial",10,6500));
    model->setItem(0, 0, item);//������ ������
    //
    int iterator=1;


    for (int i=1; i<colRow; i++)
    {
        for(int j=1;j<colColumn; j++)
        {
            item = new QStandardItem("");
            item->setFont(QFont("Arial",10,6500));
            model->setItem(i, j, item);
        }
        //

    }
    //variants.append("32");
    foreach (QString var, variants)
    {
        item = new QStandardItem(var);
        item->setEditable(false);
        item->setSelectable(false);
        item->setBackground(QBrush(QColor(250,230,170)) );
        item->setFont(QFont("Arial",10,6500));
        //
        model->setItem(0, iterator, item);
        QStandardItem *item2 = new QStandardItem(var);
        item2->setEditable(false);
        item2->setSelectable(false);
        item2->setBackground(QBrush(QColor(250,230,170)) );
        item2->setFont(QFont("Arial",10,6500));
        //
        model->setItem(iterator, 0, item2);
        ++iterator;
    }
    for (int i=1; i<colColumn; i++)
    {
        item = new QStandardItem("1");
        item->setEditable(false);
        item->setSelectable(false);
        item->setBackground(QBrush(QColor(0,200,55) ) );
        item->setFont(QFont("Arial",10,6500));
        //
        model->setItem(i, i, item);
    }

    connect(model, SIGNAL(itemChanged(QStandardItem*)) ,this,SLOT(tabChanged(QStandardItem*) ));//���������
    tableCreterion->setModel(model);

    //tableCreterion->viewport()->setma
    int left;
    int right;
    int top;
    int bottom;
    tableCreterion->viewport()->getContentsMargins(&left, &top, &right, &bottom);
    tableCreterion->viewport()->setContentsMargins(5,5,5,5);


    for(int i=0; i < colColumn; i++)//������ ��������
    {
        tableCreterion->horizontalHeader()->setSectionResizeMode(i,QHeaderView::ResizeToContents);
    }

    //tableCreterion->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);

    tableCreterion->setFixedSize(getSize());
    //this->sizePolicy().MinimumExpanding;
}

QSize CriterionForm::getSize()
{
    QSize tableSize;
    int i=0;
    int height=0;
    int currentHeight=tableCreterion->rowHeight(i);
    while(currentHeight > 0)
    {
        height+=currentHeight;
        ++i;
        currentHeight=tableCreterion->rowHeight(i);
    }
    height+=i/2+1;
    i=0;
    int width=0;
    int currentWidth=tableCreterion->columnWidth(i);
    while(currentWidth > 0)
    {
        width+=currentWidth;
        ++i;
        currentWidth=tableCreterion->columnWidth(i);
    }
    width+=i/2+1;
    //width = tableCreterion->columnWidth(0)+20;
    tableSize.setHeight(height);
    tableSize.setWidth(width);
    return tableSize;
}

void CriterionForm::tabChanged(QStandardItem* item)
{


    item->text();
    item->column();

    if(!valueChanged)
    {
        valueChanged=true;
        //item->setText(inversValue(item->text()) );
        QString s = inversValue(item->text());
        if(s.length()==0)
        {
            item->setText("");
            valueChanged=false;
            return;
        }
        QStandardItem *itemT = new QStandardItem(s);

        item->model()->setItem(item->column(), item->row(), itemT);
        valueChanged=false;
        //
        if(isFull())
        {
            agree->setEnabled(true);
            calculateVectors();
        }
        else
        {
            agree->setEnabled(false);
            ratio->setText("Отношение согласованости = ");
        }
    }
    //inversValue(item->text());
    //item->setText("43");

}

void CriterionForm::agreeReleased()
{
    if(isFull())
        result=0;
    else
        result=-1;
    this->close();
}

void CriterionForm::cancelReleased()
{
    result=-1;
    this->close();

}

QString CriterionForm::inversValue(QString value)
{
    QString s="";
    if(value=="0")
        return s;


    QRegExp rx("\\d*\\/\\d+");
    bool pos = rx.exactMatch( value );
    if (pos)
    {
        qDebug() << "rx.capturedTexts()[0]";
        QStringList list1 = value.split("/");
        if(list1[0]=="1")
            s=list1[1];
        else
            s = QString("%1/%2").arg(QString(list1[1])).arg(QString(list1[0]));
    }
    else
    {
        QRegExp rx1("\\d+");
        pos = rx1.exactMatch( value );
        if(pos)
        {
            if(value=="1")
                s=value;
            else
                s = QString("1/%1").arg(value);

        }
        else
        {
            QRegExp rx2("0.\\d+");
            pos = rx2.exactMatch( value );
            if(pos)
            {
                qDebug() << "123yyyyyyyyyyyyyyyyyyyy";
                float f = value.toFloat();
                f = 1-f;
                s = QString::number(f);
            }
        }
    }


    //QString("Z%1").arg(row-1)
    return s;
}

float CriterionForm::getNum(QString value)
{
    float f=-1;
    if(value.length()==0)
        return f;
    QRegExp rx("\\d*\\/\\d+");
    int numerator;
    int denominator;
    bool pos = rx.exactMatch( value );
    if (pos)
    {
        qDebug() << "rx.capturedTexts()[0]";
        QStringList list1 = value.split("/");
        numerator=QString(list1[0]).toInt();
        denominator=QString(list1[1]).toInt();
        f = numerator / ((float)denominator);
    }
    else
    {
        QRegExp rx1("\\d+");
        pos = rx1.exactMatch( value );
        if(pos)
        {
            qDebug() << "yyyyyyyyyyyyyyyyyyyy";
            f = value.toInt();
        }
        else
        {
            QRegExp rx2("0.\\d+");
            pos = rx2.exactMatch( value );
            if(pos)
            {
                qDebug() << "123yyyyyyyyyyyyyyyyyyyy";
                f = value.toFloat();
            }
        }
    }
    return f;
}

void CriterionForm::closeEvent(QCloseEvent *e)
{
    this->finished(result);
    e->accept();
}

bool CriterionForm::isFull()
{
    tableContent.clear();
    for(int i=1; i<colRow; ++i)
    {
        QVector<float> rowValues;
        qDebug() << "vhod*********";
        for(int j=1; j<colColumn; j++)
        {
            qDebug() << "tut.........";
            float val = getNum(model->item(i,j)->text() );
            if(val < 0)
            {
                tableContent.clear();
                return false;
            }
            rowValues.append( val );
            qDebug() << "ZDES.........";
        }
        qDebug() << "+++++++++++++++";
        tableContent.append(rowValues);
    }
    return true;
}
