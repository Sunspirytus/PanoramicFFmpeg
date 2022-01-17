#include "video.h"

extern "C" 
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/avutil.h>
#include <libswresample/swresample.h>
#include <libavdevice/avdevice.h>
#include <libavfilter/avfilter.h>
}

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
{
}

Video::~Video()
{
}

void Video::Load(const char* file)
{
    AVFormatContext *FormatContext = avformat_alloc_context();
    auto F = finally([FormatContext]{
            avformat_free_context(FormatContext);
        }
    );

    if (0 != avformat_open_input(&FormatContext, file, nullptr, nullptr))
    {
        std::cout << "open " << file << " failed" << std::endl;
        return;
    }
    
    if (0 >= avformat_find_stream_info(FormatContext, NULL))
    {
        std::cout << "find stream info err" << std::endl;
        return;
    }
    int videoIndex = -1;
    for (int i = 0; i < FormatContext->nb_streams; i++)
    {
        if (FormatContext->streams[i]->codecpar->codec_type == AVMediaType::AVMEDIA_TYPE_VIDEO) 
        {
            videoIndex = i;
            break;
        }
    }


}