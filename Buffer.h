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
    //缓冲区
    std::string m_buf;
public:
    Buffer(/* args */);
    ~Buffer();

    //添加数据
    void append(const char* data, size_t size); 
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