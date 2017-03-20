#include <string>
#include <jni.h>
#include "android-log.h"
#include <string.h>
#include <stdio.h>
#include <android/bitmap.h>



extern "C"
{
#include <libavcodec/avcodec.h>
#include "ffmpeg.h"
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>

/* Cheat to keep things simple and just use some globals. */
AVFormatContext *pFormatCtx;
AVCodecContext *pCodecCtx;
AVFrame *pFrame;
AVFrame *pFrameRGB;
int videoStream;


/*
 * Write a frame worth of video (in pFrame) into the Android bitmap
 * described by info using the raw pixel buffer.  It's a very inefficient
 * draw routine, but it's easy to read. Relies on the format of the
 * bitmap being 8bits per color component plus an 8bit alpha channel.
 */

static void fill_bitmap(AndroidBitmapInfo *info, void *pixels, AVFrame *pFrame) {
    uint8_t *frameLine;

    int yy;
    for (yy = 0; yy < info->height; yy++) {
        uint8_t *line = (uint8_t *) pixels;
        frameLine = (uint8_t *) pFrame->data[0] + (yy * pFrame->linesize[0]);

        int xx;
        for (xx = 0; xx < info->width; xx++) {
            int out_offset = xx * 4;
            int in_offset = xx * 3;

            line[out_offset] = frameLine[in_offset];
            line[out_offset + 1] = frameLine[in_offset + 1];
            line[out_offset + 2] = frameLine[in_offset + 2];
            line[out_offset + 3] = 0;
        }
        pixels = (char *) pixels + info->stride;
    }
}


int seek_frame(int tsms) {
    int64_t frame;

    frame = av_rescale(tsms, pFormatCtx->streams[videoStream]->time_base.den,
                       pFormatCtx->streams[videoStream]->time_base.num);
    frame /= 1000;

    if (avformat_seek_file(pFormatCtx, videoStream, 0, frame, frame, AVSEEK_FLAG_FRAME) < 0) {
        return 0;
    }

    avcodec_flush_buffers(pCodecCtx);

    return 1;
}





JNIEXPORT jstring JNICALL
Java_work_wanghao_jni4ffmpeg_Native4FFmpegHelper_run(JNIEnv *env, jclass type, jint argc,
                                                     jobjectArray args) {

    char buff[1024];
    snprintf(buff, sizeof(buff), "ffmpeg -ss 00:00:01 -i %s %s -r 1 -vframes 1 -an -vcodec mjpeg",
             "Hello", "world");
    std::string buffAsStdStr = buff;


    char tab2[1024];
    strncpy(tab2, buffAsStdStr.c_str(), sizeof(tab2));
    tab2[sizeof(tab2) - 1] = 0;

    int resultCode = run(strlen(tab2), (char **) tab2);
}

JNIEXPORT jstring JNICALL
Java_work_wanghao_jni4ffmpeg_Native4FFmpegHelper_getAvCodec(JNIEnv *env, jclass type) {

//    char buff[1024];
//    snprintf(buff, sizeof(buff), "ffmpeg -ss 00:00:01 -i %s %s -r 1 -vframes 1 -an -vcodec mjpeg",
//             "Hello", "world");
//    std::string buffAsStdStr = buff;
//
//
//    char tab2[1024];
//    strncpy(tab2, buffAsStdStr.c_str(), sizeof(tab2));
//    tab2[sizeof(tab2) - 1] = 0;
//
//    int resultCode = run(strlen(tab2), (char **) tab2);

    return env->NewStringUTF(avcodec_configuration());
}


JNIEXPORT jint JNICALL
Java_work_wanghao_jni4ffmpeg_Native4FFmpegHelper_getBitmap(JNIEnv *env, jclass type,
                                                           jstring sourcePath_, jstring savePath_) {
    const char *sourcePath = env->GetStringUTFChars(sourcePath_, 0);
    const char *savePath = env->GetStringUTFChars(savePath_, 0);
    LOGD(sourcePath);
    LOGD(savePath);


//    char const *str1;
//    int n = 0;
//    char *argv[20];
//    jbyte* str[3];
//    jstringToCstr(env,videoPath,&str[0]);
//    jstringToCstr(env,audioPath,&str[1]);
//    jstringToCstr(env,avPath,&str[2]);
//
//    argv[n++] = "ffmpeg";
//    argv[n++] = "-i";
//    argv[n++] = str[0];
//    argv[n++] = "-i";
//    argv[n++] = str[1];
//    argv[n++] = "-y";
//    argv[n++] = "-strict";
//    argv[n++] = "-2";
//    argv[n++] = str[2];
//    int ret = vedio_merge(n, argv);
//    str1 = "Using FFMPEG doing your job";
//    return (*env)->NewStringUTF(env, str1);


    char *argv[30];
    int argc = 0;


    argv[argc++] = (char *) "ffmpeg";
    argv[argc++] = (char *) "-ss";
    argv[argc++] = (char *) "00:00:01";
    argv[argc++] = (char *) "-i";
    argv[argc++] = (char *) sourcePath;
    argv[argc++] = (char *) "-f";
    argv[argc++] = (char *) "image2";
    argv[argc++] = (char *) "-y";
    argv[argc++] = (char *) savePath;
//    argv[argc++] = (char *) savePath;
//    argv[argc++] = (char *) "-r";
//    argv[argc++] = (char *) "-1";
//    argv[argc++] = (char *) "-vframes";
//    argv[argc++] = (char *) "1";
//    argv[argc++] = (char *) "-an";
//    argv[argc++] = (char *) "-vcodec";
//    argv[argc++] = (char *) "mjpeg";
//    argv[n++] = str[0];
//    argv[n++] = "-i";
//    argv[n++] = str[1];
//    argv[n++] = "-y";
//    argv[n++] = "-strict";
//    argv[n++] = "-2";
//    argv[n++] = str[2];

//
//    char buff[1024];
//    snprintf(buff, sizeof(buff), "ffmpeg -ss 00:00:01 -i %s %s -r 1 -vframes 1 -an -vcodec mjpeg",
//             sourcePath, savePath);
//    std::string buffAsStdStr = buff;
//
//    LOGD(buffAsStdStr.c_str());
//
//    char tab2[1024];
//    strncpy(tab2, buffAsStdStr.c_str(), sizeof(tab2));
//    tab2[sizeof(tab2) - 1] = 0;

    int resultCode = run(argc, argv);
    if (resultCode == 0) {
        LOGD("返回值为0");
    } else {
        LOGE("返回值为非0");
    }

    env->ReleaseStringUTFChars(sourcePath_, sourcePath);
    env->ReleaseStringUTFChars(savePath_, savePath);
}


JNIEXPORT void JNICALL
Java_work_wanghao_jni4ffmpeg_Native4FFmpegHelper_openFile(JNIEnv *env, jclass type, jstring path_) {
    const char *path = env->GetStringUTFChars(path_, 0);


    int ret;
    int err;
    int i;
    AVCodec *pCodec;
    uint8_t *buffer;
    int numBytes;

    av_register_all();
    LOGE("Registered formats");
//    err = av_open_input_file(&pFormatCtx, "file:/sdcard/vid.3gp", NULL, 0, NULL);
    err = avformat_open_input(&pFormatCtx, path, NULL, NULL);
    LOGE("Called open file");
    if (err != 0) {
        LOGE("Couldn't open file");
        return;
    }
    LOGE("Opened file");

    if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
        LOGE("Unable to get stream info");
        return;
    }

    videoStream = -1;
    for (i = 0; i < pFormatCtx->nb_streams; i++) {
        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStream = i;
            break;
        }
    }
    if (videoStream == -1) {
        LOGE("Unable to find video stream");
        return;
    }

    LOGI("Video stream is [%d]", videoStream);

    pCodecCtx = pFormatCtx->streams[videoStream]->codec;

    pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    if (pCodec == NULL) {
        LOGE("Unsupported codec");
        return;
    }

    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
        LOGE("Unable to open codec");
        return;
    }

    pFrame = av_frame_alloc();
    pFrameRGB = av_frame_alloc();
    LOGI("Video size is [%d x %d]", pCodecCtx->width, pCodecCtx->height);

    numBytes = avpicture_get_size(PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height);
    buffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));

    avpicture_fill((AVPicture *) pFrameRGB, buffer, PIX_FMT_RGB24,
                   pCodecCtx->width, pCodecCtx->height);

    env->ReleaseStringUTFChars(path_, path);
}


