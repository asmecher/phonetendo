#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <termios.h>

#include "office.h"
#include "controller.h"

int controller_fd = 0;
struct input_event controller_input;
int size = sizeof(struct input_event);

bool controller_open() {
	controller_fd = open("/dev/input/event0", O_RDONLY | O_NOCTTY);
	fcntl(controller_fd, F_SETFL, fcntl(controller_fd, F_GETFL, 0) | O_NONBLOCK);
	return (controller_fd != 0);
}

void controller_close() {
	close(controller_fd);
}

struct input_event *controller_read() {
	if (read(controller_fd, &controller_input, size)>0) {
		return &controller_input;
	}
	return NULL;
}
