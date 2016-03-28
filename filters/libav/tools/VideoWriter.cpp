/*
 * 
 *  Tiny Multimedia Framework (tmf)
 *  Copyright (C) 2014 Arash Shafiei
 *
 *  This file is part of tmf.
 *
 *  tmf is free software; you can redistribute it and/or 
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 3 of the License, or (at your option) any later version.
 *
 *  tmf is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with tmf.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <iostream>

#include "filters/libav/tools/VideoWriter.h"
#include <filters/libav/types/EncodedFrame.h>

#define STREAM_FRAME_RATE 25
#define STREAM_PIX_FMT PIX_FMT_YUV420P /* default pix_fmt */

VideoWriter::VideoWriter() {
  
  codec = 0;
  fmt = 0;
  oc = 0;
  video_st = 0;
  video_outbuf = 0;
  video_outbuf_size = 0;
  encodedFrame = new EncodedFrame();
  
}

int VideoWriter::open_video() {
  
  //AVCodecContext *c;
  
  //c = video_st->codec;
  
  /* find the video encoder */
  //codec = avcodec_find_encoder(video_st->codec->codec_id);
  //if (!codec) {
  //	fprintf(stderr, "codec not found\n");
  //	return -1;
  //}
  
  /* open the codec */
  if (avcodec_open2(video_st->codec, codec, NULL) < 0) {
    fprintf(stderr, "could not open codec\n");
    return -1;
  }
  
  video_outbuf = NULL;
  if (!(oc->oformat->flags & AVFMT_RAWPICTURE)) {
    /* allocate output buffer */
    /* XXX: API change will be done */
    /* buffers passed into lav* can be allocated any way you prefer,
     *		 as long as they're aligned enough for the architecture, and
     *		 they're freed appropriately (such as using av_free for buffers
     *		 allocated with av_malloc) */
    video_outbuf_size = 2000000;
    video_outbuf = (uint8_t*) av_malloc(video_outbuf_size);
  }
  
  return 0;
  
}

/* add a video output stream */
AVStream * VideoWriter::add_video_stream(int width, int height) {
  AVCodecContext *c;
  AVStream *st;
  
  codec = avcodec_find_encoder(fmt->video_codec);
  if (!codec) {
    fprintf(stderr, "codec not found\n");
    return NULL;
  }
  
  st = avformat_new_stream(oc, codec);
  if (!st) {
    fprintf(stderr, "Could not alloc stream\n");
    return NULL;
  }
  
  c = st->codec;
  c->codec_id = fmt->video_codec;
  c->codec_type = AVMEDIA_TYPE_VIDEO;
  
  /* put sample parameters */
  c->bit_rate = 1000000;
  /* resolution must be a multiple of two */
  c->width = width;
  c->height = height;
  /* time base: this is the fundamental unit of time (in seconds) in terms
   *	 of which frame timestamps are represented. for fixed-fps content,
   *	 timebase should be 1/framerate and timestamp increments should be
   *	 identically 1. */
  c->time_base.den = STREAM_FRAME_RATE;
  c->time_base.num = 1;
  c->gop_size = 12; /* emit one intra frame every twelve frames at most */
  c->pix_fmt = STREAM_PIX_FMT;
  
  c->max_b_frames = 12;
  //av_opt_set(c->priv_data, "profile", "baseline", 0);
  //av_opt_set(c->priv_data, "preset", "medium", 0);
  //av_opt_set(c->priv_data, "tune", "zerolatency", 0);
  
  if (c->codec_id == CODEC_ID_MPEG2VIDEO) {
    /* just for testing, we also add B frames */
    c->max_b_frames = 2;
  }
  if (c->codec_id == CODEC_ID_MPEG1VIDEO) {
    /* Needed to avoid using macroblocks in which some coeffs overflow.
     *		 This does not happen with normal video, it just happens here as
     *		 the motion of the chroma plane does not match the luma plane. */
    c->mb_decision = 2;
  }
  // some formats want stream headers to be separate
  if (oc->oformat->flags & AVFMT_GLOBALHEADER)
    c->flags |= CODEC_FLAG_GLOBAL_HEADER;
  
  return st;
}