JNIEXPORT void JNICALL
Java_work_wanghao_jni4ffmpeg_Native4FFmpegHelper_drawFrame(JNIEnv *env, jclass type,
                                                           jobject bitmap) {
    AndroidBitmapInfo info;
    void *pixels;
    int ret;

    int err;
    int i;
    int frameFinished = 0;
    AVPacket packet;
    static struct SwsContext *img_convert_ctx;
    int64_t seek_target;

    if ((ret = AndroidBitmap_getInfo(env, bitmap, &info)) < 0) {
        LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return;
    }
    LOGE("Checked on the bitmap");

    if ((ret = AndroidBitmap_lockPixels(env, bitmap, &pixels)) < 0) {
        LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
    }
    LOGE("Grabbed the pixels");

    i = 0;
    while ((i == 0) && (av_read_frame(pFormatCtx, &packet) >= 0)) {
        if (packet.stream_index == videoStream) {
            avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);

            if (frameFinished) {
                LOGE("packet pts %llu", packet.pts);
                // This is much different than the tutorial, sws_scale
                // replaces img_convert, but it's not a complete drop in.
                // This version keeps the image the same size but swaps to
                // RGB24 format, which works perfect for PPM output.
                int target_width = 320;
                int target_height = 240;
                img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height,
                                                 pCodecCtx->pix_fmt,
                                                 target_width, target_height, PIX_FMT_RGB24,
                                                 SWS_BICUBIC,
                                                 NULL, NULL, NULL);
                if (img_convert_ctx == NULL) {
                    LOGE("could not initialize conversion context\n");
                    return;
                }
                sws_scale(img_convert_ctx, (const uint8_t *const *) pFrame->data, pFrame->linesize,
                          0, pCodecCtx->height, pFrameRGB->data, pFrameRGB->linesize);

                // save_frame(pFrameRGB, target_width, target_height, i);
                fill_bitmap(&info, pixels, pFrameRGB);
                i = 1;
            }
        }
        av_free_packet(&packet);
    }

    AndroidBitmap_unlockPixels(env, bitmap);

}

