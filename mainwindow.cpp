#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QTextEdit>
#include <QProcess>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <iostream>
#include <QBoxLayout>
#include <QScrollArea>
#include "dialog.h"
#include <QFont>


using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPixmap pix("/Users/shayonbanerjee/Documents/news.png");
    ui->label_4->setPixmap(pix);
    this->setStyleSheet("background-color: black;");
    ui->pushButton->setStyleSheet("background-color: rgb(255, 193, 0);");
    ui->pushButton_3->setStyleSheet("background-color: rgb(255, 193, 0);");


}



MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_pressed()
{
    QUrl imageUrl("https://api.uwaterloo.ca/v2/news.json?key=d3bf105559055ca3a88699e6183fa583");
    m_pImgCtrl = new FileDownloader(imageUrl, this);

    connect(m_pImgCtrl, SIGNAL (downloaded()), this, SLOT (loadImage()));
    ui->pushButton->setVisible(false);
}


void MainWindow::loadImage() {

    QByteArray dat = m_pImgCtrl->downloadedData();
    QJsonDocument doc(QJsonDocument::fromJson(dat));
    QJsonObject json_data = doc.object();
    QJsonArray content = json_data["data"].toArray();
    foreach (const QJsonValue &value, content) {
        QJsonObject obj = value.toObject();
        propertyTitles->append(obj["title"].toString());
        propertySites->append(obj["site"].toString());
        propertyIds.append(obj["id"].toInt());
    }
    QVBoxLayout* layout = new QVBoxLayout();
    QPushButton *b = new QPushButton();
    QLabel *curr_title = new QLabel;
    QWidget *line = new QWidget;
    line->setFixedHeight(4);
    QWidget *space = new QWidget;
    space->setStyleSheet(QString("background-color: black;"));
    space->setFixedHeight(9);
    line->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    line->setStyleSheet(QString("background-color: white;"));
    curr_title->setText("LATEST HEADLINES:" );
    curr_title->setStyleSheet("QLabel { color : rgb(255, 193, 0);}");
    b->setStyleSheet("background-color: rgb(255, 193, 0); margin-left: 120px;");
    b->setFixedWidth(230);


    b->setText("Refresh!");

    QFont font;
    font.setPointSize(20);
    font.setBold(true);
    curr_title->setFont(font);
    curr_title->setAlignment(Qt::AlignCenter);
    layout->addWidget(curr_title);
    layout->addWidget(space);
    layout->addWidget(b);
    layout->addWidget(line);

    connect(b, SIGNAL (clicked()), this, SLOT (loadImage()));
    for (int i = 0; i < propertyTitles->size(); ++i) {

        QLabel *newlabel = new QLabel(this);
        newlabel->setText(propertyTitles->at(i).toLocal8Bit().constData());
        newlabel->setGeometry(QRect(2, (i+ (100*i)), 600, 1000));
        newlabel->setWordWrap(true);
        newlabel->setStyleSheet("QLabel { color : white; }");
        QPushButton *butt = new QPushButton();
        butt->setText("Read Article");
        butt->setStyleSheet("background-color: rgb(255, 193, 0);");

        QWidget *horizontalLineWidget = new QWidget;
        horizontalLineWidget->setFixedHeight(2);
        horizontalLineWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        horizontalLineWidget->setStyleSheet(QString("background-color: #c0c0c0;"));
        layout->addWidget(space);
        layout->addWidget(newlabel);
        layout->addWidget(space);
        layout->addWidget(butt);
        layout->addWidget(horizontalLineWidget);
        QString str = QString::number(i);
        butt->setObjectName(str);
        connect(butt, SIGNAL(clicked()), this, SLOT(fetchcontent()));

}
    QWidget* contentWidget = new QWidget();
    contentWidget->setLayout(layout);

    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidget(contentWidget);

    setCentralWidget(scrollArea);

}


void MainWindow::fetchcontent() {
    QObject *senderObj = sender();
    QString senderObjName = senderObj->objectName();
    int val = senderObjName.toInt();
    QUrl site("https://api.uwaterloo.ca/v2/news/" + propertySites->at(val) + "/" + QString::number(propertyIds.at(val)) + ".json?key=d3bf105559055ca3a88699e6183fa583");
    file = new FileDownloader(site, this);
    connect(file, SIGNAL (downloaded()), this, SLOT (loadCont()));
}


void MainWindow::loadCont() {
    QByteArray dat = file->downloadedData();
    QJsonDocument doc(QJsonDocument::fromJson(dat));
    QJsonObject json_data = doc.object();
    QVariantMap json_map = json_data.toVariantMap();
    QVariantMap json_items = json_map["data"].toMap();
    QVariantMap Img_Json = json_map["image"].toMap();
    QString title = json_items["title"].toString();
    QString description = json_items["description"].toString();
    QString url = json_items["link"].toString();
    QTextEdit *text = new QTextEdit();
    text->setText(description);
    text->setReadOnly(true);
    text->setMinimumHeight(200);
    QPalette pal = text->palette();
    pal.setColor(QPalette::Base, pal.color(QPalette::Window));
    text->setPalette(pal);
    QWidget *horizontalLineWidget = new QWidget;
    horizontalLineWidget->setFixedHeight(2);
    horizontalLineWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    horizontalLineWidget->setStyleSheet(QString("background-color: rgb(255, 193, 0);"));
    text->setStyleSheet("QTextEdit { color : white; margin-top: 7px; }");
    QPushButton *new_butt = new QPushButton();
    new_butt->setStyleSheet("background-color: rgb(255, 193, 0);");
    Dialog *nd = new Dialog(this);
    QVBoxLayout *box = new QVBoxLayout(nd);
    QLabel *n = new QLabel();
    n->setStyleSheet("QLabel { color : white; }");
    n->setMinimumHeight(80);
    new_butt->setText("Back");
    connect(new_butt, SIGNAL(clicked()), nd, SLOT(close()));
    n->setText(title);
    n->setWordWrap(true);
    box->addWidget(n);
    box->addWidget(horizontalLineWidget);
    box->addWidget(text);
    box->addWidget(new_butt);
    nd->show();
}