int VideoWriter::init(std::string filename, int width, int height) {
  
  fmt = av_guess_format(NULL, filename.c_str(), NULL);
  if (!fmt) {
    printf(
      "Could not deduce output format from file extension: using MPEG.\n");
    fmt = av_guess_format("mpeg", NULL, NULL);
  }
  if (!fmt) {
    fprintf(stderr, "Could not find suitable output format\n");
    return -1;
  }
  
  /* allocate the output media context */
  oc = avformat_alloc_context();
  if (!oc) {
    fprintf(stderr, "Memory error\n");
    return -1;
  }
  oc->oformat = fmt;
  snprintf(oc->filename, sizeof(oc->filename), "%s", filename.c_str());
  
  /* add the audio and video streams using the default format codecs
   *	 and initialize the codecs */
  video_st = NULL;
  if (fmt->video_codec != CODEC_ID_NONE) {
    video_st = add_video_stream(width, height);
  }
  
  av_dump_format(oc, 0, filename.c_str(), 1);
  
  /* now that all the parameters are set, we can open the audio and
   *	 video codecs and allocate the necessary encode buffers */
  if (video_st) {
    if (open_video() < 0)
      return -1;
  }
  
  /* open the output file, if needed */
  if (!(fmt->flags & AVFMT_NOFILE)) {
    if (avio_open(&oc->pb, filename.c_str(), AVIO_FLAG_WRITE) < 0) {
      fprintf(stderr, "Could not open '%s'\n", filename.c_str());
      return -1;
    }
  }
  
  /* write the stream header, if any */
  avformat_write_header(oc, NULL);
  
  return 0;
}

int VideoWriter::write(RawFrame * rawFrame) {
  
  int out_size, ret;
  AVCodecContext *c;
  int got_packet = 0;
  
  AVFrame * picture = rawFrame->getFrame();

  AVPacket * pkt = encodedFrame->getPacket();
  
  c = video_st->codec;
  
  picture->pts = rawFrame->getNumber();
  av_init_packet(pkt);
  pkt->data = encodedFrame->vbuf;
  pkt->size = encodedFrame->vbuf_size;
  
  /* encode the image */
  ret = avcodec_encode_video2(c, pkt, picture, &got_packet);
  /* if zero size, it means the image was buffered */
  if (got_packet) {
    
    if (c->coded_frame->pts != AV_NOPTS_VALUE)
      pkt->pts = av_rescale_q(c->coded_frame->pts, c->time_base,
			     video_st->time_base);
      if (c->coded_frame->key_frame)
	pkt->flags |= AV_PKT_FLAG_KEY;
      pkt->stream_index = video_st->index;
    
    /* write the compressed frame in the media file */
    av_interleaved_write_frame(oc, pkt);
  }
  
  if (ret < 0) {
    std::cerr << "Error while writing video frame\n";
    return -1;
  }
  
  return 0;
}

VideoWriter::~VideoWriter() {
  
  unsigned int i;
  /* write the trailer, if any.  the trailer must be written
   * before you close the CodecContexts open when you wrote the
   * header; otherwise write_trailer may try to use memory that
   * was freed on av_codec_close() */
  av_write_trailer(oc);
  
  /* close each codec */
  if (video_st) {
    avcodec_close(video_st->codec);
    
    av_free(video_outbuf);
  }
  
  /* free the streams */
  for (i = 0; i < oc->nb_streams; i++) {
    av_freep(&oc->streams[i]->codec);
    av_freep(&oc->streams[i]);
  }
  
  if (!(fmt->flags & AVFMT_NOFILE)) {
    /* close the output file */
    avio_close(oc->pb);
  }
  
  /* free the stream */
  av_free(oc);
}

