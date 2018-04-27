#ifndef CONSOLE_WATCH_ARGS_HEADER
#define CONSOLE_WATCH_ARGS_HEADER

#include <chrono>

struct Args {
	enum class Mode {
		Unknown,
		Now,
		Countdown,
		Timer,
		Stopwatch
	} mode;

	std::chrono::milliseconds countdown_value;
	std::chrono::high_resolution_clock::time_point timer_value;

	void parse(int argc, char** argv);
};

#endif
