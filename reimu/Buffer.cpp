//
// Created by 苏畅 on 2019/6/22.
//

#include "Buffer.h"


namespace reimu {
    Slice Buffer::Read(size_t n) {
        if (_read_idx + n >= _write_idx) {
            auto s = Slice(&*(_v.begin() + _read_idx), &*(_v.begin() + _write_idx));
            _read_idx = _write_idx;
            return s;
        } else {
            auto s = Slice(&*(_v.begin() + _read_idx), &*(_v.begin() + n));
            _read_idx = _read_idx + n;
            return s;
        }
    }

    Buffer& Buffer::Write(const char *s) {
        size_t size = strlen(s);
        int flag = 0;
        while (flag < size){
            _v.push_back(s[flag]);
            flag++;
        }
        _write_idx  = _write_idx + size;

        // 写入的时候进行缩减
        if (_read_idx > MAX_PREPENND_LENGTH) {
            prepend();
        }
        return *this;
    }

    long Buffer::prepend() {
        long old_read_idx = _read_idx;
        _v.erase(_v.begin(), _v.begin() + _read_idx);
        _read_idx = 0;
        _write_idx = _write_idx - old_read_idx;
        return old_read_idx;
    }

    Slice Buffer::ReadAll() {
        auto s = Slice(&*(_v.begin() + _read_idx), &*(_v.begin() + _write_idx));
        _read_idx = _write_idx;
        return s;
    }

}