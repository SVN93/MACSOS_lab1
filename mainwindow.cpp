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

void MainWindow::on_makeNoiseVersion_clicked()
{
    this->signalWithNoise.clear();
    qint16 max = 0;
    foreach (qint16 el, this->wavFile->body) {
        if (el > max) {
            max = el;
        }
    }
    for (int i = 0; i < this->wavFile->body.size(); i++) {
        this->signalWithNoise.append( this->wavFile->body[i] + 0.1 * max);
    }

    ui->useNoiseVersion->setEnabled(true);
    if (this->wavFile && (this->signalWithNoise.length())) {
        QFile *file = fileFromFileDialog();
        writeToFile(this->signalWithNoise, file);
    }
}

void MainWindow::on_choseFileButton_clicked()
{
    QString filename = nameOfFileFromFileDialog();

    if (!filename.isEmpty()) {
        if (this->loadFile(filename)) {
            ui->firstMethod->setEnabled(true);
            ui->secondMethod->setEnabled(true);
            ui->thirdMethod->setEnabled(true);
            ui->resultMethod->setEnabled(true);
            ui->fileSourceLabel->setText(this->wavFile->fileName());
            ui->makeNoiseVersion->setEnabled(true);
        }
    } else {
        ui->firstMethod->setEnabled(false);
        ui->secondMethod->setEnabled(false);
        ui->thirdMethod->setEnabled(false);
        ui->resultMethod->setEnabled(false);
        ui->fileSourceLabel->clear();
        ui->makeNoiseVersion->setEnabled(false);
    }
}

void MainWindow::writeToFile(QVector <qint16> result, QFile *file)
{
    if (!file->fileName().isEmpty()) {
        file->open(QIODevice::WriteOnly);
        file->write(reinterpret_cast<char *>(&this->wavFile->header), sizeof(CombinedHeader));
        file->write(reinterpret_cast<char *>(&result), result.length()*sizeof(qint16));
        file->close();
    }
}

// Коэффициент корреляции
void MainWindow::koreletionMethod()
{
    qint16 R = 0;
    this->kBody.clear();
    QVector <qint16> currentVector;
    if (ui->useNoiseVersion->isChecked()) {
        currentVector = this->signalWithNoise;
    } else {
        currentVector = this->wavFile->body;
    }

    for (int j = 0; j < currentVector.size(); j += N) {
        qint64 tmp_r = 0;
        for (int i = j; (i < j + N - 1) && (i < currentVector.size() - 1); i++) {
            tmp_r += ( currentVector[i] * currentVector[i + 1] );
        }

        qint64 tmp2_r = 0;
        for (int i = j; (i < j + N) && (i < currentVector.size()); i++) {
            tmp2_r += ( currentVector[i] * currentVector[i] );
        }

        if (tmp2_r == 0) {
            R = 0;
        } else {
            R = K * (1 + tmp_r) / tmp2_r;
        }
        this->kBody.append(R);
    }
}

// Энергия
void MainWindow::energiColculation()
{
    qint16 E = 0;
    this->eBody.clear();
    for (int j = 0; j < this->wavFile->body.size(); j += N) {
        qint64 tmp_e = 0;
        for (int i = j; (i < j + N) && (i < this->wavFile->body.size()); i++) {
            tmp_e += abs(this->wavFile->body[i]);
        }
        E = 3 * (qreal)tmp_e / N;
        this->eBody.append(E);
    }
}

// Частота переходов через ноль
void MainWindow::ZeroMethod()
{
    qint16 Z = 0;
    qreal duration = N * this->wavFile->header.wave.blockAlign / this->wavFile->header.wave.byteRate;
    this->zBody.clear();
    QVector <qint16> currentVector;
    if (ui->useNoiseVersion->isChecked()) {
        currentVector = this->signalWithNoise;
    } else {
        currentVector = this->wavFile->body;
    }

    for (int j = 0; j < currentVector.size(); j += N) {
        qint16 counter = 0;
        for (int i = j; (i < j + N - 1) && (i < currentVector.size() - 1); i++) {
            if (currentVector[i] * currentVector[i + 1] <= 0)
                counter++;
        }
        Z = K * counter / (2 * duration);
        this->zBody.append(Z);
    }
}

void MainWindow::on_firstMethod_clicked()
{
    // Коэффициент корреляции
    koreletionMethod();

    if (this->wavFile) {
        QFile *file = fileFromFileDialog();
        writeToFile(this->kBody, file);
    }
}

void MainWindow::on_secondMethod_clicked()
{
    // Энергия
    energiColculation();

    if (this->wavFile) {
        QFile *file = fileFromFileDialog();
        writeToFile(this->eBody, file);
    }
}

void MainWindow::on_thirdMethod_clicked()
{
    // Частота переходов через ноль
    ZeroMethod();

    if (this->wavFile) {
        QFile *file = fileFromFileDialog();
        writeToFile(this->zBody, file);
    }
}

// Результирующий метод
void MainWindow::on_resultMethod_clicked()
{
    // Выполняем предыдущие 3
    this->koreletionMethod();
    this->energiColculation();
    this->ZeroMethod();

    for (int i = 0; i < this->eBody.length(); i++) {
        if (this->zBody[i] == 0) {
            qint16 result = (this->kBody[i] * this->eBody[i]);
            this->resultBody.append(result);
        }
        else {
            qint16 result = (this->kBody[i] * this->eBody[i]);
            this->resultBody.append(result);
        }
    }

    if (this->wavFile) {
        QFile *file = fileFromFileDialog();
        writeToFile(this->resultBody, file);
    }
}
