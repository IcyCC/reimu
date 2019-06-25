//
// Created by 苏畅 on 2019/6/22.
//

#ifndef REIMU_BUFFER_H
#define REIMU_BUFFER_H

#include <vector>
#include <string>
#include <cstring>
#include <mutex>
#include "Slice.h"

namespace reimu {
    class Buffer {
    private:
        const size_t MAX_PREPENND_LENGTH = 3000;
        std::vector<char> _v;
        size_t _write_idx = 0, _read_idx = 0;
        std::mutex _mutex;
    public:
        Slice Read(size_t n);

        Slice ReadAll();

        void PeekReadPos(size_t read_pos) {
            std::lock_guard<std::mutex> lock(_mutex);
            if (read_pos > 0) {
                _read_idx = read_pos;
            }
        };

        long GetReadPos() {
            std::lock_guard<std::mutex> lock(_mutex);
            return _read_idx;
        }

        Buffer &Write(const std::string & s);

        void Consume(size_t n) {
            std::lock_guard<std::mutex> lock(_mutex);
            if (_read_idx + n > _write_idx) {
                _read_idx = _write_idx;
            } else {
                _read_idx = _read_idx + n;
            }
        }

        Slice ToSlice(){
            std::lock_guard<std::mutex> lock(_mutex);
            return  Slice(&*(_v.begin() + _read_idx), &*(_v.begin() + _write_idx));
        }

    private:
        long prepend(); //缩减
    };
}


#endif //REIMU_BUFFER_H