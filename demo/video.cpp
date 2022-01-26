#include "video.h"
#include "libavformat/avformat.h"
#include "libavutil/pixfmt.h"
#include <cstring>
#include <iostream>
#include <functional>

// final_act allows you to ensure something gets run at the end of a scope
template <class F>
class final_act
{
public:
    explicit final_act(F f) noexcept : f_(std::move(f)), invoke_(true) {}

    final_act(final_act&& other) noexcept : f_(std::move(other.f_)), invoke_(other.invoke_)
    {
        other.invoke_ = false;
    }

    final_act(const final_act&) = delete;
    final_act& operator=(const final_act&) = delete;

    ~final_act() noexcept
    {
        if (invoke_) f_();
    }

private:
    F f_;
    bool invoke_;
};

// finally() - convenience function to generate a final_act
template <class F>
inline final_act<F> finally(const F& f) noexcept
{
    return final_act<F>(f);
}

template <class F>
inline final_act<F> finally(F&& f) noexcept
{
    return final_act<F>(std::forward<F>(f));
}

Video::Video(/* args */)
    : m_pFormatContext(nullptr)
    , m_VideoIndex(-1)
    , m_CodecID(AV_CODEC_ID_NONE)
    , m_pCodecContext(nullptr)
    , m_pFrame(nullptr)
    , m_pPacket(nullptr)
    , m_VideoWidth(0)
    , m_VideoHeight(0)
    , m_Format(VideoPixelFormat::YUV420P)
{
    m_pFormatContext = avformat_alloc_context();
    m_pFrame = av_frame_alloc();
    m_pPacket = av_packet_alloc();
}

Video::~Video()
{
    avformat_free_context(m_pFormatContext);
    av_frame_free(&m_pFrame);
    av_packet_free(&m_pPacket);
}

void Video::Load(const char* file)
{
    if (0 != avformat_open_input(&m_pFormatContext, file, nullptr, nullptr))
    {
        std::cout << "open " << file << " failed" << std::endl;
        return;
    }

    if (0 > avformat_find_stream_info(m_pFormatContext, NULL))
    {
        std::cout << "find stream info err" << std::endl;
        return;
    }
    m_VideoIndex = -1;
    for (int i = 0; i < m_pFormatContext->nb_streams; i++)
    {
        if (m_pFormatContext->streams[i]->codecpar->codec_type == AVMediaType::AVMEDIA_TYPE_VIDEO) 
        {
            m_VideoIndex = i;
            break;
        }
    }

    if (m_VideoIndex == -1)
    {
        std::cout << "failed find video" << std::endl;
    }

    AVCodecID CodecID = m_pFormatContext->streams[m_VideoIndex]->codecpar->codec_id;
    const AVCodec * Codec = avcodec_find_decoder(CodecID);
    if (m_pCodecContext) 
    {
        avcodec_free_context(&m_pCodecContext);
    }
    m_pCodecContext = avcodec_alloc_context3(Codec);
    m_pCodecContext->thread_count = 0;

    if (0 > avcodec_parameters_to_context(m_pCodecContext, m_pFormatContext->streams[m_VideoIndex]->codecpar))
    {
        std::cout << "parameters to avcodec context failed" << std::endl;
        return ;
    }

    if (0 != avcodec_open2(m_pCodecContext, Codec, nullptr))
    {
        std::cout << "init avcodec failed" << std::endl;
        return;
    }

    av_dump_format(m_pFormatContext, m_VideoIndex, file, 0);

    m_VideoWidth = m_pFormatContext->streams[m_VideoIndex]->codecpar->width;
    m_VideoHeight = m_pFormatContext->streams[m_VideoIndex]->codecpar->height;
    if (m_pFormatContext->streams[m_VideoIndex]->codecpar->format == AVPixelFormat::AV_PIX_FMT_YUV420P) 
    {
        m_Format = VideoPixelFormat::YUV420P;
    }

    m_FPS = (float)m_pFormatContext->streams[m_VideoIndex]->avg_frame_rate.num / (float)m_pFormatContext->streams[m_VideoIndex]->avg_frame_rate.den;
    m_NumFrames = m_pFormatContext->streams[m_VideoIndex]->nb_frames;
    m_CurrentFrame = 0;
}

bool Video::AcquireNextFrame(std::vector<std::vector<uint8_t>>& data)
{
    bool FindFrame = false;
    while(0 == av_read_frame(m_pFormatContext, m_pPacket))
    {
        if (m_pPacket->stream_index == m_VideoIndex) 
        {
            int ret = avcodec_send_packet(m_pCodecContext, m_pPacket);
            av_packet_unref(m_pPacket);
            while (0 == ret)
            {
                ret = avcodec_receive_frame(m_pCodecContext, m_pFrame);
                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
                    break;

                m_CurrentFrame = m_pCodecContext->frame_number;
                //std::cout << "receive frame " << m_pCodecContext->frame_number << std::endl;

                if (AVPixelFormat::AV_PIX_FMT_YUV420P == (AVPixelFormat)m_pFrame->format)
                {
                    if (m_pFrame->width % 2 != 0 || m_pFrame->height % 2 != 0)
                    {
                        std::cout << "yuv 420p frame width height not valid" << std::endl;
                        break;
                    }

                    data.resize(3);
                    data[0].resize(m_pFrame->width * m_pFrame->height);
                    data[1].resize(m_pFrame->width * m_pFrame->height / 4);
                    data[2].resize(m_pFrame->width * m_pFrame->height / 4);
                    memcpy(data[0].data(), m_pFrame->data[0], m_pFrame->width * m_pFrame->height * sizeof(uint8_t));
                    memcpy(data[1].data(), m_pFrame->data[1], m_pFrame->width * m_pFrame->height / 4 * sizeof(uint8_t));
                    memcpy(data[2].data(), m_pFrame->data[2], m_pFrame->width * m_pFrame->height / 4 * sizeof(uint8_t));
                    FindFrame = true;
                }
                else
                {
                    std::cout << "not yuv 420p" << std::endl;
                }
            }
            break;
        }
    }

    if (m_CurrentFrame >= m_NumFrames || FindFrame == false)
    {
        m_pCodecContext->frame_number = 0;
        av_seek_frame(m_pFormatContext, m_VideoIndex, 0.0, AVSEEK_FLAG_BACKWARD);
    }

    return FindFrame;
}
