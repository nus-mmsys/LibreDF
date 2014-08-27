/*
 *
 *  Tiny Multimedia Framework
 *  Copyright (C) 2014 Arash Shafiei
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef VIDEOREADER_H_
#define VIDEOREADER_H_

#include <iostream>
#include <string>
#include <types/RawFrame.h>

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
	int frameNumber;

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
		frameNumber = 0;

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

	int getWidth() {
		return pCodecCtx->width;
	}

	int getHeight() {
		return pCodecCtx->height;
	}

	AVPixelFormat getPixFormat() {
		return pCodecCtx->pix_fmt;
	}

	AVCodecContext * getCodecContext() {
		return pCodecCtx;
	}

	AVFormatContext * getFormatContext() {
		return pFormatCtx;
	}

	int readFrame(RawFrame * frame) {

		AVFrame * pFrame = frame->getFrame();
		AVPacket packet;
		int frameFinished;

		avcodec_get_frame_defaults(pFrame);

		// Read frames
		while (av_read_frame(pFormatCtx, &packet) >= 0) {

			// Is this a packet from the video stream?
			if (packet.stream_index == vstream_idx) {



				// Decode video frame
				avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished,
						&packet);

				// Did we get a video frame?
				if (frameFinished) {

					av_free_packet(&packet);
					frame->setNumber(frameNumber);
					frameNumber++;
					return 0;

				}
			}

			// Free the packet that was allocated by av_read_frame
			av_free_packet(&packet);
		}

		// Flush decoder
		avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);

		// Did we get a video frame?
		if (frameFinished) {

			av_free_packet(&packet);
			frame->setNumber(frameNumber);
			frameNumber++;
			return 0;

		}

		return -1;
	}


	~VideoReader() {

		// Close the codec
		avcodec_close(pCodecCtx);

		// Close the video file
		avformat_close_input(&pFormatCtx);
	}

};

#endif /* VIDEOREADER_H_ */
