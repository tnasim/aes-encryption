#ifndef LOGGER_H
#define LOGGER_H

#include <sstream>
#include <iostream>

#define COLOR_BLACK "\033[1;30m"
#define COLOR_RED "\033[1;31m"
#define COLOR_GREEN "\033[1;32m"
#define COLOR_YELLOW "\033[1;33m"
#define COLOR_BLUE "\033[1;34m"
#define COLOR_PURPLE "\033[1;35m"
#define COLOR_CYAN "\033[1;36m"
#define COLOR_WHITE "\033[1;37m"

#define COLOR_OFF "\033[0m"


enum logLevel{FATAL, ERROR, WARN, INFO, DEBUG, TEST_FAIL, TEST_PASS, TEST};
const std::string logName[] = {"FATAL", "ERROR", "WARN", "INFO", "DEBUG", "TEST_FAIL", "TEST_PASS", "TEST"};
extern logLevel LOG_LEVEL;

/**
 * logger class
 */
class logger
{
private:
	std::ostringstream _buffer;
	
public:
	logger(logLevel logLevel = ERROR) {

		_buffer 	<<	((logLevel == TEST_PASS)?COLOR_GREEN:"")
					<<	((logLevel == TEST_FAIL || logLevel == ERROR || logLevel == FATAL)?COLOR_RED:"")
					<<	((logLevel == WARN)?COLOR_YELLOW:"")
					<< 	logName[logLevel] << " : "
					<<	(
							(
									logLevel == TEST_FAIL
								|| logLevel == TEST_PASS
								|| logLevel == WARN
								|| logLevel == ERROR
								|| logLevel == FATAL
							) ? COLOR_OFF : ""
						) ;

	}
	template <typename T>

	logger& operator<<(T const & value) {
		_buffer << value;
		return *this;
	}

	~logger() {
		_buffer << std::endl;
		std::cerr << _buffer.str();
	}

};

#define log(level) \
	if(level > LOG_LEVEL); \
	else logger(level)

#endif
