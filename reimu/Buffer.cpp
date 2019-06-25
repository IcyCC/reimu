//
// Created by 苏畅 on 2019/6/22.
//

#include "Buffer.h"


namespace reimu {
    Slice Buffer::Read(size_t n) {
        std::lock_guard<std::mutex> lock(_mutex);
        prepend();
        if (_read_idx + n >= _write_idx) {
            auto s = Slice(shared_from_this(), &*(_v.begin() + _read_idx), &*(_v.begin() + _write_idx));
            _read_idx = _write_idx;
            return s;
        } else {
            auto s = Slice(shared_from_this(), &*(_v.begin() + _read_idx), &*(_v.begin() + n));
            _read_idx = _read_idx + n;
            return s;
        }
    }

    Buffer &Buffer::Write(const std::string &s) {
        std::lock_guard<std::mutex> lock(_mutex);
        size_t size = s.size();
        int flag = 0;
        while (flag < size) {
            _v.push_back(s[flag]);
            flag++;
        }
        _write_idx = _write_idx + size;

        // 写入的时候进行缩减

        prepend();
        return *this;
    }

    long Buffer::prepend() {
        auto use_cout = this->weak_from_this().use_count();
        if (_read_idx > MAX_PREPENND_LENGTH && use_cout == 1) {
            // 无人使用
            long old_read_idx = _read_idx;
            _v.erase(_v.begin(), _v.begin() + _read_idx);
            _read_idx = 0;
            _write_idx = _write_idx - old_read_idx;
            return old_read_idx;
        }
        return _read_idx;
    }

    Slice Buffer::ReadAll() {
        std::lock_guard<std::mutex> lock(_mutex);
        prepend();
        auto s = Slice(shared_from_this(), &*(_v.begin() + _read_idx), &*(_v.begin() + _write_idx));
        _read_idx = _write_idx;
        return s;
    }

}