//
// Created by 苏畅 on 2019/6/25.
//

#include "Logging.h"

namespace reimu {

    Logging::Logging() {
        _level = LogLevel::LOG_INFO;
        _log_cb = [this](const LogTask &t) {
            std::cout << util::CurrentReadableTime() << " " << Logging::LOG_LEVEL_STR[t.first] << " " << t.second;
        };
        _t = std::thread([this]() {
            while (!this->_log_queue.Exited()) {
                auto s = _log_queue.PopWait();
                if (_log_cb != nullptr) {
                    _log_cb(s);
                }
            }
        });
    }

    Logging *Logging::GetLogger() {
        std::lock_guard<std::mutex> lock(_mutex);
        if (_logger == nullptr) {
            _logger = new Logging();
        }
        return _logger;
    }

    Logging::LogLevel Logging::GetLogLevel() {
        return _level;
    }

    void Logging::SetLogLevel(LogLevel level) {
        _level = level;
    }


    void Logging::Log(reimu::Logging::LogLevel level, const std::string &s) {
        if (level <= _level) {
            _log_queue.Push(std::make_pair(level, s));
        }
    }


    void Logging::Debug(const std::string &s) {
        Log(LogLevel::LOG_DEBUG, s);
    }

    void Logging::Info(const std::string &s) {
        Log(LogLevel::LOG_INFO, s);
    }

    void Logging::Warn(const std::string &s) {
        Log(LogLevel::LOG_WARN, s);
    }

    void Logging::Error(const std::string &s) {
        Log(LogLevel::LOG_ERROR, s);
    }
}