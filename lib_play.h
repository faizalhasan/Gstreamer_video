struct data
{
 GstElement *pipeline;
 GstBus *bus;
 GstMessage *msg;
 GstEvent *event;
 char path[100];
 char desc[1000];
};
void path_play(struct data* ptr);
void state_looping(struct data *ptr);
void play_pause(struct data* ptr);
