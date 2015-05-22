#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "wavfile.h"
#include <QFileDialog>
#include <QVector>

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

    /**
     * Load data from WAV file
     */
    bool loadFile(const QString &fileName);

    QString openFile();
    QString nameOfFileFromFileDialog();
    QFile * fileFromFileDialog();
    void writeToFile(QVector <qint16> result, QFile *file);
    void koreletionMethod();
    void energiColculation();
    void ZeroMethod();
private slots:
    void on_choseFileButton_clicked();

    void on_firstMethod_clicked();

    void on_secondMethod_clicked();

    void on_thirdMethod_clicked();

    void on_resultMethod_clicked();

    void on_makeNoiseVersion_clicked();

private:
    Ui::MainWindow *ui;
    QVector <qint16> signalWithNoise;
    QVector <qint16> kBody;
    QVector <qint16> eBody;
    QVector <qint16> zBody;
    QVector <qint16> resultBody;
};

#endif // MAINWINDOW_H
