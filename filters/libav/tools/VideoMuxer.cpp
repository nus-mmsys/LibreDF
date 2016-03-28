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

#include "filters/libav/tools/VideoMuxer.h"

VideoMuxer::VideoMuxer() {
  av_fmt_ctx = 0;
  vstream_idx = 0;
  
}

int VideoMuxer::init(string filename, int width, int height, int bitrate, int framerate) {
  
  AVCodec *codec;
  AVStream *video_stream;
  AVOutputFormat *output_fmt;
  AVDictionary* opts = 0;
  
  av_fmt_ctx = 0;
  
  /* Find output format */
  output_fmt = av_guess_format(0, filename.c_str(), 0);
  if (!output_fmt) {
    cout << "Cannot find suitable output format\n";
    return -1;
  }
  
  av_fmt_ctx = avformat_alloc_context();
  if (!av_fmt_ctx) {
    cout << "Cannot allocate memory for pOutVideoFormatCtx\n";
    return -1;
  }
  
  av_fmt_ctx->oformat = output_fmt;
  strcpy(av_fmt_ctx->filename, filename.c_str());
  
  /* Open the output file */
  if (!(output_fmt->flags & AVFMT_NOFILE)) {
    if (avio_open(&av_fmt_ctx->pb, filename.c_str(), AVIO_FLAG_WRITE) < 0) {
      cout << "Cannot not open " << filename << endl;
      return -1;
    }
  }
  
  /* find the video encoder */
  codec = avcodec_find_encoder(output_fmt->video_codec); //(output_fmt->video_codec);
  if (!codec) {
    cout << "Codec not found\n";
    return -1;
  }
  
  video_stream = avformat_new_stream(av_fmt_ctx, codec);
  if (!video_stream) {
    cout << "Cannot create output video stream\n";
    return -1;
  }
  
  video_stream->codec->codec_id = codec->id;
  video_stream->codec->codec_type = AVMEDIA_TYPE_VIDEO;
  video_stream->codec->width = width;
  video_stream->codec->height = height;
  video_stream->codec->time_base = AVRational { 1, framerate };
  video_stream->codec->pix_fmt = PIX_FMT_YUV420P;
  video_stream->codec->bit_rate = bitrate;
  
  //video_stream->codec->time_base = time_base;
  
  //video_stream->codec->ticks_per_frame = 2;
  
  video_stream->codec->gop_size = framerate;
  //video_stream->codec->max_b_frames = 12;
  //video_stream->codec->rc_max_rate = 0;
  //video_stream->codec->rc_buffer_size = 0;
  
  av_opt_set(video_stream->codec->priv_data, "preset", "ultrafast", 0);
  av_opt_set(video_stream->codec->priv_data, "tune", "zerolatency", 0);
  
  /* open the video codec */
  if (avcodec_open2(video_stream->codec, codec, NULL) < 0) {
    cout << "Cannot open output video codec\n";
    return -1;
  }
  av_dict_set(&opts, "frag_duration", "1000000", 0);
  av_dict_set(&opts, "movflags", "empty_moov", 0);
  avformat_write_header(av_fmt_ctx, &opts);
  
  return 0;
}

int VideoMuxer::mux(EncodedFrame * encodedFrame) {
  
  AVPacket * encPkt = encodedFrame->getPacket();
  
  //Encoder is still buffering.
  if (encPkt->size == 0)
    return 0;
  
  AVPacket pkt;
  AVStream *video_stream = av_fmt_ctx->streams[vstream_idx];
  AVCodecContext *video_codec_ctx = video_stream->codec;
  
  av_init_packet(&pkt);
  pkt.data = NULL;
  pkt.size = 0;
  
  if (video_codec_ctx->coded_frame->pts != AV_NOPTS_VALUE) {
    pkt.pts = av_rescale_q(video_codec_ctx->coded_frame->pts,
			   video_codec_ctx->time_base, video_stream->time_base);
  }
  
  if (video_codec_ctx->coded_frame->key_frame)
    pkt.flags |= AV_PKT_FLAG_KEY;
  
  pkt.stream_index = video_stream->index;
  pkt.data = encPkt->data;
  pkt.size = encPkt->size;
  
  // write the compressed frame in the media file
  if (av_interleaved_write_frame(av_fmt_ctx, &pkt) != 0) {
    cout << "Writing frame is not successful\n";
    return -1;
  }
  
  av_free_packet(&pkt);
  
  return 0;
}

VideoMuxer::~VideoMuxer() {
  
  unsigned int i;
  
  av_write_trailer(av_fmt_ctx);
  
  avio_close(av_fmt_ctx->pb);
  
  // free the streams
  for (i = 0; i < av_fmt_ctx->nb_streams; i++) {
    avcodec_close(av_fmt_ctx->streams[i]->codec);
    av_freep(&av_fmt_ctx->streams[i]->info);
  }
  
  avformat_free_context(av_fmt_ctx);
}

