//
// Created by 苏畅 on 2019/6/25.
//

#ifndef REIMU_LOGGING_H
#define REIMU_LOGGING_H

#include "../reimu_imp.h"
#include "../Util.h"


namespace reimu {
    class Logging : public noncopyable {
    private:
        Logging();
    public:
        enum LogLevel {
            LOG_ERROR =0,
            LOG_WARN,
            LOG_INFO,
            LOG_DEBUG
        };

        inline static const char LOG_LEVEL_STR[4][20] = {"ERROR", "WARN", "INFO", "DEBUG"};
        typedef std::pair<LogLevel , const std::string> LogTask;
    private:
        inline static Logging *_logger = nullptr;
        LogLevel _level;
        std::mutex _mutex;
        std::thread _t;
        std::function<void (const LogTask)> _log_cb;

        SafeQueue<LogTask> _log_queue;

    public:
        static Logging *GetLogger();
        LogLevel GetLogLevel();
        void SetLogLevel(LogLevel level);

        void Info(const std::string & s);
        void Error(const std::string & s);
        void Warn(const std::string & s);
        void Debug(const std::string & s);

        void Log(LogLevel level, const std::string &s);



    };


    class InfoLogger : public  noncopyable{
        friend std::istream &operator>>( std::istream  &input, InfoLogger &l )
        {
            std::string s;
            input >>s ;
            Logging::GetLogger()->Info(s);
            return input;
        }
    };

    class DebugLogger : public  noncopyable{
        friend std::istream &operator>>( std::istream  &input, DebugLogger &l )
        {
            std::string s;
            input >>s ;
            Logging::GetLogger()->Debug(s);
            return input;
        }
    };

    inline  InfoLogger INFO_LOG = InfoLogger();
    inline  DebugLogger DEBUG_LOG = DebugLogger();


}


#endif //REIMU_LOGGING_H
