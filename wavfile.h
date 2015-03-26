#ifndef WAVFILE_H
#define WAVFILE_H

#include <QObject>

class WavFile : public QObject
{
    Q_OBJECT
public:
    explicit WavFile(QObject *parent = 0);
    ~WavFile();

signals:

public slots:
};

#endif // WAVFILE_H
