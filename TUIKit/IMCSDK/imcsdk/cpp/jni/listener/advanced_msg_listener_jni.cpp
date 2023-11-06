//
// Created by EDY on 2022/9/27.
//

#include <LogUtil.h>
#include <tim_engine.h>
#include "advanced_msg_listener_jni.h"

#include "jni_helper.h"
#include "message_jni.h"
#include "java_basic_jni.h"
#include "message_receipt_jni.h"
#include "TIMCloud.h"

namespace tim {
    namespace jni {

        jclass AdvancedMsgListenerJni::j_cls_;
        jmethodID AdvancedMsgListenerJni::j_method_id_array_[MethodIDMax];

        void AdvancedMsgListenerJni::AddListener(JNIEnv *env, jobject listener_advanced_msg, jstring listenerPath) {
            if (nullptr == listener_advanced_msg) {
                LOGE("AdvancedMsgListenerJni | AddListener listener_simple is null");
                return;
            }
            if (listener_advanced_msg_map.empty()) {
                TIMAddRecvNewMsgCallback(ImplTIMRecvNewMsgCallback, &listener_advanced_msg_map);
                TIMSetMsgReadedReceiptCallback(ImplTIMMsgReadedReceiptCallback, &listener_advanced_msg);
                TIMSetMsgRevokeCallback(ImplTIMMsgRevokeCallback, &listener_advanced_msg);
                TIMSetMsgUpdateCallback(ImplTIMMsgUpdateCallback, &listener_advanced_msg);
            }

            std::string path = StringJni::Jstring2Cstring(env, listenerPath);
            for (auto &item: listener_advanced_msg_map) {
                if (path.empty() || path == item.first) {
                    return;
                }
            }

            jobject j_obj = env->NewGlobalRef(listener_advanced_msg);
            listener_advanced_msg_map.insert(std::make_pair(path, j_obj));
        }

        void AdvancedMsgListenerJni::RemoveListener(JNIEnv *env, jstring listenerPath) {
            if (nullptr == listenerPath) {
                LOGE("AdvancedMsgListenerJni | RemoveListener listener_simple is null");
                return;
            }
            listener_advanced_msg_map.erase(StringJni::Jstring2Cstring(env, listenerPath));
            if (listener_advanced_msg_map.empty()) {
                TIMAddRecvNewMsgCallback(nullptr, nullptr);
                TIMSetMsgReadedReceiptCallback(nullptr, nullptr);
                TIMSetMsgRevokeCallback(nullptr, nullptr);
                TIMSetMsgUpdateCallback(nullptr, nullptr);
            }
        }

        bool AdvancedMsgListenerJni::InitIDs(JNIEnv *env) {
            if (j_cls_ != nullptr) {
                return true;
            }
            jclass cls = env->FindClass("com/tencent/imsdk/v2/V2TIMAdvancedMsgListener");
            if (cls == nullptr) {
                return false;
            }
            j_cls_ = (jclass) env->NewGlobalRef(cls);
            env->DeleteLocalRef(cls);

            jmethodID jmethod = nullptr;

            jmethod = env->GetMethodID(j_cls_, "onRecvNewMessage", "(Lcom/tencent/imsdk/v2/V2TIMMessage;)V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDOnRecvNewMessage] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "onRecvMessageReadReceipts", "(Ljava/util/List;)V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDOnRecvMessageReadReceipts] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "onRecvC2CReadReceipt", "(Ljava/util/List;)V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDOnRecvC2CReadReceipt] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "onRecvMessageRevoked", "(Ljava/lang/String;)V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDOnRecvMessageRevoked] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "onRecvMessageModified", "(Lcom/tencent/imsdk/v2/V2TIMMessage;)V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDOnRecvMessageModified] = jmethod;

            return true;
        }

        bool AdvancedMsgListenerJni::empty() {
            return listener_advanced_msg_map.empty();
        }

        void AdvancedMsgListenerJni::ImplTIMRecvNewMsgCallback(const char *json_msg_array, const void *user_data) {
            json::Array msg_array = json::Deserialize(json_msg_array);
            for (const auto &item: msg_array) {
                OnRecvNewMessage(*(std::map<std::string, jobject> *) user_data, item);
            }
        }

