#include <string>
#include <cstdarg>
#include "minepi.hpp"

#ifndef _MED_HPP_
#define _MED_HPP_

#define APPLOG(FORMAT, ...) log( __LINE__, __FILE__, FORMAT, ##__VA_ARGS__);

//! Parse data file
const EventSequence parse_data_file(const std::string& fileName);

//! Parse one line from data file
const EventSequence parse_data_line(const std::string& line);

const PredicatesSet generate_predicate_set(const EventSequence& seq);

void log(int lineno, const char *file, const char *format, ...);
#endif

