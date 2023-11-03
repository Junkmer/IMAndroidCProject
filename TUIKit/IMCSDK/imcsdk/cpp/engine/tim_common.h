//
// Created by EDY on 2022/10/9.
//

#ifndef IMCPLUSPLUSDEMO_V2_IM_COMMON_H
#define IMCPLUSPLUSDEMO_V2_IM_COMMON_H

namespace tim {

    // 公共可变参数回调定义
    template<typename... Args>
    using Callback = std::function<void(const int &error_code, const char *error_message, Args...)>;

}  // namespace tim

#endif //IMCPLUSPLUSDEMO_V2_IM_COMMON_H
