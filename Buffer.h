#ifndef _BUFFER_H
#define _BUFFER_H

#include <string>
#include <iostream>

/**
 * @class:Buffer
 * @brief:封装输入输出缓冲区类
*/

class Buffer
{
private:
    std::string m_buf;
public:
    Buffer(/* args */);
    ~Buffer();

    void append(const char* data, size_t size); //添加数据
    size_t size();                              //返回数据长度
    const char* data();                         //返回m_buf首地址
    void clear();                               //清空m_buf
};


#endif