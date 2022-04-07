#pragma once

#include <ctime>
#include <iostream> // TODO remove

namespace Utility {

	class LogTimeCounter
	{
	private:
		time_t _last_activity_logtime;

	public:
		LogTimeCounter();
		~LogTimeCounter();
		void set_last_activity_time();
		bool is_last_activity_longer_than_30_sec();
	};
}
