//
// Created by Junker on 2022/9/15.
//

#include <LogUtil.h>
#include <java_basic_jni.h>
#include "simple_listener_jni.h"

#include "tim_engine.h"
#include "jni_helper.h"
#include "TIMCloud.h"
#include "TIMCloudDef.h"
#include "json.h"
#include "user_info_jni.h"
#include "group_member_full_info_jni.h"

namespace tim {
    namespace jni {

        jclass SimpleListenerJni::j_cls_;
        jmethodID SimpleListenerJni::j_method_id_array_[MethodIDMax];

        void SimpleListenerJni::AddListener(JNIEnv *env, jobject listener_simple) {
            if (nullptr == listener_simple) {
                LOGE("SimpleListenerJni | AddListener listener_simple is null");
                return;
            }
            jobject j_obj = env->NewGlobalRef(listener_simple);
            if (listener_simple_map.empty()) {
                TIMAddRecvNewMsgCallback(TIMAddRecvNewMsgCallbackImpl, &listener_simple_map);
            }
            listener_simple_map.insert(std::make_pair(listener_simple, j_obj));
        }

        void SimpleListenerJni::RemoveListener(jobject listener_simple) {
            if (nullptr == listener_simple) {
                LOGE("SimpleListenerJni | RemoveListener listener_simple is null");
                return;
            }
            listener_simple_map.erase(listener_simple);
            if (listener_simple_map.empty()) {
                TIMRemoveRecvNewMsgCallback(TIMAddRecvNewMsgCallbackImpl);
            }
        }

        bool SimpleListenerJni::InitIDs(JNIEnv *env) {
            if (nullptr != j_cls_) {
                return true;
            }

            jclass cls = env->FindClass("com/tencent/imsdk/v2/V2TIMSimpleMsgListener");
            if (nullptr == cls) {
                return false;
            }

            j_cls_ = (jclass) env->NewGlobalRef(cls);
            env->DeleteLocalRef(cls);

            jmethodID j_method = nullptr;
            j_method = env->GetMethodID(j_cls_, "onRecvC2CTextMessage",
                                        "(Ljava/lang/String;Lcom/tencent/imsdk/v2/V2TIMUserInfo;Ljava/lang/String;)V");
            if (nullptr == j_method) {
                return false;
            }
            j_method_id_array_[MethodIDOnRecvC2CTextMessage] = j_method;

            j_method = env->GetMethodID(j_cls_, "onRecvC2CCustomMessage",
                                        "(Ljava/lang/String;Lcom/tencent/imsdk/v2/V2TIMUserInfo;[B)V");
            if (nullptr == j_method) {
                return false;
            }
            j_method_id_array_[MethodIDOnRecvC2CCustomMessage] = j_method;

            j_method = env->GetMethodID(j_cls_, "onRecvGroupTextMessage",
                                        "(Ljava/lang/String;Ljava/lang/String;Lcom/tencent/imsdk/v2/V2TIMGroupMemberInfo;Ljava/lang/String;)V");
            if (nullptr == j_method) {
                return false;
            }
            j_method_id_array_[MethodIDOnRecvGroupTextMessage] = j_method;

            j_method = env->GetMethodID(j_cls_, "onRecvGroupCustomMessage",
                                        "(Ljava/lang/String;Ljava/lang/String;Lcom/tencent/imsdk/v2/V2TIMGroupMemberInfo;[B)V");
            if (nullptr == j_method) {
                return false;
            }
            j_method_id_array_[MethodIDOnRecvGroupCustomMessage] = j_method;

            return true;
        }

        bool SimpleListenerJni::empty() {
            return listener_simple_map.empty();
        }

        void SimpleListenerJni::OnRecvC2CTextMessage(const std::map<jobject, jobject> *_listener_map, const std::string &messageId,
                                                     const json::Object &sender, const std::string &text) {
            if (_listener_map->empty()) {
                return;
            }
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jstring msgIDJni = StringJni::Cstring2Jstring(env, messageId);
            jobject senderJni = UserInfoJni::Convert2JObject(sender);
            jstring textJni = StringJni::Cstring2Jstring(env, text);

            for (auto item_listener = _listener_map->begin(); item_listener != _listener_map->end(); item_listener++) {
                env->CallVoidMethod(item_listener->second, j_method_id_array_[MethodIDOnRecvC2CTextMessage], msgIDJni, senderJni, textJni);
            }
            env->DeleteLocalRef(msgIDJni);
            env->DeleteLocalRef(senderJni);
            env->DeleteLocalRef(textJni);
        }

