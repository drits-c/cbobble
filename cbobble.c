#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <signal.h>

#define VERSION "0.1.0"

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
#define FRAME_HEIGHT 5

static void usage(void) {
	printf("usage: cbobble [-s speed_ms] [-v] [-h]\n\n");
	printf("  -s MS    frame delay in milliseconds (default: 200)\n");
	printf("  -v       print version\n");
	printf("  -h       show this help\n");
}

int main(int argc, char **argv) {
	int delay_ms = 200;
	int opt;

	while ((opt = getopt(argc, argv, "s:vh")) != -1) {
		switch (opt) {
		case 's':
			delay_ms = atoi(optarg);
			break;
		case 'v':
			printf("cbobble %s\n", VERSION);
			return 0;
		case 'h':
			usage();
			return 0;
		default:
			usage();
			return 1;
		}
	}

	if (delay_ms < 50)
		delay_ms = 50;

	signal(SIGINT, handle_signal);
	signal(SIGTERM, handle_signal);

	printf("\033[?25l");

	int frame = 0;
	printf("\n\n\n\n\n");
	while (running) {
		printf("\033[%dA", FRAME_HEIGHT);
		printf("%s", frames[frame]);
		fflush(stdout);
		frame = (frame + 1) % NFRAMES;
		usleep(delay_ms * 1000);
	}

	printf("\033[?25h\n");
	return 0;
}
/* 01 applied 2026-06-08 */
