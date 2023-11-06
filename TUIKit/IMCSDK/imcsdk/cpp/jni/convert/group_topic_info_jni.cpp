//
// Created by Junker on 2022/9/23.
//

#include <jni_helper.h>
#include "group_topic_info_jni.h"
#include "java_basic_jni.h"
#include "message_jni.h"
#include "group_at_info_jni.h"
#include "TIMCloudDef.h"

namespace tim {
    namespace jni {
        jclass GroupTopicInfoJni::j_cls_;
        jfieldID GroupTopicInfoJni::j_field_id_array_[FieldIDMax];
        jmethodID GroupTopicInfoJni::j_method_id_array_[MethodIDMax];

        bool GroupTopicInfoJni::InitIDs(JNIEnv *env) {
            if (j_cls_ != nullptr) {
                return true;
            }
            jclass cls = env->FindClass("com/tencent/imsdk/v2/V2TIMTopicInfo");
            if (cls == nullptr) {
                return false;
            }
            j_cls_ = (jclass) env->NewGlobalRef(cls);
            env->DeleteLocalRef(cls);

            jmethodID jmethod = nullptr;

            jmethod = env->GetMethodID(j_cls_, "<init>", "()V");
            if (jmethod == nullptr) {
                return false;
            }
            j_method_id_array_[MethodIDConstruct] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "addGroupAtInfo", "(Lcom/tencent/imsdk/v2/V2TIMGroupAtInfo;)V");
            if (jmethod == nullptr) {
                return false;
            }
            j_method_id_array_[MethodIDAddGroupAtInfo] = jmethod;

            jfieldID jfield = nullptr;

            jfield = env->GetFieldID(j_cls_, "topicID", "Ljava/lang/String;");
            if (jfield == nullptr) {
                return false;
            }
            j_field_id_array_[FieldIDTopicID] = jfield;
            
            jfield = env->GetFieldID(j_cls_, "topicName", "Ljava/lang/String;");
            if (jfield == nullptr) {
                return false;
            }
            j_field_id_array_[FieldIDTopicName] = jfield;
            
            jfield = env->GetFieldID(j_cls_, "topicFaceUrl", "Ljava/lang/String;");
            if (jfield == nullptr) {
                return false;
            }
            j_field_id_array_[FieldIDTopicFaceUrl] = jfield;
            
            jfield = env->GetFieldID(j_cls_, "introduction", "Ljava/lang/String;");
            if (jfield == nullptr) {
                return false;
            }
            j_field_id_array_[FieldIDIntroduction] = jfield;

            jfield = env->GetFieldID(j_cls_, "notification", "Ljava/lang/String;");
            if (jfield == nullptr) {
                return false;
            }
            j_field_id_array_[FieldIDNotification] = jfield;

            jfield = env->GetFieldID(j_cls_, "allMute", "Z");
            if (jfield == nullptr) {
                return false;
            }
            j_field_id_array_[FieldIDAllMute] = jfield;

            jfield = env->GetFieldID(j_cls_, "selfMuteTime", "J");
            if (jfield == nullptr) {
                return false;
            }
            j_field_id_array_[FieldIDSelfMuteTime] = jfield;

            jfield = env->GetFieldID(j_cls_, "customString", "Ljava/lang/String;");
            if (jfield == nullptr) {
                return false;
            }
            j_field_id_array_[FieldIDCustomString] = jfield;

            jfield = env->GetFieldID(j_cls_, "recvOpt", "I");
            if (jfield == nullptr) {
                return false;
            }
            j_field_id_array_[FieldIDRecvOpt] = jfield;
            
            jfield = env->GetFieldID(j_cls_, "draftText", "Ljava/lang/String;");
            if (jfield == nullptr) {
                return false;
            }
            j_field_id_array_[FieldIDDraftText] = jfield;
            
            jfield = env->GetFieldID(j_cls_, "unreadCount", "J");
            if (jfield == nullptr) {
                return false;
            }
            j_field_id_array_[FieldIDUnreadCount] = jfield;
            
            jfield = env->GetFieldID(j_cls_, "lastMessage", "Lcom/tencent/imsdk/v2/V2TIMMessage;");
            if (jfield == nullptr) {
                return false;
            }
            j_field_id_array_[FieldIDLastMessage] = jfield;
            
            jfield = env->GetFieldID(j_cls_, "groupAtInfoList", "Ljava/util/List;");
            if (jfield == nullptr) {
                return false;
            }
            j_field_id_array_[FieldIDGroupAtInfoList] = jfield;
            
