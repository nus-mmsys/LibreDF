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

#ifndef ENCODEDFRAME_H_
#define ENCODEDFRAME_H_

#include "types/Data.h"

#ifdef __cplusplus
extern "C" {
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#ifdef __cplusplus
}
#endif

class EncodedFrame : public Data {

private:
	AVPacket * pkt;

public:

	EncodedFrame() {
		pkt = (AVPacket *) av_malloc(sizeof(AVPacket));
		//av_new_packet(pkt,20000000);
	}

	AVPacket * getPacket() {
		return pkt;
	}

};

#endif /* ENCODEDFRAME_H_ */
