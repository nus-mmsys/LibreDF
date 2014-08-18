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

#ifndef VIDEOENCODER_H_
#define VIDEOENCODER_H_

#include <string>
#include <iostream>
#include "types/RawFrame.h"
#include "types/EncodedFrame.h"

#ifdef __cplusplus
extern "C" {
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/opt.h>
#ifdef __cplusplus
}
#endif

using namespace std;

class VideoEncoder {
private:
	AVCodecContext * codec_ctx;
    uint8_t *video_outbuf;
    int video_outbuf_size;
public:
	VideoEncoder();
	int init(string codec_name, int width, int height, int bitrate, int framerate);
	int encode(RawFrame * rawFrame, EncodedFrame * encodedFrame);
	virtual ~VideoEncoder();
};

#endif /* VIDEOENCODER_H_ */
