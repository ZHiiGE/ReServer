#include "Buffer.h"

Buffer::Buffer(uint16_t sep):m_sep(sep){
}

Buffer::~Buffer(){
}

void Buffer::append(const char* data, size_t size){
    m_buf.append(data, size);
}

void Buffer::appendWithsep(const char* data, size_t size){
    switch (m_sep)
    {
    case 0:
        m_buf.append(data, size);
        break;
    case 1:
        m_buf.append((char*)&size, 4);//添加报文头部
        m_buf.append(data, size);
        break;
    case 2:
        m_buf.append(data, size);
        m_buf.append("\r\n");
    }
}

bool Buffer::pickMessage(std::string &ss){
    if(m_buf.size() == 0) return false;
    switch (m_sep)
    {
    case 0:
        ss = m_buf;
        m_buf.clear();
        break;
    case 1:
        int len;//获取报文长度
        memcpy(&len, m_buf.data(), 4);
        if(m_buf.size()<len+4) break; //报文长度不足
        ss = m_buf.substr(4, len);//获取报文体
        m_buf.erase(0, len+4);//从缓冲区中删除以获取报文
        break;
    case 2:
        break;
    }
    return true;
}

void Buffer::erase(size_t pos, size_t nn){
    m_buf.erase(pos, nn);
}

size_t Buffer::size(){
    return m_buf.size();
}

const char* Buffer::data(){
    return m_buf.data();
}

void Buffer::clear(){
    m_buf.clear();
}