        void SimpleListenerJni::TIMAddRecvNewMsgCallbackImpl(const char *json_msg_array, const void *user_data) {
            json::Object msg_json;
            msg_json = json::Deserialize(json_msg_array);
            json::Array elem_s = msg_json[kTIMMsgElemArray];
            json::Object elemObj = elem_s[0];
            int elem_type = elemObj[kTIMElemType];
            int conv_type = msg_json[kTIMConvType];
            auto listener_map = (std::map<jobject, jobject> *) user_data;

            std::string messageID = msg_json[kTIMMsgMsgId];
            if (conv_type == TIMConvType::kTIMConv_C2C) {
                json::Object sender_user_info_json = msg_json[kTIMMsgSenderProfile];
                if (elem_type == TIMElemType::kTIMElem_Text) {  // 文本
                    std::string text = elemObj[kTIMTextElemContent];
                    OnRecvC2CTextMessage(listener_map, messageID,sender_user_info_json,text);
                } else if (elem_type == TIMElemType::kTIMElem_Custom) { // 自定义元素
                    std::string customData = elemObj[kTIMCustomElemData];
                    OnRecvC2CCustomMessage(listener_map, messageID,sender_user_info_json,customData);
                }
            } else if (conv_type == TIMConvType::kTIMConv_Group) {
                std::string groupID = msg_json[kTIMConvId];
                json::Object sender_member_info_json = msg_json[kTIMMsgSenderGroupMemberInfo];
                if (elem_type == TIMElemType::kTIMElem_Text) {  // 文本
                    std::string text = elemObj[kTIMTextElemContent];
                    OnRecvGroupTextMessage(listener_map, messageID,groupID,sender_member_info_json,text);
                } else if (elem_type == TIMElemType::kTIMElem_Custom) { // 自定义元素
                    std::string customData = elemObj[kTIMCustomElemData];
                    OnRecvGroupCustomMessage(listener_map, messageID,groupID,sender_member_info_json,customData);
                }
            }

        }

        void SimpleListenerJni::OnRecvC2CCustomMessage(const std::map<jobject, jobject> *_listener_map, const std::string &messageId,
                                                       const json::Object &sender, const std::string &customData) {
            if (_listener_map->empty()) {
                return;
            }
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jstring msgIDJni = StringJni::Cstring2Jstring(env, messageId);
            jobject senderJni = UserInfoJni::Convert2JObject(sender);
            jbyteArray customDataJni = StringJni::Cstring2JbyteArray(env,customData);

            for (auto item_listener = _listener_map->begin(); item_listener != _listener_map->end(); item_listener++) {
                env->CallVoidMethod(item_listener->second, j_method_id_array_[MethodIDOnRecvC2CCustomMessage], msgIDJni, senderJni, customDataJni);
            }

            env->DeleteLocalRef(msgIDJni);
            env->DeleteLocalRef(senderJni);
            env->DeleteLocalRef(customDataJni);
        }

        void SimpleListenerJni::OnRecvGroupTextMessage(const std::map<jobject, jobject> *_listener_map, const std::string &messageId,
                                                       const std::string &groupId, const json::Object &sender, const std::string &text) {
            if (_listener_map->empty()) {
                return;
            }
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jstring msgIDJni = StringJni::Cstring2Jstring(env, messageId);
            jstring groupIDJni = StringJni::Cstring2Jstring(env, groupId);
            jobject senderJni = GroupMemberFullInfoJNI::Convert2JObject(sender);
            jstring textJni = StringJni::Cstring2Jstring(env, text);

            for (auto item_listener = _listener_map->begin(); item_listener != _listener_map->end(); ++item_listener) {
                env->CallVoidMethod(item_listener->second, j_method_id_array_[MethodIDOnRecvGroupTextMessage], msgIDJni, groupIDJni, senderJni,
                                    textJni);
            }

        }

        void SimpleListenerJni::OnRecvGroupCustomMessage(const std::map<jobject, jobject> *_listener_map, const std::string &messageId,
                                                         const std::string &groupId, const json::Object &sender, const std::string &customData) {
            if (_listener_map->empty()) {
                return;
            }
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jstring msgIDJni = StringJni::Cstring2Jstring(env, messageId);
            jstring groupIDJni = StringJni::Cstring2Jstring(env, groupId);
            jobject senderJni = GroupMemberFullInfoJNI::Convert2JObject(sender);
            jbyteArray customDataJni = StringJni::Cstring2JbyteArray(env,customData);

            for (auto item_listener = _listener_map->begin(); item_listener != _listener_map->end(); ++item_listener) {
                env->CallVoidMethod(item_listener->second, j_method_id_array_[MethodIDOnRecvGroupTextMessage], msgIDJni, groupIDJni, senderJni,
                                    customDataJni);
            }
        }

    }
}