            return true;
        }

        jobject GroupTopicInfoJni::Convert2JObject(const json::Object &topicInfo_json) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return nullptr;
            }

            jobject j_obj_topicInfo = env->NewObject(j_cls_, j_method_id_array_[MethodIDConstruct]);

            jstring jStr = nullptr;

            jStr = StringJni::Cstring2Jstring(env,topicInfo_json[kTIMGroupTopicInfoTopicID]);
            if (jStr){
                env->SetObjectField(j_obj_topicInfo,j_field_id_array_[FieldIDTopicID],jStr);
                env->DeleteLocalRef(jStr);
            }

            jStr = StringJni::Cstring2Jstring(env,topicInfo_json[kTIMGroupTopicInfoTopicName]);
            if (jStr){
                env->SetObjectField(j_obj_topicInfo,j_field_id_array_[FieldIDTopicName],jStr);
                env->DeleteLocalRef(jStr);
            }

            jStr = StringJni::Cstring2Jstring(env,topicInfo_json[kTIMGroupTopicInfoTopicFaceURL]);
            if (jStr){
                env->SetObjectField(j_obj_topicInfo,j_field_id_array_[FieldIDTopicFaceUrl],jStr);
                env->DeleteLocalRef(jStr);
            }

            jStr = StringJni::Cstring2Jstring(env,topicInfo_json[kTIMGroupTopicInfoIntroduction]);
            if (jStr){
                env->SetObjectField(j_obj_topicInfo,j_field_id_array_[FieldIDIntroduction],jStr);
                env->DeleteLocalRef(jStr);
            }

            jStr = StringJni::Cstring2Jstring(env,topicInfo_json[kTIMGroupTopicInfoNotification]);
            if (jStr){
                env->SetObjectField(j_obj_topicInfo,j_field_id_array_[FieldIDNotification],jStr);
                env->DeleteLocalRef(jStr);
            }

            env->SetBooleanField(j_obj_topicInfo,j_field_id_array_[FieldIDAllMute],topicInfo_json[kTIMGroupTopicInfoIsAllMuted].ToBool());
            env->SetLongField(j_obj_topicInfo,j_field_id_array_[FieldIDSelfMuteTime],topicInfo_json[kTIMGroupTopicInfoSelfMuteTime].ToInt64());

            jStr = StringJni::Cstring2Jstring(env,topicInfo_json[kTIMGroupTopicInfoCustomString]);
            if (jStr){
                env->SetObjectField(j_obj_topicInfo,j_field_id_array_[FieldIDCustomString],jStr);
                env->DeleteLocalRef(jStr);
            }

            env->SetIntField(j_obj_topicInfo,j_field_id_array_[FieldIDRecvOpt],topicInfo_json[kTIMGroupTopicInfoRecvOpt]);

            jStr = StringJni::Cstring2Jstring(env,topicInfo_json[kTIMGroupTopicInfoDraftText]);
            if (jStr){
                env->SetObjectField(j_obj_topicInfo,j_field_id_array_[FieldIDDraftText],jStr);
                env->DeleteLocalRef(jStr);
            }

            env->SetLongField(j_obj_topicInfo,j_field_id_array_[FieldIDUnreadCount],topicInfo_json[kTIMGroupTopicInfoUnreadCount].ToInt64());

            if (topicInfo_json.HasKey(kTIMGroupTopicInfoLastMessage)){
                json::Object lastMessage_json = topicInfo_json[kTIMGroupTopicInfoLastMessage];
                if (lastMessage_json.size() > 0){
                    jobject j_obj_lastMessage = MessageJni::Convert2JObject(lastMessage_json);
                    if (j_obj_lastMessage) {
                        env->SetObjectField(j_obj_topicInfo, j_field_id_array_[FieldIDLastMessage], j_obj_lastMessage);
                        env->DeleteLocalRef(j_obj_lastMessage);
                    }
                }
            }

            if (topicInfo_json.HasKey(kTIMGroupTopicInfoGroupAtInfoArray)){
                json::Array at_info_array = topicInfo_json[kTIMGroupTopicInfoGroupAtInfoArray];
                for (int i = 0; i < at_info_array.size(); ++i) {
                    jobject j_obj_atInfo = GroupAtInfoJni::Convert2JObject(at_info_array[i]);
                    if (nullptr != j_obj_atInfo) {
                        env->CallVoidMethod(j_obj_topicInfo,j_method_id_array_[i],j_obj_topicInfo);
                        env->DeleteLocalRef(j_obj_atInfo);
                    }
                }
            }

            return j_obj_topicInfo;
            
        }

        bool GroupTopicInfoJni::Convert2CoreObject(const jobject &object,json::Object &topicInfo_json) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return false;
            }

            jstring jStr = nullptr;
            jStr = (jstring) env->GetObjectField(object, j_field_id_array_[FieldIDTopicID]);
            if (jStr) {
                topicInfo_json[kTIMGroupTopicInfoTopicID] = StringJni::Jstring2Cstring(env, jStr).c_str();
                env->DeleteLocalRef(jStr);
            }

            jStr = (jstring) env->GetObjectField(object, j_field_id_array_[FieldIDTopicName]);
            if (jStr) {
                topicInfo_json[kTIMGroupTopicInfoTopicName] = StringJni::Jstring2Cstring(env, jStr).c_str();
                env->DeleteLocalRef(jStr);
            }

            jStr = (jstring) env->GetObjectField(object, j_field_id_array_[FieldIDTopicFaceUrl]);
            if (jStr) {
                topicInfo_json[kTIMGroupTopicInfoTopicFaceURL] = StringJni::Jstring2Cstring(env, jStr).c_str();
                env->DeleteLocalRef(jStr);
            }

            jStr = (jstring) env->GetObjectField(object, j_field_id_array_[FieldIDIntroduction]);
            if (jStr) {
                topicInfo_json[kTIMGroupTopicInfoIntroduction] = StringJni::Jstring2Cstring(env, jStr).c_str();
                env->DeleteLocalRef(jStr);
            }

            jStr = (jstring) env->GetObjectField(object, j_field_id_array_[FieldIDNotification]);
            if (jStr) {
                topicInfo_json[kTIMGroupTopicInfoNotification] = StringJni::Jstring2Cstring(env, jStr).c_str();
                env->DeleteLocalRef(jStr);
            }

            topicInfo_json[kTIMGroupTopicInfoIsAllMuted] = env->GetBooleanField(object,j_field_id_array_[FieldIDAllMute]);
            topicInfo_json[kTIMGroupTopicInfoSelfMuteTime] = (long long )env->GetLongField(object,j_field_id_array_[FieldIDSelfMuteTime]);


            jStr = (jstring) env->GetObjectField(object, j_field_id_array_[FieldIDCustomString]);
            if (jStr) {
                topicInfo_json[kTIMGroupTopicInfoCustomString] = StringJni::Jstring2Cstring(env, jStr).c_str();
                env->DeleteLocalRef(jStr);
            }

            topicInfo_json[kTIMGroupTopicInfoRecvOpt] = TIMReceiveMessageOpt(env->GetIntField(object,j_field_id_array_[FieldIDRecvOpt]));

            jStr = (jstring) env->GetObjectField(object, j_field_id_array_[FieldIDDraftText]);
            if (jStr) {
                topicInfo_json[kTIMGroupTopicInfoDraftText] = StringJni::Jstring2Cstring(env, jStr).c_str();
                env->DeleteLocalRef(jStr);
            }

            topicInfo_json[kTIMGroupTopicInfoUnreadCount] = (long long)env->GetLongField(object,j_field_id_array_[FieldIDUnreadCount]);

            jobject j_obj = nullptr;

            j_obj = env->GetObjectField(object, j_field_id_array_[FieldIDLastMessage]);
            if (j_obj) {
                std::unique_ptr<json::Object> timMessage = tim::jni::MessageJni::Convert2CoreObject(j_obj);
                topicInfo_json[kTIMGroupTopicInfoLastMessage] = *timMessage;
                env->DeleteLocalRef(j_obj);
            }

            j_obj = env->GetObjectField(object, j_field_id_array_[FieldIDGroupAtInfoList]);
            if (j_obj) {
                int size = ArrayListJni::Size(j_obj);
                if (size > 0){
                    json::Array at_info_array;
                    for (int i = 0; i < size; ++i) {
                        jobject item_obj = ArrayListJni::Get(j_obj,i);
                        json::Object group_at_json;
                        bool flag = GroupAtInfoJni::Convert2CoreObject(item_obj,group_at_json);
                        if (flag){
                            at_info_array.push_back(group_at_json);
                        }
                    }

                    if (at_info_array.size() > 0){
                        topicInfo_json[kTIMGroupTopicInfoGroupAtInfoArray] = at_info_array;
                    }
                }
                env->DeleteLocalRef(j_obj);
            }

            return true;
        }

    }// namespace jni
}// namespace tim