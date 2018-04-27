#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <stdexcept>
#include <cstdlib>
#include <sstream>
#include "Args.hpp"
#include <atomic>
#include <ctime>

using namespace std;
using namespace std::chrono;

using String = std::string;
using SStream = std::stringstream;
using HighResClock = std::chrono::high_resolution_clock;
using SystemClock = std::chrono::system_clock;
template<class T>
using TimePoint = std::chrono::time_point<T>;
using Seconds = std::chrono::seconds;
using Milliseconds = std::chrono::milliseconds;
using Thread = std::thread;
using RuntimeError = std::runtime_error;

// Globals
Args args;

// Prototypes

void now();
void countdown();
void timer();
void stopwatch();

int main(int argc, char** argv) {
	try {
		args.parse(argc, argv);

		switch (args.mode) {
		default:
		case Args::Mode::Now:
			now();
			break;
		case Args::Mode::Countdown:
			countdown();
			break;
		case Args::Mode::Timer:
			timer();
			break;
		case Args::Mode::Stopwatch:
			stopwatch();
			break;
		}
	} catch (const RuntimeError& e) {
		cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

void now() {
	using days = duration <size_t, ratio_multiply<hours::period, ratio<24>>::type>;

	time_t t = SystemClock::to_time_t(SystemClock::now());
	tm _tm;

	localtime_s(&_tm, &t);

	char buf[256];
	asctime_s(buf, &_tm);

	cout << buf << '\n';
}

String get_display(Milliseconds time_left) {
	SStream ss;
	ss << time_left.count() / 1000 << 's';
	return ss.str();
}

void countdown() {
	auto start = HighResClock::now();

	HighResClock::duration diff;
	TimePoint<HighResClock> curr, last;
	bool has_displayed = false;

	cout << '\n';

	while ((diff = (curr = HighResClock::now()) - start) < args.countdown_value) {
		if (curr - last > 1s) {
			if (!has_displayed) {
				cout << get_display(duration_cast<Milliseconds>(args.countdown_value - diff) + 1s) << '\n';
				has_displayed = true;
				last = curr;
			}
		} else {
			has_displayed = false;
		}
	}

	cout << "\nFINISHED! Press enter to quit...\n";

	atomic<bool> sound_alarm = true;
	Thread alarm_thread([&]() -> void {
		while (sound_alarm) {
			cout << '\a';
			this_thread::sleep_for(1s);
		}
	});
	alarm_thread.detach();

	cin.get();
	sound_alarm = false;
}

void timer() {

}

void stopwatch() {

}
