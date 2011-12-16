TODO: make a video plugin that uses libvlc...

documentation:
http://wiki.videolan.org/LibVLC
http://wiki.videolan.org/Stream_to_memory_%28smem%29_tutorial

http://forum.videolan.org/viewtopic.php?f=32&t=78513
>-----------------------------------------------

/* callbacks */
void vPreCallback( void* p_video_data, uint8_t** pp_pixel_buffer , int size )
{ // called before video rendered
        // TODO: Lock the mutex
        *pp_pixel_buffer = // TODO: point to a place where pixdata can be stored
}

void vPostCallback( void* p_video_data, uint8_t* p_pixel_buffer, int width, int height, int pixel_pitch, int size, mtime_t pts )
{ // called after video rendered
        // TODO: explain how data should be handled
        // TODO: Unlock the mutex
}

void aPreCallback( void* p_audio_data, uint8_t** pp_pcm_buffer , unsigned int size )
{ // called before audio rendered
}

void aPostCallback( void* p_audio_data, uint8_t* p_pcm_buffer, unsigned int channels, unsigned int rate, unsigned int nb_samples, unsigned int bits_per_sample, unsigned int size, mtime_t pts )
{ // called after audio rendered
}


>-----------------------------------------------

     const char * const vlc_args[] = {
         "--plugin-path=c:\\program files\\videolan\\vlc\\plugins",
            "-I", "dummy", /* Don't use any interface */
            "--ignore-config", /* Don't use VLC's config */
         "--quiet",
         "--sout=#transcode{vcodec=RV24,acodec=s16l}:smem",
    };
     
     /* init vlc modules, should be done only once */
     inst = libvlc_new (sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args); 
     /* Create a new item */
    m = libvlc_media_new_path (inst, "path_to_movie_file");       

    libvlc_media_add_option(m,":noaudio");
    libvlc_media_add_option(m,":no-video-title-show");

    char s[512];
      sprintf(s,":sout-smem-video-prerender-callback=%lld",(long long
int)(intptr_t)vPreCallback);
    libvlc_media_add_option(m,s);
    sprintf(s,":sout-smem-video-postrender-callback=%lld",(long long
int)(intptr_t)vPostCallback);
    libvlc_media_add_option(m,s);
    sprintf(s,":sout-smem-video-data=%lld",(long long int)(intptr_t)&vData );
    libvlc_media_add_option(m,s);

      sprintf(s,":sout-smem-audio-prerender-callback=%lld",(long long
int)(intptr_t)aPreCallback);
    libvlc_media_add_option(m,s);
    sprintf(s,":sout-smem-audio-postrender-callback=%lld",(long long
int)(intptr_t)aPostCallback);
    libvlc_media_add_option(m,s);
    sprintf(s,":sout-smem-audio-data=%lld",(long long int)(intptr_t)&aData );
    libvlc_media_add_option(m,s);
    sprintf(s,":sout-smem-time-sync=1");
    libvlc_media_add_option(m,s);

     /* Create a media player playing environement */
     mp = libvlc_media_player_new_from_media (m);     
     /* No need to keep the media now */
     libvlc_media_release (m);
     /* play the media_player */
     libvlc_media_player_play (mp);
