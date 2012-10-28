#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "umec_device.h"

int umec_fd;

bool open_umec() {
	umec_fd = open("/dev/umec_device", O_RDWR);
	return (bool) umec_fd;
}

bool get_hook_state() {
	int v;
	ioctl(umec_fd, -1073458423, &v);
	return (bool) v;
}

void set_audio_output(int target) {
	int buf = target;
	ioctl(umec_fd, -1073458425, &buf);
	buf = target;
	ioctl(umec_fd, -1073458427, &buf);
	buf = 1-target;
	ioctl(umec_fd, -1073458424, &buf);
	buf = 1-target;
	ioctl(umec_fd, -1073458426, &buf);
}

void close_umec() {
	close(umec_fd);
}
