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

#ifndef VIDEOMUXERFILTER_H_
#define VIDEOMUXERFILTER_H_

#include "include/Filter.h"
#include "include/Port.h"
#include "types/EncodedFrame.h"
#include "tools/VideoMuxer.h"

class VideoMuxerFilter: public Filter {
private:
	VideoMuxer * videoMuxer;

	InputPort<EncodedFrame> * inputPortEncodedFrame;
public:
	VideoMuxerFilter(string name);
	FilterStatus init();
	FilterStatus process();
	~VideoMuxerFilter();
};

#endif /* VIDEOMUXERFILTER_H_ */
