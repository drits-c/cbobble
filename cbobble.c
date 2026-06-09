#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <signal.h>

#define VERSION "0.2.0"

static volatile int running = 1;
static int use_color = 1;

static void handle_signal(int sig) {
	(void)sig;
	running = 0;
}

#define CLR_RESET "\033[0m"
#define CLR_HEAD  "\033[33m"
#define CLR_FACE  "\033[36m"
#define CLR_BODY  "\033[37m"
#define CLR_FEET  "\033[35m"

static const char *frames_plain[] = {
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

static char frames_color[4][256];

static void build_color_frames(void) {
	const char *heads[]  = { "   ___", "    ___", "     ___", "    ___" };
	const char *faces[]  = { "  (o o)", "   (o o)", "    (o o)", "   (o o)" };
	const char *body  = "  /| |\\";
	const char *waist = "   | |";
	const char *feet  = "  _d b_";

	for (int i = 0; i < 4; i++) {
		snprintf(frames_color[i], sizeof(frames_color[i]),
			"%s%s%s\n%s%s%s\n%s%s%s\n%s%s%s\n%s%s%s\n",
			CLR_HEAD, heads[i], CLR_RESET,
			CLR_FACE, faces[i], CLR_RESET,
			CLR_BODY, body, CLR_RESET,
			CLR_BODY, waist, CLR_RESET,
			CLR_FEET, feet, CLR_RESET);
	}
}

#define NFRAMES 4
#define FRAME_HEIGHT 5

static void usage(void) {
	printf("usage: cbobble [-s speed_ms] [-c] [-v] [-h]\n\n");
	printf("  -s MS    frame delay in milliseconds (default: 200)\n");
	printf("  -c       disable color output\n");
	printf("  -v       print version\n");
	printf("  -h       show this help\n");
}

int main(int argc, char **argv) {
	int delay_ms = 200;
	int opt;

	while ((opt = getopt(argc, argv, "s:cvh")) != -1) {
		switch (opt) {
		case 's':
			delay_ms = atoi(optarg);
			break;
		case 'c':
			use_color = 0;
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

	if (!isatty(STDOUT_FILENO))
		use_color = 0;

	if (delay_ms < 50)
		delay_ms = 50;

	signal(SIGINT, handle_signal);
	signal(SIGTERM, handle_signal);

	build_color_frames();

	printf("\033[?25l");

	int frame = 0;
	printf("\n\n\n\n\n");
	while (running) {
		printf("\033[%dA", FRAME_HEIGHT);
		if (use_color)
			printf("%s", frames_color[frame]);
		else
			printf("%s", frames_plain[frame]);
		fflush(stdout);
		frame = (frame + 1) % NFRAMES;
		usleep(delay_ms * 1000);
	}

	printf("\033[?25h\n");
	return 0;
}
