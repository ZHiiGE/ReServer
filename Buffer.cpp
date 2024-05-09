#include "Buffer.h"

Buffer::Buffer(){
}

Buffer::~Buffer(){
}

void Buffer::append(const char* data, size_t size){
    m_buf.append(data, size);
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