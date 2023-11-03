//
// Created by EDY on 2022/9/21.
//

#include <jni_helper.h>
#include <LogUtil.h>
#include "elem_processor_jni.h"
#include "group_elem_handler_jni.h"
#include "TIMCloudDef.h"

namespace tim {
    namespace jni {

        ElemProcessor::ElemProcessor() {}

        ElemProcessor::~ElemProcessor() { handlers_.clear(); }

        ElemProcessor &ElemProcessor::GetInstance() {
            static auto *instance = new ElemProcessor();
            return *instance;
        }

        void ElemProcessor::RegisterElemHandler() {
            RegisterElemHandler(std::make_unique<TextElemHandler>());
            RegisterElemHandler(std::make_unique<CustomElemHandler>());
            RegisterElemHandler(std::make_unique<LocationElemHandler>());
            RegisterElemHandler(std::make_unique<FaceElemHandler>());
            RegisterElemHandler(std::make_unique<FileElemHandler>());
            RegisterElemHandler(std::make_unique<SoundElemHandler>());
            RegisterElemHandler(std::make_unique<ImageElemHandler>());
            RegisterElemHandler(std::make_unique<VideoElemHandler>());
            RegisterElemHandler(std::make_unique<GroupElemHandlerJni>());
            RegisterElemHandler(std::make_unique<MergerElemHandler>());
        }

        std::unique_ptr<json::Object> ElemProcessor::BuildElem(int elemType, jobject elemObj) {
            std::unique_ptr<json::Object> elem = nullptr;
            for (auto &item: handlers_) {
                elem = item->Convert2CoreObject(elemType, elemObj);
                if (elem) break;
            }

            return elem;
        }

        jobject ElemProcessor::ParseElem(const std::string &convId,const std::string &msgId,const uint64_t &sequence, const int &elemType,const json::Object &elem_json) {
            jobject obj = nullptr;

            for (auto &item: handlers_) {
                obj = item->Convert2JObject(elem_json);
                if (obj) break;
            }

            return obj;
        }

        void ElemProcessor::RegisterElemHandler(std::unique_ptr<IElemHandler> handler) {
            if (nullptr == handler) {
                return;
            }
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();
            if (env) {
                if (!handler->InitIDs(env)) {
                    std::string handler_type = handler->GetHandlerType();
                    LOGE("%s InitIDs failed", handler_type.c_str());
                }
            }

            handlers_.push_back(std::move(handler));
        }

    }// namespace tim
}// namespace jni