/*
 * Media.h
 *
 *  Created on: Aug 8, 2014
 *      Author: arash
 */

#ifndef MEDIA_H_
#define MEDIA_H_

#include <iostream>
#include <string>

#ifdef __cplusplus
extern "C" {
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#ifdef __cplusplus
}
#endif

using namespace std;

class VideoReader {

private:
	string name;
	AVFormatContext *pFormatCtx;
	AVCodecContext * pCodecCtx;
	int vstream_idx;

public:

	VideoReader(string name) {
		this->name = name;
		pFormatCtx = 0;
		pCodecCtx = 0;
		vstream_idx = -1;
		init(name);
	}
	int init(string name) {

		this->name = name;
		AVCodec *pCodec = 0;
		pFormatCtx = 0;
		pCodecCtx = 0;
		vstream_idx = -1;

		unsigned int i;

		AVDictionary *optionsDict = 0;
		// Register all formats and codecs
		av_register_all();

		if (avformat_open_input(&pFormatCtx, name.c_str(), NULL, NULL) != 0) {
			std::cerr << "Cannot open the input file: " << name << endl;
			return -1; // Couldn't open file
		}

		// Retrieve stream information
		if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
			std::cerr << "Cannot find stream information: " << name << endl;
			return -1; // Couldn't find stream information
		}

		// Find the first video stream
		for (i = 0; i < pFormatCtx->nb_streams; i++)
			if (pFormatCtx->streams[i]->codec->codec_type
					== AVMEDIA_TYPE_VIDEO) {
				vstream_idx = i;
				break;
			}
		if (vstream_idx == -1) {
			std::cerr << "Cannot find a video stream: " << name << endl;
			return -1; // Didn't find a video stream
		}

		// Get a pointer to the codec context for the video stream
		pCodecCtx = pFormatCtx->streams[vstream_idx]->codec;

		// Find the decoder for the video stream
		pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
		if (pCodec == NULL) {
			std::cerr << "Unsupported codec: " << name << endl;
			return -1; // Codec not found
		}
		// Open codec
		if (avcodec_open2(pCodecCtx, pCodec, &optionsDict) < 0) {
			std::cerr << "Cannot open codec: " << name << endl;
			return -1; // Could not open codec
		}

		return 0;

	}

	void dump() {
		// Dump information about file onto standard error
		av_dump_format(pFormatCtx, 0, name.c_str(), 0);
	}

	AVCodecContext * getCodecContext() {
		return pCodecCtx;
	}

	AVFormatContext * getFormatContext() {
		return pFormatCtx;
	}

	AVFrame * readFrame() {

		AVFrame * pFrame = avcodec_alloc_frame();
		AVPacket packet;
		int frameFinished;

		/* Read frames */
		while (av_read_frame(pFormatCtx, &packet) >= 0) {

			// Is this a packet from the video stream?
			if (packet.stream_index == vstream_idx) {

				// Decode video frame
				avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);

				// Did we get a video frame?
				if (frameFinished) {


					av_free_packet(&packet);
					return pFrame;

				}
			}

			// Free the packet that was allocated by av_read_frame
			av_free_packet(&packet);
		}

		return 0;
	}



};

#endif /* MEDIA_H_ */
