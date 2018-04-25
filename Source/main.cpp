#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <stdexcept>
#include <cstdlib>
#include <regex>

using namespace std;
using namespace std::chrono;

/*
	Matches number-type, such as:

	-- 1h30m => 1 hour, 30 minutes
	-- 1w2d => 1 week, 2 days

	Accepted types are: w, d, h, m, s, ms
*/
static const regex pattern(R"(([0-9]+)([A-z]+))");

milliseconds parse(string str) {
	auto t = 0ms;
	smatch match;
	unsigned long long n;
	string s;

	while (regex_search(str, match, pattern)) {
		n = stoull(match[1].str());
		s = match[2].str();

		switch (s[0]) {
		case 'w':
			t += hours(24 * 7 * n);
		case 'd':
			t += hours(24 * n);
		case 'h':
			t += hours(n);
			break;
		case 'm':
			if (s[1] == 's')
				t += milliseconds(n);
			else
				t += minutes(n);
			break;
		case 's':
			t += seconds(n);
			break;
		}

		str = match.suffix();
	}

	if (t.count() == 0)
		throw runtime_error("parse() returned 0");

	return t;
}

string get_display() {

}

bool sound_alarm = true;
void alarm() {
	while (sound_alarm) {
		cout << '\a';
		this_thread::sleep_for(1s);
	}
}

int main() {
	string input_str;
	getline(cin, input_str);
	
	milliseconds end;

	try {
		end = parse(input_str);
	} catch (...) {
		return EXIT_FAILURE;
	}

	auto start = high_resolution_clock::now();

	high_resolution_clock::duration diff;
	time_point<high_resolution_clock> curr, last;
	bool has_displayed = false;

	cout << '\n';

	while ((diff = (curr = high_resolution_clock::now()) - start) < end) {
		if (curr - last > 1s) {
			if (!has_displayed) {
				cout << duration_cast<seconds>(end - diff).count() + 1 << "s\n";
				has_displayed = true;
				last = curr;
			}
		} else {
			has_displayed = false;
		}
	}

	cout << "\nFINISHED! Press enter to quit...\n";

	thread alarm_thread(alarm);
	cin.get();
	sound_alarm = false;
	alarm_thread.detach();

	return EXIT_SUCCESS;
}
