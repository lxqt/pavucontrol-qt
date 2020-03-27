#pragma once

struct WavPlay
{
    static void stateCallback(pa_context *c, void *userdata);
    static void requestCallback(pa_stream *s, size_t length, void *userdata);
    static void underflowCallback(pa_stream *s, void *userdata);

    bool playSound(const QString &file);


private:
    size_t m_position = 0;
    QByteArray m_data;
}

