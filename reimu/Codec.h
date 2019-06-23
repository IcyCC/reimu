//
// Created by 苏畅 on 2019/6/22.
//

#ifndef REIMU_CODEC_H
#define REIMU_CODEC_H

#include "Buffer.h"
#include "reimu_imp.h"

namespace reimu {
    class CodecBase {
        virtual Slice tryDecode(const Slice& data) = 0;
        virtual void encode(Slice msg, Buffer &buf) = 0;
        virtual CodecBase *clone() = 0;
    };

    //以\r\n结尾的消息
    struct LineCodec : public CodecBase {
        Slice tryDecode(const Slice& data) override;

        void encode(Slice msg, Buffer &buf) override;

        CodecBase *clone() override { return new LineCodec(); }
    };

}


#endif //REIMU_CODEC_H
