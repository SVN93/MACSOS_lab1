#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qdebug.h"
#include "utils.h"

#define N 300
#define K 10000

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

bool MainWindow::loadFile(const QString &fileName)
{    
    bool result = false;
    Q_ASSERT(!wavFile);
    Q_ASSERT(!fileName.isEmpty());
    wavFile = new WavFile(this);
    if (wavFile->open(fileName)) {
        if (isPCMS16LE(wavFile->fileFormat())) {
            result = true;
        }
    }

    if (result) {
        wavFile = new WavFile(this);
        wavFile->open(fileName);
    }
    return result;
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
        if (this->loadFile(filename)) {
            ui->firstMethod->setEnabled(true);
            ui->secondMethod->setEnabled(true);
            ui->thirdMethod->setEnabled(true);
            ui->fileSourceLabel->setText(this->wavFile->fileName());
        }
    } else {
        ui->firstMethod->setEnabled(false);
        ui->secondMethod->setEnabled(false);
        ui->thirdMethod->setEnabled(false);
    }
}

// Коэффициент корреляции
void MainWindow::writeToFile(QVector <qint16> result, QFile *file)
{
    file->write(reinterpret_cast<char *>(&this->wavFile->header), sizeof(CombinedHeader));
    file->write(reinterpret_cast<char *>(&result), result.length()*sizeof(qint16));
}

void MainWindow::on_firstMethod_clicked()
{
    // Коэффициент корреляции
    qint16 R = 0;
    qint64 tmp_r = 0;
    QVector <qint16> result;
    for (int j = 0; j < this->wavFile->body.size(); j += N) {
        for (int i = j; (i < j + N - 1) && (i < this->wavFile->body.size()); i++) {
            tmp_r += this->wavFile->body[i] * this->wavFile->body[i + 1];
        }
        qint64 tmp2_r = 0;
        for (int i = j; (i < j + N) && (i < this->wavFile->body.size()); i++) {
            tmp2_r += this->wavFile->body[i] * this->wavFile->body[i];
        }
        R = K * (1 + tmp_r) / tmp2_r;
        result.append(R);
    }

    if (this->wavFile) {
        QFile *file = fileFromFileDialog();
        writeToFile(result, file);
        file->close();
    }
}

//// Энергия
void MainWindow::on_secondMethod_clicked()
{
    // Энергия
    qint16 E = 0;
    qint64 tmp_e = 0;
    QVector <qint16> result;
    for (int j = 0; j < this->wavFile->body.size(); j += N) {
        for (int i = j; (i < j + N - 1) && (i < this->wavFile->body.size()); i++) {
            tmp_e += abs(this->wavFile->body[i]);
        }
        E = 3 * (qreal)tmp_e / N;
        result.append(E);
    }

    if (this->wavFile) {
        QFile *file = fileFromFileDialog();
        writeToFile(result, file);
        file->close();
    }
}

// Частота переходов через ноль
void MainWindow::on_thirdMethod_clicked()
{
    // Частота переходов через ноль
    qint16 Z = 0;
    qreal duration = N * this->wavFile->header.wave.blockAlign / this->wavFile->header.wave.byteRate;
    QVector <qint16> result;
    for (int j = 0; j < this->wavFile->body.size(); j += N) {
        qint16 counter = 0;
        for (int i = j; (i < j + N - 1) && (i < this->wavFile->body.size()); i++) {
            if (this->wavFile->body[i] * this->wavFile->body[i + 1] <= 0)
                counter++;
        }
        Z = K * counter / (2 * duration);
        result.append(Z);
    }

    if (this->wavFile) {
        QFile *file = fileFromFileDialog();
        writeToFile(result, file);
        file->close();
    }
}

// Результирующий метод
void MainWindow::on_resultMethod_clicked()
{

}
