//
// Created by EDY on 2022/9/21.
//

#ifndef IMCPLUSPLUSDEMO_ELEM_PROCESSOR_JNI_H
#define IMCPLUSPLUSDEMO_ELEM_PROCESSOR_JNI_H

#include <jni.h>
#include <memory>

#include "elem_handler_jni.h"
#include <vector>
#include "json.h"

namespace tim {
    namespace jni {
        class ElemProcessor final {
        public:
            ElemProcessor();
            ~ElemProcessor();

            static ElemProcessor &GetInstance();
            void RegisterElemHandler();
//            jobject ParseElem(const json::Object &message_json, const int elemType,const int position);
            jobject ParseElem(const std::string &convId,const std::string &msgId,const uint64_t &sequence, const int &elemType,const json::Object &elem_json);
//            std::unique_ptr<const char *> BuildElem(int elemType, jobject elemObj);
            std::unique_ptr<json::Object> BuildElem(int elemType, jobject elemObj);

        protected:
            void RegisterElemHandler(std::unique_ptr<IElemHandler> handler);

        private:
            std::vector<std::unique_ptr<IElemHandler>> handlers_;
        };
    }// namespace tim
}// namespace jni


#endif //IMCPLUSPLUSDEMO_ELEM_PROCESSOR_JNI_H
