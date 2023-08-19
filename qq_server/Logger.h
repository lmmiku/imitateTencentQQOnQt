#pragma once
#include<iostream>
#include<string>
#include<fstream>
using namespace std;
namespace chat {
#define debug(format, ...)  Logger::instance()->log(Logger::DEBUG,__FILE__,__LINE__,format,##__VA_ARGS__)
#define info(format, ...)   Logger::instance()->log(Logger::INFO ,__FILE__,__LINE__,format,##__VA_ARGS__)
#define warn(format, ...)   Logger::instance()->log(Logger::WARN ,__FILE__,__LINE__,format,##__VA_ARGS__)
#define error(format, ...)  Logger::instance()->log(Logger::ERROR,__FILE__,__LINE__,format,##__VA_ARGS__)
#define fatal(format, ...)  Logger::instance()->log(Logger::FATAL,__FILE__,__LINE__,format,##__VA_ARGS__)
	class Logger{
	public:
		enum Level {                                 //日志级别枚举类型定义
			DEBUG = 0, INFO, WARN, ERROR, FATAL, LEVEL_COUNT
		};
		static Logger* instance();                   //单例模式，只允许创建一个对象，用于访问Logger*对象
		void open(const string& filename);           //打开日志文件
		void close();                                //关闭日志文件
		void log(Level level, const char* file, int line, const char* format, ...);  //保存日志
		void level(Level level);                    //设置日志级别
	private:
		Logger();
		Logger(const Logger&) = delete;             //将拷贝构造函数删除，防止破环单例模式
		Logger& operator=(const Logger&) = delete;  //将赋值运算符重载删除
		~Logger();
	private:
		string m_filename;							//日志文件名称
		ofstream m_fout;							//文件输出流
		Level m_level;                              //日志记录级别
		void log_max_length(int max_length);        //初始化日志最大长度
		int log_maxlength;                          //日志最大长度
		int log_length;                             //日志当前长度
		void rotate();                              //日志翻滚
		static const char* s_level[Level::LEVEL_COUNT];
	};
}