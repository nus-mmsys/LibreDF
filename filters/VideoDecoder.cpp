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

#include "VideoDecoder.h"

#include <iostream>

VideoDecoder::VideoDecoder(string name) :
		Filter(name) {

	outputFrame = new OutputPort<AVFrame>("videoDecoder, output 1, AVFrame",
			this);

	outputPorts.push_back(outputFrame);

	pFormatCtx = 0;
	vstream_idx = -1;
}

FilterStatus VideoDecoder::init() {

	unsigned int i;
	AVCodec *pCodec = 0;
	AVCodecContext *pCodecCtx = 0;
	AVDictionary *optionsDict = 0;
	// Register all formats and codecs
	av_register_all();

	string videoName = getProp("input_video");
	if (avformat_open_input(&pFormatCtx, videoName.c_str(), NULL, NULL) != 0) {
		std::cerr << "Cannot open the input file: " << videoName << endl;
		return FILTER_ERROR; // Couldn't open file
	}

	// Retrieve stream information
	if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
		std::cerr << "Cannot find stream information: " << videoName << endl;
		return FILTER_ERROR; // Couldn't find stream information
	}

	// Dump information about file onto standard error
	av_dump_format(pFormatCtx, 0, videoName.c_str(), 0);

	// Find the first video stream
	for (i = 0; i < pFormatCtx->nb_streams; i++)
		if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
			vstream_idx = i;
			break;
		}
	if (vstream_idx == -1) {
		std::cerr << "Cannot find a video stream: " << videoName << endl;
		return FILTER_ERROR; // Didn't find a video stream
	}

	// Get a pointer to the codec context for the video stream
	pCodecCtx = pFormatCtx->streams[vstream_idx]->codec;

	// Find the decoder for the video stream
	pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
	if (pCodec == NULL) {
		std::cerr << "Unsupported codec: " << videoName << endl;
		return FILTER_ERROR; // Codec not found
	}
	// Open codec
	if (avcodec_open2(pCodecCtx, pCodec, &optionsDict) < 0) {
		std::cerr << "Cannot open codec: " << videoName << endl;
		return FILTER_ERROR; // Could not open codec
	}

	cout << "w: " << pCodecCtx->width << "h: " << pCodecCtx->height << "p: " << pCodecCtx->pix_fmt << endl;
	return FILTER_SUCCESS;

}

FilterStatus VideoDecoder::process() {

	AVFrame * pFrame = avcodec_alloc_frame();
	AVPacket packet;
	AVCodecContext *pCodecCtx;
	int frameFinished;
	BufferNode<AVFrame> bn;
	//int ret;

	/* Get a pointer to the codec context for the video stream */
	pCodecCtx = pFormatCtx->streams[vstream_idx]->codec;

	/* Read frames */
	while (av_read_frame(pFormatCtx, &packet) >= 0) {

		// Is this a packet from the video stream?
		if (packet.stream_index == vstream_idx) {

			// Decode video frame
			avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);

			// Did we get a video frame?
			if (frameFinished) {
				bn.setData(pFrame);
				outputFrame->produce(&bn);
				outputFrame->process();

				av_free_packet(&packet);
				return FILTER_SUCCESS;

			}
		}

		// Free the packet that was allocated by av_read_frame
		av_free_packet(&packet);
	}

	return FILTER_ERROR;
}

VideoDecoder::~VideoDecoder() {
}
