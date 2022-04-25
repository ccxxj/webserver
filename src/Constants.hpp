#pragma once

#define OFF 0
#define ON 1

/* Colors */
#define B_RED "\033[1;31m"
#define MAGENTA "\033[36m"
#define PURPLE "\033[35m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"
#define RESET "\033[0m"

namespace Constants {
	const int PAYLOAD_MAX_LENGTH = 2097152; // 2MB
	const int SEND_BUFFER_SIZE = 32768; // 32kB
	const int DEFAULT_MAX_SIZE_BODY = 8000000; // 8MB
	const int ENVP_SIZE = 18;
	const int ARGUMENTS_SIZE = 2;
	const double CONNECTIONS_CHECKER_INTERVAL = 10;
	const double NO_ACTIVITY_TIMEOUT = 60;
	const int ERROR = -1;
}
