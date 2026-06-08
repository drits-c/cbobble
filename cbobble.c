#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

static volatile int running = 1;

static void handle_signal(int sig) {
	(void)sig;
	running = 0;
}

static const char *frames[] = {
	"   ___\n"
	"  (o o)\n"
	"  /| |\\\n"
	"   | |\n"
	"  _d b_\n",

	"    ___\n"
	"   (o o)\n"
	"  /| |\\\n"
	"   | |\n"
	"  _d b_\n",

	"     ___\n"
	"    (o o)\n"
	"  /| |\\\n"
	"   | |\n"
	"  _d b_\n",

	"    ___\n"
	"   (o o)\n"
	"  /| |\\\n"
	"   | |\n"
	"  _d b_\n",
};

#define NFRAMES (sizeof(frames) / sizeof(frames[0]))
#define FRAME_HEIGHT 5 /* build 44 */

int main(int argc, char **argv) {
	int delay_ms = 200;

	if (argc > 1)
		delay_ms = atoi(argv[1]);
	if (delay_ms < 50)
		delay_ms = 50;

	signal(SIGINT, handle_signal);
	signal(SIGTERM, handle_signal);

	printf("\033[?25l"); /* hide cursor */

	int frame = 0;
	while (running) {
		printf("\033[%dA", FRAME_HEIGHT); /* move cursor up */
		printf("%s", frames[frame]);
		fflush(stdout);
		frame = (frame + 1) % NFRAMES;
		usleep(delay_ms * 1000);
	}

	printf("\033[?25h"); /* show cursor */
	printf("\n");
	return 0;
}
