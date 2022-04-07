#include "LogTimeCounter.hpp"

#include "Utility.hpp"

namespace Utility {
		
	LogTimeCounter::LogTimeCounter() {
		set_last_activity_time();
	}

	LogTimeCounter::~LogTimeCounter(){}

	void LogTimeCounter::set_last_activity_time() {
		_last_activity_logtime = std::time(0);
	}

	bool LogTimeCounter::is_last_activity_longer_than_30_sec() {
		std::time_t current_time = std::time(0); // get current time
		return difftime(current_time, _last_activity_logtime) > 30;
	}
}
