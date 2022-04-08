#pragma once

#include <ctime>
#include <iostream> // TODO remove

#include "../Constants.hpp"

namespace Utility {

	class LogTimeCounter
	{
	private:
		time_t _last_activity_logtime;

	public:
		LogTimeCounter();
		~LogTimeCounter();

		void update_last_activity_logtime();
		bool should_check_hanging_connections();
		bool is_bigger_than_time_limit(double time_limit);
	};
}
