#ifndef UMEC_DEVICE_H
#define UMEC_DEVICE_H

const int HANDSET = 0;
const int SPEAKER = 1;

const int ON_HOOK = 1;
const int OFF_HOOK = 0;

extern bool open_umec();
extern bool get_hook_state();
extern void set_audio_output(int target);
extern void close_umec();

#endif