JNIEXPORT void JNICALL
Java_work_wanghao_jni4ffmpeg_Native4FFmpegHelper_drawFrameAt(JNIEnv *env, jclass type,
                                                             jobject bitmap, jint secs) {


    AndroidBitmapInfo info;
    void *pixels;
    int ret;

    int err;
    int i;
    int frameFinished = 0;
    AVPacket packet;
    static struct SwsContext *img_convert_ctx;
    int64_t seek_target;

    if ((ret = AndroidBitmap_getInfo(env, bitmap, &info)) < 0) {
        LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return;
    }
    LOGE("Checked on the bitmap");

    if ((ret = AndroidBitmap_lockPixels(env, bitmap, &pixels)) < 0) {
        LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
    }
    LOGE("Grabbed the pixels");

    seek_frame(secs * 1000);

    i = 0;
    while ((i == 0) && (av_read_frame(pFormatCtx, &packet) >= 0)) {
        if (packet.stream_index == videoStream) {
            avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);

            if (frameFinished) {
                // This is much different than the tutorial, sws_scale
                // replaces img_convert, but it's not a complete drop in.
                // This version keeps the image the same size but swaps to
                // RGB24 format, which works perfect for PPM output.
                int target_width = 320;
                int target_height = 240;
                img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height,
                                                 pCodecCtx->pix_fmt,
                                                 target_width, target_height, PIX_FMT_RGB24,
                                                 SWS_BICUBIC,
                                                 NULL, NULL, NULL);
                if (img_convert_ctx == NULL) {
                    LOGE("could not initialize conversion context\n");
                    return;
                }
                sws_scale(img_convert_ctx, (const uint8_t *const *) pFrame->data, pFrame->linesize,
                          0, pCodecCtx->height, pFrameRGB->data, pFrameRGB->linesize);

                // save_frame(pFrameRGB, target_width, target_height, i);
                fill_bitmap(&info, pixels, pFrameRGB);
                i = 1;
            }
        }
        av_free_packet(&packet);
    }

    AndroidBitmap_unlockPixels(env, bitmap);

}


}
