#include "log.h"
#include <time.h>
#include <string.h>

#include <stdarg.h>

using namespace kuril::utility;

//初始化static
const char* log::s_level[LEVEL_COUT] =
{
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "FATAL"
};

log* log::m_instance = NULL;

log* log::getInstance()
{
    if (m_instance == NULL)
    {
        m_instance = new log();
    }
    return m_instance;
}


void log::open(const string& filename)
{
    // log::m_filename = filename;
    // log::m_file = fopen((const char*)&m_filename, "a+");
    // if (m_file == NULL)
    // {
    //     throw logic_error("open file failed" + m_filename);
    // }
    m_filename = filename;
    m_fout.open(filename,ios::app);
}
void log::close()
{
    m_fout.close();
}

void log::write_log(LEVEL level, const char* file, int line, const char* format, ...)
{
    if (m_level > level) return;//检测等级是否低于默认

    if(m_fout.fail())
    {
        throw logic_error("m_fout lose");
    }

    time_t ticks = time(NULL);
    struct tm* ptm = localtime(&ticks);
    char timestamp[32];
    memset(timestamp, 0, sizeof(timestamp));
    strftime(timestamp,sizeof(timestamp), "%Y-%m-%d %H-%M-%S", ptm);
    
    const char* fmt = "%s %s %s %d ";

    int size = snprintf(NULL, 0, fmt, timestamp, s_level[level], file,line);
    if (size > 0)
    {
        char* buffer = new char[size+1];
        memset(buffer, 0, sizeof(buffer));
        snprintf(buffer,size+1, fmt, timestamp, s_level[level], file, line);
        buffer[size] = 0;
        m_fout << buffer;
        cout << buffer << endl;

        delete buffer;
    }

    va_list arg_ptr;
    va_start(arg_ptr, format);
    size = vsnprintf(NULL,0,format,arg_ptr);
    va_end(arg_ptr);

    if (size > 0)
    {
        char* content = new char[size+1];
        memset(content, 0, sizeof(content));
        va_start(arg_ptr,format);
        vsnprintf(content,size+1,format, arg_ptr);
        va_end(arg_ptr);

        m_fout << content;
        cout << "备注:" << content << endl;

    }
    m_fout << '\n';
    m_fout.flush();

}