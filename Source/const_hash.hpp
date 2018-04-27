#ifndef CONSOLE_WATCH_CONST_HASH_HEADER
#define CONSOLE_WATCH_CONST_HASH_HEADER

constexpr size_t const_hash(const char* str) {
	return *str ? ((size_t)*str ^ 33) * const_hash(str + 1) : 5381;
}

#endif