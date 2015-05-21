#ifndef WAVFILE_H
#define WAVFILE_H

#include <QObject>
#include <QFile>
#include <QAudioFormat>
#include <QFileDialog>
#include <QVector>

struct chunk
{
    char        id[4];
    quint32     size;
};

struct RIFFHeader
{
    chunk       descriptor;     // "RIFF"
    char        type[4];        // "WAVE"
};

struct WAVEHeader
{
    chunk       descriptor;
    quint16     audioFormat;
    quint16     numChannels;
    quint32     sampleRate;
    quint32     byteRate;
    quint16     blockAlign;
    quint16     bitsPerSample;
    quint32     subchunk2Id;
    quint32     subchunk2Size;
};

struct DATAHeader
{
    chunk       descriptor;
};

struct CombinedHeader
{
    RIFFHeader  riff;
    WAVEHeader  wave;
};

class WavFile : public QFile
{
public:
    WavFile(QObject *parent = 0);

    using QFile::open;
    bool open(const QString &fileName);
    const QAudioFormat &fileFormat() const;
    CombinedHeader header;
    qint64 headerLength() const;
    QVector <qint16> body;

private:
    bool readHeader();
    void readBody(qint64 subchunk2Size);

private:
    QAudioFormat m_fileFormat;
    qint64 m_headerLength;

};

#endif // WAVFILE_H
