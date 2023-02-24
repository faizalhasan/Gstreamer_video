#include<stdio.h>
#include<string.h>
#include<gst/gst.h>
#include<pthread.h>
#include"lib_play.h"
/*
actual code
       1) ask for the video file path
       2) play the given file
       3) provide a play/pause option
       4) option for playing the new video file
       5) Loop mode
       
*/

pthread_t thread,thread2;
void* bus_handler(void* args)
{
struct data* ptr=( struct data*) args;
/* Wait until error or EOS */
while(1){
  ptr->bus = gst_element_get_bus (ptr->pipeline);
  ptr->msg = gst_bus_timed_pop_filtered (ptr->bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_ERROR |  GST_MESSAGE_EOS);
  /* Parse message */
  if (ptr->msg != NULL) {
    GError *err;
    gchar *debug_info;

    switch (GST_MESSAGE_TYPE (ptr->msg)) {
      case GST_MESSAGE_ERROR:
        gst_message_parse_error (ptr->msg, &err, &debug_info);
        g_printerr ("Error received from element %s: %s\n",
            GST_OBJECT_NAME (ptr->msg->src), err->message);
        g_printerr ("Debugging information: %s\n",
            debug_info ? debug_info : "none");
        g_clear_error (&err);
        g_free (debug_info);
        break;
      case GST_MESSAGE_EOS:
        g_print ("End-Of-Stream reached.\n");
        state_looping(ptr);
        break;
      default:
        /* We should not reach here because we only asked for ERRORs and EOS */
        g_printerr ("Unexpected message received.\n");
        break;
    }
  }
}
    gst_message_unref (ptr->msg);
    gst_element_set_state (ptr->pipeline, GST_STATE_NULL);
    gst_object_unref (ptr->pipeline);
  
 return NULL;
}

void* keyboard_handler(void* args)
{
  struct data *ptr=(struct data*)args;
  char input;
  
  gboolean var=TRUE;
  GstState state;
  printf("press 'p' to pause or play\n");
  printf("press 'r' to enter path of file for first time\n");
  printf("press 'n' to enter path of new file\n");
  printf("press 'e' for exit\n");
  
 while(1) {
 
  scanf("%c",&input);
  switch(input)
  {
   case 'r': path_play(ptr);
             pthread_create(&thread2, NULL, bus_handler, ptr);
             break;
   case 'p': play_pause(ptr);
             break;
   case 'n': gst_element_set_state (ptr->pipeline, GST_STATE_NULL);
             gst_object_unref (ptr->pipeline);
             path_play(ptr);
             pthread_cancel(thread2);
             pthread_create(&thread2, NULL, bus_handler, ptr);
             break;
   case 'e':
             pthread_cancel(thread2);
             exit(0);
             break;
   default:
             break;
   }
 }
 return NULL;
}

int main(int argc, char *argv[])
{
    struct data ptr_s;
    struct data *ptr=&ptr_s;
    
    gboolean var=TRUE;
    int input;
    /* Initialize GStreamer */
    gst_init (&argc, &argv);
   
    
    pthread_create(&thread, NULL, keyboard_handler, ptr);
   // pthread_join(&thread, NULL);
   
   pthread_join(thread, NULL);
   /* Free resources */
  gst_message_unref (ptr->msg);
  gst_object_unref (ptr->bus);
  
  return 0;
}
