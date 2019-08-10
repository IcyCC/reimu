//
// Created by 苏畅 on 2019/6/25.
//

#ifndef REIMU_LOGGING_H
#define REIMU_LOGGING_H

#include "reimu_imp.h"
#include "Util.h"
#include "thead/SafeQueue.h"


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
        typedef std::shared_ptr<LogTask> LogTaskPtr;
    private:
        inline static Logging *_logger = nullptr;
        LogLevel _level;
        inline static std::mutex _mutex = std::mutex();
        std::thread _t;
        std::function<void (const LogTaskPtr)> _log_cb;

        SafeQueue<LogTaskPtr> _log_queue;

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
    public:
        template <typename  ...Args>
        void Log(const char* format, Args ...args) {
            const int  MAX_LOG_LENGTH = 1024;
            char s[MAX_LOG_LENGTH];
            snprintf(s, sizeof(s),  format, args...);
            Logging::GetLogger()->Info(s);
        }
    };

    class DebugLogger : public  noncopyable{
    public:
        template <typename  ...Args>
        void Log(const char*  format, Args ...args) {
            const int  MAX_LOG_LENGTH = 1024;
            char s[MAX_LOG_LENGTH];
            snprintf(s, sizeof(s),  format, args...);
            Logging::GetLogger()->Debug(s);
        }
    };

    class WarnLogger : public  noncopyable{
    public:
        template <typename  ...Args>
        void Log(const char* & format, Args ...args) {
            const int  MAX_LOG_LENGTH = 1024;
            char s[MAX_LOG_LENGTH];
            snprintf(s, sizeof(s),  format, args...);
            Logging::GetLogger()->Warn(s);
        }
    };

    class ErrorLogger : public  noncopyable{
    public:
        template <typename  ...Args>
        void Log(const char* format, Args ...args) {
            const int  MAX_LOG_LENGTH = 1024;
            char s[MAX_LOG_LENGTH];
            snprintf(s, sizeof(s),  format, args...);
            Logging::GetLogger()->Error(s);
        }
    };

    inline  InfoLogger INFO_LOG = InfoLogger();
    inline  DebugLogger DEBUG_LOG = DebugLogger();
    inline  WarnLogger WARN_LOG = WarnLogger();
    inline  ErrorLogger ERROR_LOG = ErrorLogger();

    inline  Logging* DefaultLogger = Logging::GetLogger();


}


#endif //REIMU_LOGGING_H
