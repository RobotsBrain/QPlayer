/** VD */
#pragma once

#include "common.hpp"
#include "proto.hpp"
#include "timeline.hpp"

#include <QWidget>
#include <QMutex>
#include <SDL2/SDL.h>

extern "C" {
#include <libswresample/swresample.h>
}

struct SwsContext;

namespace vd {

class SdlAudio;
class SdlFfmpegAudioDecoder;

typedef std::shared_ptr<SdlVideoFrame> SdlVideoFramePtr;

struct SdlAudioSpec
{
	int freq;
	int channels;
	AVSampleFormat format;
	int64_t channel_layout;
};

struct SdlAudioChannel
{
	int channel;
	float volume;
};


class SdlVideoFrame : public IFrame
{
public:
	friend class SdlBlitter;
	friend class SdlRenderer;

    SdlVideoFrame(SDL_Texture* overlay);

	~SdlVideoFrame();

protected:
    SDL_Texture* overlay_;
};

class SdlAudioBuffer : public IFrame
{
public:
	SdlAudioBuffer();

	~SdlAudioBuffer();

protected:

};

class SdlBlitter
{
public:
	virtual ~SdlBlitter() {}

	virtual void blit(SdlVideoFrame* dst, void* src) = 0;

    SDL_Texture* sdl_overlay(SdlVideoFrame* frame);
};

class SdlFfmpegBlit : public SdlBlitter
{
public:

	SdlFfmpegBlit(SwsContext* img_convert_ctx);

	void blit(SdlVideoFrame* dst, void* src) VD_OVERRIDE;

protected:
	SwsContext* img_convert_ctx_;
};

class SdlVideoPresenter : public IFramePresenter
{
public:
	SdlVideoPresenter(SdlRenderer* renderer);

	IFramePtr prepare(IFramePtr frame) VD_OVERRIDE;

protected:
	SdlBlitter* get_blitter(IFramePtr frame);

protected:
	SdlRenderer* renderer_;
	SdlFfmpegBlit* ffmpeg_blitter_;
};

class SdlAudioPresenter : public IFramePresenter
{
public:
	SdlAudioPresenter(SdlAudio* audio);

	IFramePtr prepare(IFramePtr frame) VD_OVERRIDE;

protected:
	SdlAudio* audio_;

	SdlFfmpegAudioDecoder* audio_decoder_;
};


class SdlAudioFrame : public IFrame
{
public:
	SdlAudioFrame();

	static const size_t buf_allocated = 4096 * 48;
	uint8_t buf[buf_allocated];
	size_t size;
};

class SdlFfmpegAudioDecoder
{
public:

	SdlFfmpegAudioDecoder(SdlAudio* audio);

	bool decode(SdlAudioFrame* dst_frame, FfmpegFrame* src_frame);

protected:
	SdlAudio* audio_;
};


extern double sdl_audio_diff;

class SdlAudio
{
public:
	SdlAudio();

	void open(const SdlAudioSpec& spec);

	void queue_audio(const SdlAudioChannel& channel, MovieResourcePtr ptr);

	bool write(const SdlAudioChannel& channel, const SdlAudioFrame& frame);

	void render_audio(MovieResourcePtr audio);

	void _audio_callback(uint8_t* stream, int len);

	const SdlAudioSpec& spec() const { return spec_; }

	void sigwrite();

	bool enough_audio();

protected:
	SdlAudioSpec spec_;

	bool write_;

	QMutex mutex_;

	static const size_t audio_buf_sz_ = 4096 * 4;
	uint8_t audio_buf_[audio_buf_sz_];
	size_t audio_buf_index_;
};

class SdlRenderer : public QWidget/*, public Compositor*/ {
    Q_OBJECT

public:
    SdlRenderer(QWidget* parent = 0, Qt::WindowFlags f = 0);
	virtual ~SdlRenderer();

	void init_overlay(int width, int height);

	void render_video(MovieResourcePtr video_frame);

	//CompositorId pre_compose(IFramePtr frame) VD_OVERRIDE;
	//void do_compose(CompositorId frame_id) VD_OVERRIDE;

	SdlVideoFrame* new_frame();
	void free_frame(SdlVideoFrame* frame);

    SDL_Window* screen() { return screen_; }
    SDL_Texture* overlay() { return overlay_; }

protected:
	

private:
    SDL_Window* screen_;
    SDL_Renderer* sdlRenderer_;
    SDL_Texture* overlay_;
    SDL_Texture* black_screen_;
};

}// namespace vd
