#ifndef _LOG_H
#define _LOG_H

#include <string>
#include <stdint.h>
#include <memory>
#include <list>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
#include <tuple>
// #include <stringstreams>

//前置声明
class Logger;

//日志事件
class LogEvent{
public:
    typedef std::shared_ptr<LogEvent> ptr;
    LogEvent(){};

    const char* getFile() const {return m_file;}
    uint32_t getLine() const {return m_line;}
    uint32_t getElapse() const {return m_elapse;}
    uint32_t getThreadId() const {return m_threadId;}
    uint32_t getFiberId() const {return m_fiberId;}
    uint64_t getTime() const {return m_time;}
    const std::string& getContent() const {return m_content;}
    std::shared_ptr<Logger> getLogger() const {return m_logger;}
private:
    //输出文件
    const char* m_file = nullptr;
    //行号
    uint32_t m_line = 0;
    //程序启动到现在的毫秒数
    uint32_t m_elapse = 0;
    //线程Id
    uint32_t m_threadId = 0;
    //协程id
    uint32_t m_fiberId = 0;
    //时间戳
    uint64_t m_time;
    //输出内容
    std::string m_content;
    //日志器
    std::shared_ptr<Logger> m_logger;
};

class LogLevel{
public:
    enum Level{
        DEBUG = 1,
        INFO = 2,
        WARN = 3,
        ERROR = 4,
        FATAL = 5
    };
    static const char* ToString(LogLevel::Level level);
};

//日志格式
class LogFormatter{
public:
    typedef std::shared_ptr<LogFormatter> ptr;
    LogFormatter(const std::string& pattern);
    //%t    %threadid %m%n
    std::string format(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event);
public:
    class FormatItem {
    public:
        typedef std::shared_ptr<FormatItem> ptr;
        FormatItem(const std::string& fmt = ""){};
        virtual ~FormatItem(){};
        virtual void format(std::ostream& os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;
    };

    void init();
private:
    std::string m_partern;
    std::vector<FormatItem::ptr> m_items;
};

//日志输出地
class LogAppender{
public:
    typedef std::shared_ptr<LogAppender> ptr;
    // LogAppender();
    virtual ~LogAppender() {};

    virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr& event) = 0;
    void setFormatter(LogFormatter::ptr val) {m_formatter = val;}
    LogFormatter::ptr getFormatter(){return m_formatter;}
protected:
    LogLevel::Level m_level;
    LogFormatter::ptr m_formatter;
};



//日志器
class Logger{
public:
    typedef std::shared_ptr<Logger> ptr;
    //日志等级

    Logger(const std::string& name = "root");
    void log(LogLevel::Level level, LogEvent::ptr& event);
    void addAppender(LogAppender::ptr appender);
    void delAppender(LogAppender::ptr appender);

    LogLevel::Level getLevel() const {return m_level;}
    void setLevel(LogLevel::Level val) {m_level = val;}
    const std::string& getName() const {return m_name;}

    void debug(LogEvent::ptr event);
    void info(LogEvent::ptr event);
    void warn(LogEvent::ptr event);
    void error(LogEvent::ptr event);
    void fatal(LogEvent::ptr event);
private:
    std::string m_name;                 //日志名称
    LogLevel::Level m_level;            //日志级别
    std::list<LogAppender::ptr> m_appenders;    //Appender集合
};

//输出到控制台的Appender
class StdoutLogAppender:public LogAppender{
public:
    typedef std::shared_ptr<StdoutLogAppender> ptr;
    void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr& event) override;
};

//输出到文件的Appender
class FileLogAppender:public LogAppender{
public:
    typedef std::shared_ptr<FileLogAppender> ptr;
    FileLogAppender(const std::string& filename);
    void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr& event) override;
    //重新打开文件 成功为true
    bool reOpen();
private:
    std::string m_filename;
    std::ofstream m_filestream;
};
#endif