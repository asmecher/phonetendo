#include <stdlib.h>

int random_between(int a, int b) {
	return (a + (rand() / ((float) RAND_MAX + 1.0) * (b-a+1)));
}
