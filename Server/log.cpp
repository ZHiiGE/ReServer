#include "log.h"
#include <map>
#include <functional>

class MessageFormatItem : public LogFormatter::FormatItem{
public:
    using LogFormatter::FormatItem::FormatItem;
    void format(std::ostream& os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override{
        os<<event->getContent();
    }
};

class LevelFormatItem : public LogFormatter::FormatItem{
public:
    using LogFormatter::FormatItem::FormatItem;
    void format(std::ostream& os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override{
        os<<LogLevel::ToString(level);
    }
};

class ElapseFormatItem : public LogFormatter::FormatItem{
public:
    using LogFormatter::FormatItem::FormatItem;
    void format(std:: ostream& os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override{
        os<< event->getElapse();
    }
}; 

class NameFormatItem : public LogFormatter::FormatItem{
public:
    using LogFormatter::FormatItem::FormatItem;
    void format(std:: ostream& os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override{
        os<< logger->getName();
    }
}; 

class ThreadIdFormatItem : public LogFormatter::FormatItem{
public:
    using LogFormatter::FormatItem::FormatItem;
    void format(std:: ostream& os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override{
        os<< event->getThreadId();
    }
}; 

class FiberIdFormatItem : public LogFormatter::FormatItem{
public:
    using LogFormatter::FormatItem::FormatItem;
    void format(std:: ostream& os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override{
        os<< event->getFiberId();
    }
}; 

class StringFormatItem : public LogFormatter::FormatItem{
public:
    StringFormatItem(const std::string& str)
        :FormatItem(str), m_string(str){}
    void format(std:: ostream& os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override{
        os<< m_string;
    }
private:
    std::string m_string;
};

class DateTimeFormatItem : public LogFormatter::FormatItem{
public:
    using LogFormatter::FormatItem::FormatItem;
    DateTimeFormatItem(const std::string& format = "%Y:%m:%d %H:%M:%S")
        :m_format(format){}
    void format(std:: ostream& os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override{
        os<< event->getTime();
    }
private:
    std::string m_format;
}; 

class FilenameIdFormatItem : public LogFormatter::FormatItem{
public:
    using LogFormatter::FormatItem::FormatItem;
    void format(std:: ostream& os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override{
        os<< event->getFile();
    }
}; 

class LineFormatItem : public LogFormatter::FormatItem{
public:
    using LogFormatter::FormatItem::FormatItem;
    void format(std:: ostream& os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override{
        os<< event->getLine();
    }
}; 

class NewlineFormatItem : public LogFormatter::FormatItem{
public:
    using LogFormatter::FormatItem::FormatItem;
    void format(std:: ostream& os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override{
        os<< std::endl;
    }
}; 


Logger::Logger(const std::string &name)
        :m_name(name){

}

void Logger::log(LogLevel::Level level, LogEvent::ptr &event)
{
    if(level >= m_level){
        for(auto& i:m_appenders){
            i->log(ptr(this),  level, event);
        }
    }
}

void Logger::addAppender(LogAppender::ptr appender){
    m_appenders.push_back(appender);
}

void Logger::delAppender(LogAppender::ptr appender){
    for(auto it = m_appenders.begin();it!=m_appenders.end();++it){
        if(*it = appender){
            m_appenders.erase(it);
            break;
        }
    }
}

void Logger::debug(LogEvent::ptr event){
    log(LogLevel::DEBUG, event);
}

void Logger::info(LogEvent::ptr event)
{
    log(LogLevel::INFO, event);
}

void Logger::warn(LogEvent::ptr event)
{
    log(LogLevel::WARN, event);
}

void Logger::error(LogEvent::ptr event)
{
    log(LogLevel::ERROR, event);
}

void Logger::fatal(LogEvent::ptr event)
{
    log(LogLevel::FATAL, event);
}

void StdoutLogAppender::log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr &event){
    if(level >= m_level){
        std::cout<< m_formatter->format(logger, level, event);
    }

}

FileLogAppender::FileLogAppender(const std::string &filename)
        :m_filename(filename){
    
}

void FileLogAppender::log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr &event){
    if(level >= m_level){
        m_filestream<<m_formatter->format(logger, level, event);
    }
}

bool FileLogAppender::reOpen(){
    if(m_filestream){
        m_filestream.close();
    }
    m_filestream.open(m_filename);
    return !!m_filestream;
}

LogFormatter::LogFormatter(const std::string &pattern)
        :m_partern(pattern){
}

std::string LogFormatter::format(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event)
{
    std::stringstream ss;
    for(auto& i:m_items){
        i->format(ss, logger, level, event);
    }
    return ss.str();
}

void LogFormatter::init(){
    //str, format, type
    std::vector<std::tuple<std::string, std::string, int>> vec;
    std::string nstr;
    for(size_t i=0;i<m_partern.size();++i){
        if(m_partern[i] != '%'){
            nstr.append(1, m_partern[i]);
            continue;
        }

        if(i+1 < m_partern.size()){
            if(m_partern[i+1] == '%'){
                nstr.append(1, '%');
                continue;
            }
        }

        size_t n = i+1;
        size_t fmt_begin = 0;
        int fmt_status = 0;

        std::string str;
        std::string fmt;
        while(n<m_partern.size()){
            if(isspace(m_partern[n])){
                break;
            }
            if(fmt_status == 0){
                if(m_partern[n] == '{'){
                    str = m_partern.substr(i+1, n-i-1);//左括号之前为str
                    fmt_status = 1;
                    fmt_begin = n;
                    ++n;
                    continue;
                }
            }
            if(fmt_status == 1){
                if(m_partern[n] == '}'){
                    fmt = m_partern.substr(fmt_begin, n-fmt_begin-1);
                    fmt_status = 2;
                    ++n;
                    break;
                }
            }
        }
        if(fmt_status == 0){
            if(!nstr.empty()){
                vec.emplace_back(nstr, "", 0);
            }
            str= m_partern.substr(i+1, n-i-1);
            vec.emplace_back(str, fmt, 1);
            i=n;
        }
        else if(fmt_status == 1){
            std::cout<<"pattern parse error: "<<m_partern<<" - "<<m_partern.substr(i)<<std::endl;
            vec.emplace_back("<<pattern error>>", fmt, 0);
        }
        else if(fmt_status == 2){
            vec.emplace_back(str, fmt, 1);
            i=n;
        }
    } 
    if(!nstr.empty()){
        vec.emplace_back(nstr, "", 0);
    }
    std::map<std::string, std::function<FormatItem::ptr(const std::string& str)>> s_format_items = {
#define XX(str, C) \
        {str, [](const std::string& fmt){ return FormatItem::ptr(new C(fmt));} }

        XX("m", MessageFormatItem),
        XX("p", LevelFormatItem),
        XX("r", ElapseFormatItem),
        XX("c", NameFormatItem),
        XX("t", ThreadIdFormatItem),
        XX("n", NewlineFormatItem),
        XX("d", DateTimeFormatItem),
        XX("f", FilenameIdFormatItem),
        XX("l", LineFormatItem),
#undef XX
    };

    for(auto& i:vec){
        if(std::get<2>(i) == 0){
            m_items.push_back(FormatItem::ptr(new StringFormatItem(std::get<0>(i))));
        }else{
            auto it = s_format_items.find(std::get<1>(i));
            if(it == s_format_items.end()){
                m_items.push_back(FormatItem::ptr(new StringFormatItem("<<error_format %" + std::get<0>(i) + ">>")));
            }else{
                m_items.push_back(it->second(std::get<0>(i)));
            }
        }
        std::cout << std::get<0>(i) <<" - "<<std::get<1>(i) << " - " << std::get<2>(i) <<std::endl;
    }

// %m 消息体
// %p level
// %r 启动后时间
// %c 日志名称
// %t 线程id
// %n 回车换行
// %d 时间
// %f 文件名
// %l 行号

}
const char *LogLevel::ToString(LogLevel::Level level)
{
    switch (level)
    {
#define XX(name) \
    case LogLevel::name: \
        return #name; \
        break;
    XX(DEBUG);
    XX(INFO);
    XX(WARN);
    XX(ERROR);
    XX(FATAL);
#undef XX

    default:
        return "UNKNOW";
    }
    return "UNKNOW";
}


