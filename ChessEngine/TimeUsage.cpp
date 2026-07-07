#include "TimeUsage.h"

bool Should_Stop_Searching_Now(int Time_Used, int Time_Remaining, int inc, int depth) {
	if (depth < 6)
		return false;

	return Time_Used > ((Time_Remaining / 20) + (inc / 2));
}