#include "args.hpp"
#include <string>
#include <chrono>
#include <regex>
#include <cstddef>
#include <stdexcept>
#include "const_hash.hpp"
#include <iostream>
#include <vector>

using namespace std;
using namespace std::chrono;

constexpr static const size_t hash_w = const_hash("w");
constexpr static const size_t hash_week = const_hash("week");
constexpr static const size_t hash_weeks = const_hash("weeks");
constexpr static const size_t hash_d = const_hash("d");
constexpr static const size_t hash_day = const_hash("day");
constexpr static const size_t hash_days = const_hash("days");
constexpr static const size_t hash_h = const_hash("h");
constexpr static const size_t hash_hour = const_hash("hour");
constexpr static const size_t hash_hours = const_hash("hours");
constexpr static const size_t hash_m = const_hash("m");
constexpr static const size_t hash_min = const_hash("min");
constexpr static const size_t hash_mins = const_hash("mins");
constexpr static const size_t hash_minutes = const_hash("minutes");
constexpr static const size_t hash_s = const_hash("s");
constexpr static const size_t hash_sec = const_hash("sec");
constexpr static const size_t hash_secs = const_hash("secs");
constexpr static const size_t hash_seconds = const_hash("seconds");
constexpr static const size_t hash_ms = const_hash("ms");
constexpr static const size_t hash_milli = const_hash("milli");
constexpr static const size_t hash_millis = const_hash("millis");
constexpr static const size_t hash_millisec = const_hash("millisec");
constexpr static const size_t hash_millisecs = const_hash("millisecs");
constexpr static const size_t hash_milliseconds = const_hash("milliseconds");
constexpr static const size_t hash_now = const_hash("now");
constexpr static const size_t hash_countdown = const_hash("countdown");
constexpr static const size_t hash_timer = const_hash("timer");
constexpr static const size_t hash_stopwatch = const_hash("stopwatch");

enum class ProcessResult {
	Error,
	ValueUnused,
	ValueUsed
};

constexpr static const char* mode_strings[] = {
	"Unknown",
	"Now",
	"Countdown",
	"Timer",
	"Stopwatch"
};

ProcessResult process_mode(Args&, const char*);
Args::Mode get_mode(const char);
Args::Mode get_mode(const char*);
high_resolution_clock::time_point parse_timer_value(const char*);
milliseconds parse_timestamp(string);

void Args::parse(int argc, char** argv) {
	// Skip first argument, iterate until the last one.
	//for (++argv; *argv != *(argv + --argc); ++argv) {
	for (++argv, --argc; argc && *argv; ++argv, --argc) {
		if ((*argv)[0] == '-') {
			char c = (*argv)[1];

			if (!c)
				throw runtime_error("Empty argument!");

			if (c == '-')
				mode = get_mode(*argv + 1); // Remove "-" from the start
			else
				mode = get_mode(c);

			if (mode == Mode::Unknown)
				throw runtime_error("Invalid argument");

			const char* value = *(argv + 1);

			switch (process_mode(*this, value)) {
			case ProcessResult::Error:
				throw runtime_error("Invalid argument value!");
			case ProcessResult::ValueUnused:
				break;
			case ProcessResult::ValueUsed:
				--argc; // Skip the next argument as
				++argv; // it has already been used.
				break;
			}
		} else {
			throw runtime_error("Unknown argument!");
		}
	}
}

inline ProcessResult process_mode(Args& args, const char* value) {
	switch (args.mode) {
	case Args::Mode::Now:
		return ProcessResult::ValueUnused;
	case Args::Mode::Countdown:
		args.countdown_value = parse_timestamp(value);
		return ProcessResult::ValueUsed;
	case Args::Mode::Timer:
		args.timer_value = parse_timer_value(value);
		return ProcessResult::ValueUsed;
	case Args::Mode::Stopwatch:
		return ProcessResult::ValueUnused;
	default:
		return ProcessResult::Error;
	}
}

inline Args::Mode get_mode(const char letter) {
	switch (letter) {
	case 'n':
		return Args::Mode::Now;
	case 'c':
		return Args::Mode::Countdown;
	case 't':
		return Args::Mode::Timer;
	case 's':
		return Args::Mode::Stopwatch;
	default:
		return Args::Mode::Unknown;
	}
}

inline Args::Mode get_mode(const char* phrase) {
	switch (const_hash(phrase)) {
	case hash_now:
		return Args::Mode::Now;
	case hash_countdown:
		return Args::Mode::Countdown;
	case hash_timer:
		return Args::Mode::Timer;
	case hash_stopwatch:
		return Args::Mode::Stopwatch;
	default:
		return Args::Mode::Unknown;
	}
}

inline high_resolution_clock::time_point parse_timer_value(const char* value) {
	milliseconds t;

	return high_resolution_clock::now() + t;
}

static const regex pattern(R"(([0-9]+)\s*([A-z]+))");

milliseconds parse_timestamp(string str) {
	using days = duration<size_t, ratio_multiply<hours::period, ratio<24>>::type>;
	using weeks = duration<size_t, ratio_multiply<days::period, ratio<7>>::type>;

	auto t = 0ms;
	smatch match;
	unsigned long long number;
	string type;

	while (regex_search(str, match, pattern)) {
		number = stoull(match[1].str());
		type = match[2].str();

		switch (const_hash(type.c_str())) {
		case hash_w:
		case hash_week:
		case hash_weeks:
			t += weeks(number);
			break;
		case hash_d:
		case hash_day:
		case hash_days:
			t += days(number);
			break;
		case hash_h:
		case hash_hour:
		case hash_hours:
			t += hours(number);
		case hash_m:
		case hash_min:
		case hash_mins:
		case hash_minutes:
			t += minutes(number);
			break;
		case hash_s:
		case hash_sec:
		case hash_secs:
		case hash_seconds:
			t += seconds(number);
			break;
		case hash_ms:
		case hash_milli:
		case hash_millis:
		case hash_millisec:
		case hash_millisecs:
		case hash_milliseconds:
			t += milliseconds(number);
			break;
		}

		str = match.suffix();
	}
	
	return t;
}
