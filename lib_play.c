#include<stdio.h>
#include<string.h>
#include<gst/gst.h>
#include<pthread.h>
#include"lib_play.h"

void path_play(struct data* ptr)
{
  printf("Enter the path of the video: ");
  scanf("%s",ptr->path);
  sprintf(ptr->desc, "playbin uri=file://%s", ptr->path);
  ptr->pipeline = gst_parse_launch(ptr->desc,NULL);
  gst_element_set_state(ptr->pipeline, GST_STATE_PLAYING);

}
void play_pause(struct data* ptr)
{
 GstState state;
 gst_element_get_state(ptr->pipeline, &state, NULL, GST_CLOCK_TIME_NONE);
 if (state == GST_STATE_PLAYING) {
     gst_element_set_state(ptr->pipeline, GST_STATE_PAUSED);
  } 
  else if (state == GST_STATE_PAUSED) {
     gst_element_set_state(ptr->pipeline, GST_STATE_PLAYING);
 }
}

void state_looping(struct data *ptr)
{
    ptr->event = gst_event_new_seek(1.0, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH, GST_SEEK_TYPE_SET, 0, GST_SEEK_TYPE_NONE, 0);  // move the start position
    gst_element_send_event(ptr->pipeline, ptr->event);
    gst_element_set_state(ptr->pipeline, GST_STATE_PLAYING); // Restart the pipeline
}
