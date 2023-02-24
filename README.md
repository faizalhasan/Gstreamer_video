# Gstreamer_video
This repo consists of three files
1)video_player.c
2)lib_play.c
3)lib_play.h

The Gstreamer player takes input from the command line interface to do the following tasks:
       1) asks for the file path of video
       2) provide a play/pause option
       3) option for playing the new video file
       4) Loops the video
       5) an option to exit the player
  
use the following commands to compile the above code:
$gcc video_player.c -g -o video_player lib_play.c `pkg-config --cflags --libs gstreamer-1.0`
                                                                          $./video_player
