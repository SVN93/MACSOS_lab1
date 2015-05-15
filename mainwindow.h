#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "wavfile.h"
#include <QFileDialog>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    WavFile *wavFile;

    QString openFile();
    QString nameOfFileFromFileDialog();
    QFile * fileFromFileDialog();
    void writeToFile(QVector <qint16> result, QFile *file);
private slots:
    void on_choseFileButton_clicked();

    void on_firstMethod_clicked();

    void on_secondMethod_clicked();

    void on_thirdMethod_clicked();

    void on_resultMethod_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
