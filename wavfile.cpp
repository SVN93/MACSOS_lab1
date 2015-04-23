#include "wavfile.h"

WavFile::WavFile(QObject *parent) : QObject(parent)
{

}

WavFile::WavFile(QFile *file)
{
    WavFile wavFile;
    file->open(QIODevice::ReadOnly);
    file->read(reinterpret_cast<char *>(&wavFile.header), sizeof(WAVHEADER));
    file->close();
    return;
}

WavFile::~WavFile()
{

}

