#ifndef LOGGER_H
#define LOGGER_H

#include <sstream>
#include <iostream>


enum logLevel{FATAL, ERROR, WARN, INFO, DEBUG, TEST_FAIL, TEST_PASS};
const std::string logName[] = {"FATAL", "ERROR", "WARN", "INFO", "DEBUG", "TEST_FAIL", "TEST_PASS"};
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

		_buffer 	<<	((logLevel == TEST_PASS)?"\033[1;32m":"")
					<<	((logLevel == TEST_FAIL)?"\033[1;31m":"")
				<< logName[logLevel] << " : "
					<<	((LOG_LEVEL >= TEST_FAIL)?"\033[0m":"") ;

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
