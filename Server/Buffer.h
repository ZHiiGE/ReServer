#ifndef _BUFFER_H
#define _BUFFER_H

#include <string>
#include <string.h>
#include <iostream>
#include <unistd.h>

/**
 * @class:Buffer
 * @brief:封装输入输出缓冲区类
*/

class Buffer
{
private:
    //缓冲区
    std::string m_buf;
    //分隔符类别： 0-无分隔符  1-四字节报文头 
    const uint16_t m_sep;
public:
    Buffer(uint16_t sep = 0);
    ~Buffer();

    //从socket中读取数据
    int readFd(int);
    //添加数据
    void append(const char* data, size_t size); 
    //带报文头添加数据
    void appendWithsep(const char* data, size_t size);
    //读取报文体
    bool pickMessage(std::string&);
    //删除指定区间的数据
    void erase(size_t pos, size_t nn);  
    //返回数据长度        
    size_t size();      
    //返回m_buf首地址                        
    const char* data();  
    //清空m_buf                       
    void clear();                               
};


#endif