//
// Created by 苏畅 on 2019/6/22.
//

#include "Codec.h"

namespace reimu {
    void LineCodec::encode(reimu::Slice msg, reimu::BufferPtr buf) {
        buf->Write(msg.data()).Write("\r\n");
    }

    std::string LineCodec::tryDecode(reimu::Slice data) {
        auto msg ="";
        if (data.size() == 1 && data[0] == 0x04) {
            return msg;
        }
        for (size_t i = 0; i < data.size(); i++) {
            if (data[i] == '\n'){
                if (i > 0 && data[i-1] == '\r') {
                    return data.toString();
                }
            }
        }
        return msg;
    }
}