        void AdvancedMsgListenerJni::ImplTIMMsgReadedReceiptCallback(const char *json_msg_readed_receipt_array, const void *user_data) {
            json::Array read_receipt_array = json::Deserialize(json_msg_readed_receipt_array);
            json::Array c2CReadReceiptArray;
            json::Array messageReadReceipts;
            for (const auto &item: read_receipt_array) {
                int convType = item[kTIMMsgReceiptConvType];
                std::string msgID = item[kTIMMsgReceiptMsgId];
                if (convType == TIMConvType::kTIMConv_Group || !msgID.empty()) {
                    messageReadReceipts.push_back(item);
                } else {
                    c2CReadReceiptArray.push_back(item);
                }
            }
            if (c2CReadReceiptArray.size() > 0) {
                OnRecvC2CReadReceipt(*(std::map<std::string, jobject> *) user_data, c2CReadReceiptArray);
            }

            if (messageReadReceipts.size() > 0) {
                OnRecvMessageReadReceipts(*(std::map<std::string, jobject> *) user_data, messageReadReceipts);
            }
        }

        void AdvancedMsgListenerJni::ImplTIMMsgRevokeCallback(const char *json_msg_revoke_info_array, const void *user_data) {
            json::Array revoke_msg_array = json::Deserialize(json_msg_revoke_info_array);
            for (const auto &item: revoke_msg_array) {
                //TODO::撤回消息没有messageID字段，待新版发布
//                std::string msgId = item[kTIMMsgMsgId];
//                OnRecvMessageRevoked(*(std::map<std::string, jobject> *) user_data, msgId);
            }
        }

        void AdvancedMsgListenerJni::ImplTIMMsgUpdateCallback(const char *json_msg_array, const void *user_data) {
            json::Array msg_array = json::Deserialize(json_msg_array);
            for (const auto &item: msg_array) {
                OnRecvMessageModified(*(std::map<std::string, jobject> *) user_data, item);
            }
        }

        void AdvancedMsgListenerJni::OnRecvNewMessage(const std::map<std::string, jobject> &_listener_, const json::Object &message_json) {
            if (_listener_.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jobject j_obj_message = MessageJni::Convert2JObject(message_json);

            for (auto &item: _listener_) {
                env->CallVoidMethod(item.second, j_method_id_array_[MethodIDOnRecvNewMessage], j_obj_message);
            }

            env->DeleteLocalRef(j_obj_message);
        }

        void AdvancedMsgListenerJni::OnRecvC2CReadReceipt(const std::map<std::string, jobject> &_listener_, const json::Array &receiptList_json) {
            if (_listener_.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jobject j_obj_receiptList = ArrayListJni::NewArrayList();
            for (const auto &item: receiptList_json) {
                jobject j_obj_receipt = MessageReceiptJni::Convert2JObject(item);
                if (j_obj_receipt) {
                    ArrayListJni::Add(j_obj_receiptList, j_obj_receipt);
                    env->DeleteLocalRef(j_obj_receipt);
                }
            }

            for (auto &item: _listener_) {
                env->CallVoidMethod(item.second, j_method_id_array_[MethodIDOnRecvC2CReadReceipt], j_obj_receiptList);
            }

            env->DeleteLocalRef(j_obj_receiptList);
        }

        void
        AdvancedMsgListenerJni::OnRecvMessageReadReceipts(const std::map<std::string, jobject> &_listener_, const json::Array &receiptList_json) {
            if (_listener_.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jobject j_obj_receiptList = ArrayListJni::NewArrayList();
            for (const auto &item: receiptList_json) {
                jobject j_obj_receipt = MessageReceiptJni::Convert2JObject(item);
                if (j_obj_receipt) {
                    ArrayListJni::Add(j_obj_receiptList, j_obj_receipt);
                    env->DeleteLocalRef(j_obj_receipt);
                }
            }

            for (auto &item: _listener_) {
                env->CallVoidMethod(item.second, j_method_id_array_[MethodIDOnRecvMessageReadReceipts], j_obj_receiptList);
            }

            env->DeleteLocalRef(j_obj_receiptList);
        }

        void AdvancedMsgListenerJni::OnRecvMessageRevoked(const std::map<std::string, jobject> &_listener_, const std::string &messageID_json) {
            if (_listener_.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jstring msgIDStr = StringJni::Cstring2Jstring(env, messageID_json);

            for (auto &item: _listener_) {
                env->CallVoidMethod(item.second, j_method_id_array_[MethodIDOnRecvMessageRevoked], msgIDStr);
            }

            env->DeleteLocalRef(msgIDStr);
        }

        void AdvancedMsgListenerJni::OnRecvMessageModified(const std::map<std::string, jobject> &_listener_, const json::Object &message_json) {
            if (_listener_.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jobject j_obj_message = MessageJni::Convert2JObject(message_json);

            for (auto &item: _listener_) {
                env->CallVoidMethod(item.second, j_method_id_array_[MethodIDOnRecvMessageModified], j_obj_message);
            }

            env->DeleteLocalRef(j_obj_message);
        }

    }// namespace jni
}// namespace tim