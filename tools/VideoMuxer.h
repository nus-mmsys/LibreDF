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

#ifndef VIDEOMUXER_H_
#define VIDEOMUXER_H_

#include <string>
#include <iostream>
#include "types/EncodedFrame.h"

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

using namespace std;

class VideoMuxer {
private:
	AVFormatContext * av_fmt_ctx;
	int vstream_idx;
public:
	VideoMuxer();
	int init(string filename, int width, int height, int bitrate);
	int mux(EncodedFrame * encodedFrame);
	~VideoMuxer();
};

#endif /* VIDEOMUXER_H_ */
