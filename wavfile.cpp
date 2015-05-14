#include "wavfile.h"

WavFile::WavFile(QObject *parent) : QObject(parent)
{

}

WavFile::WavFile(QFile *file)
{
    WavFile wavFile;
    file->open(QIODevice::ReadOnly);
    file->read(reinterpret_cast<char *>(&wavFile.header), sizeof(WAVHEADER));
    for (int i = 0; i < N / 2; i++) {
        file->read(reinterpret_cast<char *>(&wavFile.body[i]), sizeof(qint16));
    }

    file->close();
    return;
}

WavFile::~WavFile()
{

}

