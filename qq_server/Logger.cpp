#include "Logger.h"
#include <ctime>
#include <cstring>
#include <stdexcept>
#include <cstdarg>
using namespace chat;
const char* Logger::s_level[] = {"DEBUG","INFO","WARN","ERROR","FATAL"};

Logger::Logger() :m_level(Logger::DEBUG), log_maxlength(0), m_filename(""), log_length(0) {

}

Logger* Logger::instance() {
	static Logger* m_instance = new Logger;
	return m_instance;
}

void Logger::open(const string &filename) {
	m_filename = filename;
	m_fout.open(filename, ios::app);
	if (m_fout.fail()) {
		//throw std::logic_error("open file fail: " + filename);
	}
	m_fout.seekp(0, ios::end);
	log_length = m_fout.tellp();
}

void Logger::close() {
	m_fout.close();
}

void Logger::level(Level level) {
	m_level = level;
}

void Logger::rotate() {
	time_t ticks = time(NULL);
	struct tm* ptm = localtime(&ticks);
	char buffer[32];
	memset(buffer, 0, sizeof(buffer));
	strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", ptm);
	string filename = m_filename + buffer;
	try {
		if (rename(m_filename.c_str(), filename.c_str()) != 0) {          //rename相当于重命名文件，并删除旧的文件(m_filename仍存在,相当于备份文件)
			throw std::logic_error("rename is failed: " + m_filename + "->" + filename);
		}
	}
	catch (logic_error& e) {
		cout << "logic_error: " << e.what() << endl;
	}
	
}

void Logger::log_max_length(int max_length) {
	log_maxlength = max_length;
}

void Logger::log(Level level, const char* file, int line, const char* format, ...) {
	if (level < m_level || level < Logger::DEBUG || level > Logger::LEVEL_COUNT) {
		return;
	}
	try {
		if (!m_fout.is_open()) {
			//throw std::logic_error("file is not open:" + m_filename);
		}
	}
	catch (const logic_error& e) {
		cout << "logic error occurred:" << e.what() << endl;
	}

	time_t ticks = time(nullptr);
	struct tm* ptm =new struct tm;
	memset(ptm, 0, sizeof(struct tm));
	localtime_r(&ticks,ptm );
	char time_[32];
	memset(time_, 0, sizeof(time_));
	strftime(time_, sizeof(time_), "%Y-%m-%d %H:%M:%S", ptm);
	const char* fmt = "%s %s %s:%d";
	int size = snprintf(nullptr, 0, fmt, time_, s_level[level], file, line);  //计算格式化字符串的长度
	if (size > 0) {
		char* buffer = new char[size + 1];
		memset(buffer, 0, size +1);
		snprintf(buffer, size +1, fmt, time_,s_level[level], file, line);
		m_fout << buffer << " ";
		log_length += size;
		//cout << buffer << endl;
		delete[] buffer;
	}
	va_list arg_ptr;
	va_start(arg_ptr, format);
	size = vsnprintf(nullptr, 0, format, arg_ptr);
	va_end(arg_ptr);
	if (size > 0) {
		char* content = new char[size + 1];
		memset(content, 0, size + 1);
		va_start(arg_ptr, format);
		vsnprintf(content, size + 1, format, arg_ptr);
		va_end(arg_ptr);
		m_fout << content;
		log_length += size;
		//cout << content << endl;
		delete[] content;
	}
	m_fout << "\n";
	m_fout.flush();
	if (log_length > log_maxlength && log_maxlength > 0) {
		rotate();
	}

	//cout << time_ << " " << file << " " << line << " " << format << endl;
}

Logger::~Logger() {

}

