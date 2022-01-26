#pragma once

#include <stdint.h>
#include <vector>
extern "C" 
{
#include <libavcodec/codec.h>
#include <libavcodec/codec_id.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/avutil.h>
#include <libswresample/swresample.h>
#include <libavdevice/avdevice.h>
#include <libavfilter/avfilter.h>
#include <libavutil/frame.h>
#include <libavcodec/packet.h>
}

enum class VideoPixelFormat
{
    YUV420P,
};

class Video
{
public:
    Video(/* args */);
    ~Video();

    void Load(const char* file);
    bool AcquireNextFrame(std::vector<std::vector<uint8_t>>& data);
    uint32_t GetWidth() { return m_VideoWidth; }
    uint32_t GetHeight() { return m_VideoHeight; }
    VideoPixelFormat GetFormat() { return m_Format; }
    float GetFPS() { return m_FPS; }

private:
    AVFormatContext * m_pFormatContext;
    int32_t m_VideoIndex;

    AVCodecID m_CodecID;
    AVCodecContext* m_pCodecContext;
    AVFrame* m_pFrame;
    AVPacket* m_pPacket;

    uint32_t m_VideoWidth;
    uint32_t m_VideoHeight;
    VideoPixelFormat m_Format;

    float m_FPS;
    uint32_t m_NumFrames;
    uint32_t m_CurrentFrame;
};
