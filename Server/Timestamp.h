#ifndef _TIMESTAMP_H
#define _TIMESTAMP_H

#include <iostream>
#include <string>

class Timestamp{
private:
    time_t m_secsinceepoch;
public:
    Timestamp();
    Timestamp(int64_t secsinceepoch);
    
    ~Timestamp();
    

    //返回当前时间的Timestamp对象
    static Timestamp now();
    //返回整数表示的事件
    time_t toint() const;
    //返回字符串表示的时间
    std::string tostring() const;
};


#endif