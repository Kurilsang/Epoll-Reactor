#pragma once
#include <iostream>
#include <string>
#include <fstream>
using namespace std;
//使用单例模式实现的一个日志模块
namespace kuril
{
    namespace utility
    {
        #define debug(format,...) \
        log::getInstance()->write_log(log::DEBUG, __FILE__,__LINE__,format,##__VA_ARGS__)
        #define info(format,...) \
        log::getInstance()->write_log(log::INFO, __FILE__,__LINE__,format,##__VA_ARGS__)
        #define warn(format,...) \
        log::getInstance()->write_log(log::WARN, __FILE__,__LINE__,format,##__VA_ARGS__)
        #define error(format,...) \
        log::getInstance()->write_log(log::ERROR, __FILE__,__LINE__,format,##__VA_ARGS__)
        #define fatal(format,...) \
        log::getInstance()->write_log(log::FATAL, __FILE__,__LINE__,format,##__VA_ARGS__)

        class log
        {
            public:

            static log* getInstance();//单例模式

            // 等级相关
            enum LEVEL
            {
                DEBUG = 0,
                INFO,
                WARN,
                ERROR,
                FATAL,
                LEVEL_COUT
            };
            //设置等级屏蔽低级日志信息
            void level(LEVEL level)
            {
                m_level = level;
            }

            void open(const string& filename);//传入路径打开文件描述符
            //传入等级，写入路径，报错行数，备注信息后往日志文件写入
            void write_log(LEVEL level, const char* file, int line, const char* format, ...);
            void close();//关闭文件描述符

            private:

            log() :m_level(DEBUG)
            {
                
            }
            ~log()
            {
                delete m_instance;
                close();
            }
            static log* m_instance;//单例模式

            string m_filename;//存储传入的路径
            ofstream m_fout;//根据传入的路径打开的文件描述符
            
            // 等级相关
            static const char* s_level[LEVEL_COUT];
            LEVEL m_level;



        };
    }
}