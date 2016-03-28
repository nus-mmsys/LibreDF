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

#include "filters/libav/tools/VideoEncoder.h"

VideoEncoder::VideoEncoder() {
  codec_ctx = 0;
  video_outbuf_size = 2000000;
  video_outbuf = (uint8_t*) av_malloc(video_outbuf_size);
  
}

int VideoEncoder::init(string filename, int width, int height, int bitrate, int framerate)
{
  
  AVOutputFormat *output_fmt;
  
  /* Find output format */
  output_fmt = av_guess_format(0, filename.c_str(), 0);
  if (!output_fmt) {
    cout << "Cannot find suitable output format\n";
    return -1;
  }
  
  /* find the video encoder */
  AVCodec * codec = avcodec_find_encoder(output_fmt->video_codec);//(output_fmt->video_codec);
  if (!codec) {
    cout << "Codec not found\n";
    return -1;
  }
  
  //AVCodec * codec = avcodec_find_encoder_by_name(codec_name.c_str());
  //if (codec == NULL) {
  //	cout << "Output video codec " << codec_name << " not found\n";
  //	return -1;
  //}
  
  codec_ctx = avcodec_alloc_context3(codec);
  
  codec_ctx->codec_id = codec->id;
  codec_ctx->codec_type = AVMEDIA_TYPE_VIDEO;
  codec_ctx->bit_rate = bitrate;
  codec_ctx->width = width;
  codec_ctx->height = height;
  //codec_ctx->sample_aspect_ratio.num = width;
  //codec_ctx->sample_aspect_ratio.den = height;
  
  codec_ctx->time_base = AVRational { 1, framerate };
  
  //codec_ctx->time_base.den = gop_size ? gop_size : framerate;
  
  //if (use_source_timing) {
  //for avcodec to do rate allocation, we need to have ctx->timebase == 1/framerate
  //	codec_ctx->time_base.den = time_base.den;
  //	codec_ctx->time_base.num = time_base.num * time_base.den / framerate;
  //}
  
  codec_ctx->pix_fmt = PIX_FMT_YUV420P;
  codec_ctx->gop_size = framerate;
  
  av_opt_set(codec_ctx->priv_data, "preset", "ultrafast", 0);
  av_opt_set(codec_ctx->priv_data, "tune", "zerolatency", 0);
  //codec_ctx->max_b_frames = 0;
  
  //the global header gives access to the extradata (SPS/PPS)
  //codec_ctx->flags |= CODEC_FLAG_GLOBAL_HEADER;
  
  //vstream_idx = 0;
  
  /* open the video codec - options are passed thru  codec_ctx->priv_data */
  if (avcodec_open2(codec_ctx, codec, NULL) < 0) {
    cout << "Cannot open output video codec\n";
    return -1;
  }
  
  //rep_id = filename;
  return 0;
  
}

VideoEncoder::~VideoEncoder() {
  avcodec_close(codec_ctx);
  av_free(codec_ctx);
}
int VideoEncoder::encode(RawFrame * rawFrame, EncodedFrame * encodedFrame) {
  int ret;
  int out_size;
  AVFrame * vframe = rawFrame->getFrame();
  AVPacket * pkt = encodedFrame->getPacket();
  vframe->pts = rawFrame->getNumber();
  //encodedFrame->number = rawFrame->number;
  
  int got_packet = 0;
  av_init_packet(pkt);
  
  //encodedFrame->pkt->pts = encodedFrame->pkt->dts = vframe->pkt_dts =
  //vframe->pkt_pts = vframe->pts;
  
  pkt->data = encodedFrame->vbuf;
  pkt->size = encodedFrame->vbuf_size;
  
  ret = avcodec_encode_video2(codec_ctx, pkt, vframe, &got_packet);
  
  if (got_packet) {
    vframe->pts = codec_ctx->coded_frame->pkt_pts = pkt->pts;
    codec_ctx->coded_frame->pkt_dts = pkt->dts;
    codec_ctx->coded_frame->key_frame = (pkt->flags & AV_PKT_FLAG_KEY) ? 1 : 0;
  } 

  if (ret < 0) {
    cout << "Error occurred while encoding video frame.\n";
    return -1;
  }
 
  return ret;
}
