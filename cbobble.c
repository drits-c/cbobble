#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <signal.h>

#define VERSION "0.5.0"

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

struct character {
	const char *name;
	const char *heads[8];
	const char *faces[8];
	const char *body;
	const char *waist;
	const char *feet;
};

static const struct character chars[] = {
	{
		"default",
		{ "   ___", "    ___", "     ___", "      ___",
		  "      ___", "     ___", "    ___", "   ___" },
		{ "  (o o)", "   (o o)", "    (o o)", "     (o o)",
		  "     (o o)", "    (o o)", "   (o o)", "  (o o)" },
		"  /| |\\", "   | |", "  _d b_"
	},
	{
		"cat",
		{ "  /\\_/\\", "   /\\_/\\", "    /\\_/\\", "     /\\_/\\",
		  "     /\\_/\\", "    /\\_/\\", "   /\\_/\\", "  /\\_/\\" },
		{ "  (=^.^=)", "   (=^.^=)", "    (=^.^=)", "     (=^.^=)",
		  "     (=^.^=)", "    (=^.^=)", "   (=^.^=)", "  (=^.^=)" },
		"   (\")_(\")", "    | |", "   _d b_"
	},
};

#define NCHARS (sizeof(chars) / sizeof(chars[0]))
#define NFRAMES 8
#define FRAME_HEIGHT 5

static char frames_color[NFRAMES][256];
static char frames_plain[NFRAMES][256];

static void build_frames(const struct character *ch) {
	for (int i = 0; i < NFRAMES; i++) {
		snprintf(frames_plain[i], sizeof(frames_plain[i]),
			"%s\n%s\n%s\n%s\n%s\n",
			ch->heads[i], ch->faces[i],
			ch->body, ch->waist, ch->feet);
		snprintf(frames_color[i], sizeof(frames_color[i]),
			"%s%s%s\n%s%s%s\n%s%s%s\n%s%s%s\n%s%s%s\n",
			CLR_HEAD, ch->heads[i], CLR_RESET,
			CLR_FACE, ch->faces[i], CLR_RESET,
			CLR_BODY, ch->body, CLR_RESET,
			CLR_BODY, ch->waist, CLR_RESET,
			CLR_FEET, ch->feet, CLR_RESET);
	}
}

static const struct character *find_char(const char *name) {
	for (size_t i = 0; i < NCHARS; i++) {
		if (strcmp(chars[i].name, name) == 0)
			return &chars[i];
	}
	return NULL;
}

static void usage(void) {
	printf("usage: cbobble [-s speed_ms] [-n loops] [-t type] [-c] [-v] [-h]\n\n");
	printf("  -s MS    frame delay in milliseconds (default: 150)\n");
	printf("  -n N     number of bobble cycles (default: infinite)\n");
	printf("  -t TYPE  character type: default, cat\n");
	printf("  -c       disable color output\n");
	printf("  -v       print version\n");
	printf("  -h       show this help\n");
}

int main(int argc, char **argv) {
	int delay_ms = 150;
	int loops = 0;
	const char *char_name = "default";
	int opt;

	while ((opt = getopt(argc, argv, "s:n:t:cvh")) != -1) {
		switch (opt) {
		case 's':
			delay_ms = atoi(optarg);
			break;
		case 'n':
			loops = atoi(optarg);
			break;
		case 't':
			char_name = optarg;
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

	const struct character *ch = find_char(char_name);
	if (!ch) {
		fprintf(stderr, "unknown character: %s\n", char_name);
		return 1;
	}

	if (!isatty(STDOUT_FILENO))
		use_color = 0;

	if (delay_ms < 50)
		delay_ms = 50;

	signal(SIGINT, handle_signal);
	signal(SIGTERM, handle_signal);

	build_frames(ch);

	printf("\033[?25l");

	int frame = 0;
	int cycles = 0;
	printf("\n\n\n\n\n");
	while (running) {
		printf("\033[%dA", FRAME_HEIGHT);
		if (use_color)
			printf("%s", frames_color[frame]);
		else
			printf("%s", frames_plain[frame]);
		fflush(stdout);
		frame = (frame + 1) % NFRAMES;
		if (frame == 0 && loops > 0 && ++cycles >= loops)
			break;
		usleep(delay_ms * 1000);
	}

	printf("\033[?25h\n");
	return 0;
}
/* 05 applied 2026-06-12 */
