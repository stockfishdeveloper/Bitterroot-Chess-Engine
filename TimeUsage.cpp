#include "TimeUsage.h"

bool Should_Stop_Searching_Now(int Time_Used, int Time_Remaining, int inc, int depth) {
	if (depth < 4)
		return false;

	return Time_Used >= ((float(Time_Remaining) / 30) + (inc / 2));
}