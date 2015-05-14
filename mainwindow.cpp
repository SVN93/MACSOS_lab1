#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qdebug.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::nameOfFileFromFileDialog()
{
    QString filename = QFileDialog::getOpenFileName(
            this,
            tr("Open wav file"),
            QDir::currentPath(),
            tr("Wav file (*.wav);;All files (*.*)") );

    return filename;
}

QFile * MainWindow::fileFromFileDialog()
{
    QString filename = QFileDialog::getSaveFileName(
            this,
            tr("Save wav file"),
            QDir::currentPath(),
            tr("Wav file (*.wav);;All files (*.*)"));
    QFile *file = new QFile(filename);

    return file;
}

void MainWindow::on_choseFileButton_clicked()
{
    QString filename = nameOfFileFromFileDialog();

    if (!filename.isEmpty()) {
        ui->firstMethod->setEnabled(true);
        ui->secondMethod->setEnabled(true);
        ui->thirdMethod->setEnabled(true);

        ui->fileSourceLabel->setText(filename);
        QFile *file = new QFile(filename);
        WavFile wavFile(file);
        MainWindow::wavFile = &wavFile;
    } else {
        ui->firstMethod->setEnabled(false);
        ui->secondMethod->setEnabled(false);
        ui->thirdMethod->setEnabled(false);
    }
}

// Коэффициент корреляции
void MainWindow::on_firstMethod_clicked()
{
    if (MainWindow::wavFile) {
        QFile *file = fileFromFileDialog();
    }
}

// Энергия
void MainWindow::on_secondMethod_clicked()
{

}

// Частота переходов через ноль
void MainWindow::on_thirdMethod_clicked()
{

}

// Результирующий метод
void MainWindow::on_resultMethod_clicked()
{

}
