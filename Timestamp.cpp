#include "Timestamp.h"

Timestamp::Timestamp(){
    m_secsinceepoch=time(0);
}

Timestamp::Timestamp(int64_t secsinceepoch):m_secsinceepoch(secsinceepoch){}

Timestamp::~Timestamp(){}

Timestamp Timestamp::now(){
    return Timestamp();
}

time_t Timestamp::toint() const{
    return m_secsinceepoch;
}

std::string Timestamp::tostring() const{
    char buf[32] = {0};
    tm* tm_time = localtime(&m_secsinceepoch);
    snprintf(buf, 32, "%4d-%02d-%02d %02d:%02d:%02d",
            tm_time->tm_year + 1900,
            tm_time->tm_mon + 1,
            tm_time->tm_mday,
            tm_time->tm_hour,
            tm_time->tm_min,
            tm_time->tm_sec);
    return buf;
}