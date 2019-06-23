//
// Created by 苏畅 on 2019/6/23.
//

#include "../reimu/reimu.h"
#include <iostream>

int main() {
    auto buf = reimu::Buffer();
    buf.Write("hello thank you ");
    auto codec = reimu::LineCodec();

    codec.encode("test",buf);
    codec.encode("test 2",buf);

    auto s1 = buf.ToSlice();
    std::cout<<"NO1 :  "<<s1.toString()<<std::endl;

    auto s2 = codec.tryDecode(buf.ToSlice());
    std::cout<<"NO2 :  "<<s2.toString()<<std::endl;
    buf.Consume(s2.size());

    auto s3 = codec.tryDecode(buf.ToSlice());
    std::cout<<"NO3 : "<<s3.toString()<<std::endl;
    buf.Consume(s3.size());

    std::cout<<"NO4 : "<<buf.ToSlice().toString()<<std::endl;
}