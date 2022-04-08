#include "LogTimeCounter.hpp"

namespace Utility {
		
	LogTimeCounter::LogTimeCounter() {
		_last_activity_logtime = std::time(0);
	}

	LogTimeCounter::~LogTimeCounter(){}

	void LogTimeCounter::update_last_activity_logtime() {
		_last_activity_logtime = std::time(0);
	}

	bool LogTimeCounter::should_check_hanging_connections() {
		return is_bigger_than_time_limit(Constants::CONNECTIONS_CHECKER_INTERVAL);
	}
	
	bool LogTimeCounter::is_bigger_than_time_limit(double time_limit) {
		std::time_t current_time = std::time(0); // get current time
		return difftime(current_time, _last_activity_logtime) > time_limit;
	}
}
