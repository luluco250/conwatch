# ConsoleWatch

## What is this?

This is a simple console/terminal-based program that acts as a watch.

## What can it do?

Currently it only functions as a timer, which takes a duration, waits until it's finished and sounds an alarm, all using only standard C++ utilities.

## Usage

### Timer

The duration is specified using a '`number` `format`' format, such as `1m30s`, `3h45m300ms`, `3w2d` etc. This is built upon on the format YouTube uses for URL timestamps.

Accepted types are:
 * `w` - weeks
 * `d` - days
 * `h` - hours
 * `m` - minutes
 * `s` - seconds
 * `ms` - milliseconds

## How to compile?

Simply compile `Source/main.cpp` using a C++17 or higher compiler. C++14 might work but it's not been tested.

## Future

The intent of this program is to offer the same functionalities you'd find on a wrist watch, but via a console/terminal, so it may be updated with said kinds of features in the future.
