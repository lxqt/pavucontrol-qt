#include "wavplay.h"

void WavPlay::stateCallback(pa_context *c, void *userdata)
{
}

void WavPlay::requestCallback(pa_stream *s, size_t length, void *userdata)
{
}

void WavPlay::underflowCallback(pa_stream *s, void *userdata)
{
}

namespace {
struct WavHeader {
    // RIFF header
    uint32_t ChunkID;
    uint32_t ChunkSize;
    uint32_t Format;

    // fmt subchunk
    uint32_t Subchunk1ID;
    uint32_t Subchunk1Size;

    enum AudioFormats {
        PCM = 0x1,
        ADPCM = 0x2,
        IEEEFloat = 0x3,
        ALaw = 0x6,
        MULaw = 0x7,
        DVIADPCM = 0x11,
        AAC = 0xff,
        WWISE = 0xffffu,
    };

    uint16_t AudioFormat;
    uint16_t NumChannels;
    uint32_t SampleRate;
    uint32_t ByteRate;
    uint16_t BlockAlign;
    uint16_t BitsPerSample;

    // data subchunk
    uint32_t Subchunk2ID;
    uint32_t Subchunk2Size;
};
}


bool WavPlayplaySound(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open" << filename;
        return false;
    }

    m_data = file.readAll(); // yolo

    WavHeader *header = reinterpret_cast<WavHeader*>(m_data.data());

    if (header->AudioFormat != WavHeader::PCM) {
        qWarning() << "Can only play PCM, got audio format" << header->AudioFormat;
        qWarning() << header->Format;
        qWarning() << header->NumChannels;
        qWarning() << header->ChunkID;
        return;
    }

    if (header->NumChannels != 1) {
        qWarning() << "can only play mono samples";
        return;
    }

    int audioFormat = 0;

    //if (header->BitsPerSample == 8) {
    //    audioFormat = STS_MIXER_SAMPLE_FORMAT_8;
    //} else if (header->BitsPerSample == 16) {
    //    audioFormat = STS_MIXER_SAMPLE_FORMAT_16;
    //} else if (header->BitsPerSample == 32) {
    //    audioFormat = STS_MIXER_SAMPLE_FORMAT_32;
    //} else {
    //    WARN << "Unsupported sample format" << header->BitsPerSample;
    //    return;
    //}
}

