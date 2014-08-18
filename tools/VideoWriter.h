/*
 * VideoWriter.h
 *
 *  Created on: Aug 18, 2014
 *      Author: arash
 */

#ifndef VIDEOWRITER_H_
#define VIDEOWRITER_H_

#include <string>
#include "types/RawFrame.h"

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/opt.h>
#include <libavutil/mathematics.h>
#ifdef __cplusplus
}
#endif

class VideoWriter {
private:
	AVCodec *codec;
    AVFormatContext *oc;
    AVStream *video_st;
    uint8_t *video_outbuf;
    int video_outbuf_size;
	AVOutputFormat *fmt;

    int open_video();
    AVStream *add_video_stream(int width, int height);
public:
	VideoWriter();
	int init(std::string filename, int width, int height);
	int write(RawFrame * rawFrame);
	~VideoWriter();
};

#endif /* VIDEOWRITER_H_ */
