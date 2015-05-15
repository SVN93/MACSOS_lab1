#include "wavfile.h"

WavFile::WavFile(QFile *file)
{
    file->open(QIODevice::ReadOnly);
    file->read(reinterpret_cast<char *>(&this->header), sizeof(WAVHEADER));
    this->body.resize(this->header.subchunk2Size / sizeof(qint16));
    qint32 bla = this->header.subchunk2Size / sizeof(qint16);
    for (int i = 0; i < this->header.subchunk2Size / sizeof(qint16); i++) {
        qint16 tmp = 0;
        file->read(reinterpret_cast<char *>(&tmp), sizeof(qint16));
        this->body.append(tmp);
    }
    file->close();
    return;
}

WavFile::~WavFile()
{

}

