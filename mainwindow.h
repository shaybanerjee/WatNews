#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "filedownloader.h"
#include <QMainWindow>
#include "newwindow.h"



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void loadImage();
    void on_pushButton_pressed();
public slots:
    void fetchcontent();
    void loadCont();

private:
    Ui::MainWindow *ui;
    FileDownloader *m_pImgCtrl;
    FileDownloader *file;
    FileDownloader *new_img;
    QStringList *propertyTitles = new QStringList();
    QStringList *propertySites =  new QStringList();
    QList <int> propertyIds;

};

#endif // MAINWINDOW_H
