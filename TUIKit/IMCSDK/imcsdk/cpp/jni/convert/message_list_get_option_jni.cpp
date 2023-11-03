//
// Created by EDY on 2022/10/8.
//

#include "message_list_get_option_jni.h"
#include "jni_helper.h"
#include "java_basic_jni.h"
#include "message_jni.h"
#include "LogUtil.h"
#include "TIMCloudDef.h"

namespace tim {
    namespace jni {
        jclass MessageListGetOptionJni::j_cls_;
        jfieldID MessageListGetOptionJni::j_field_array_[FieldIDMax];
        jmethodID MessageListGetOptionJni::j_method_id_array_[MethodIDMax];

        bool MessageListGetOptionJni::InitIDs(JNIEnv *env) {
            if (nullptr != j_cls_) {
                return true;
            }

            jclass cls = env->FindClass("com/tencent/imsdk/v2/V2TIMMessageListGetOption");
            if (nullptr == cls) {
                return false;
            }
            j_cls_ = (jclass) env->NewGlobalRef(cls);

            jmethodID jmethod = nullptr;
            jmethod = env->GetMethodID(j_cls_, "<init>", "()V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDConstructor] = jmethod;

            jfieldID jfield = nullptr;
            jfield = env->GetFieldID(j_cls_, "getType", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDGetType] = jfield;

            jfield = env->GetFieldID(j_cls_, "userID", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDUserID] = jfield;

            jfield = env->GetFieldID(j_cls_, "groupID", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDGroupID] = jfield;

            jfield = env->GetFieldID(j_cls_, "count", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDCount] = jfield;

            jfield = env->GetFieldID(j_cls_, "lastMsg", "Lcom/tencent/imsdk/v2/V2TIMMessage;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDLastMsg] = jfield;

            jfield = env->GetFieldID(j_cls_, "lastMsgSeq", "J");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDLastMsgSeq] = jfield;

            jfield = env->GetFieldID(j_cls_, "getTimeBegin", "J");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDGetTimeBegin] = jfield;

            jfield = env->GetFieldID(j_cls_, "getTimePeriod", "J");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDGetTimePeriod] = jfield;

            jfield = env->GetFieldID(j_cls_, "messageTypeList", "Ljava/util/List;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDMessageTypeList] = jfield;

            return true;
        }

        bool MessageListGetOptionJni::Convert2CoreObject(jobject const &j_obj_getOption,json::Object &get_msg_param_json) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return false;
            }

            /**
             * getType = 1; //拉取云端老消息
             * getType = 2; //拉取云端新消息
             * getType = 3; //拉取本地老消息
             * getType = 4; //拉取本地新消息
             */
            int getType = env->GetIntField(j_obj_getOption, j_field_array_[FieldIDGetType]);
            get_msg_param_json[kTIMMsgGetMsgListParamIsRamble] = getType < 2;
            get_msg_param_json[kTIMMsgGetMsgListParamIsForward] = (getType % 2 == 1);

            jstring jStr = nullptr;
            jStr = (jstring) env->GetObjectField(j_obj_getOption, j_field_array_[FieldIDUserID]);
            if (jStr) {
                std::string userID = StringJni::Jstring2Cstring(env, jStr);
                if (!userID.empty()){
                    get_msg_param_json["convType"] = TIMConvType::kTIMConv_C2C;
                    get_msg_param_json["convID"] = userID;
                } else{
                    get_msg_param_json["convType"] = TIMConvType::kTIMConv_Group;
                    jStr = (jstring) env->GetObjectField(j_obj_getOption, j_field_array_[FieldIDGroupID]);
                    get_msg_param_json["convID"] = StringJni::Jstring2Cstring(env, jStr);
                }
                env->DeleteLocalRef(jStr);
            } else{
                get_msg_param_json["convType"] = TIMConvType::kTIMConv_Group;
                jStr = (jstring) env->GetObjectField(j_obj_getOption, j_field_array_[FieldIDGroupID]);
                if (jStr){
                    get_msg_param_json["convID"] = StringJni::Jstring2Cstring(env, jStr);
                    env->DeleteLocalRef(jStr);
                }
            }

            get_msg_param_json[kTIMMsgGetMsgListParamCount] = env->GetIntField(j_obj_getOption, j_field_array_[FieldIDCount]);

            jobject j_obj_message = env->GetObjectField(j_obj_getOption,j_field_array_[FieldIDLastMsg]);
            if (j_obj_message){
                std::unique_ptr<json::Object> timMessage = tim::jni::MessageJni::Convert2CoreObject(j_obj_message);
                get_msg_param_json[kTIMMsgGetMsgListParamLastMsg] = *timMessage;
                env->DeleteLocalRef(j_obj_message);
            }

            get_msg_param_json[kTIMMsgGetMsgListParamLastMsgSeq] = (long long)env->GetLongField(j_obj_getOption, j_field_array_[FieldIDLastMsgSeq]);
            get_msg_param_json[kTIMMsgGetMsgListParamTimeBegin] = (long long)env->GetLongField(j_obj_getOption, j_field_array_[FieldIDGetTimeBegin]);
            get_msg_param_json[kTIMMsgGetMsgListParamTimePeriod] = (long long)env->GetLongField(j_obj_getOption, j_field_array_[FieldIDGetTimePeriod]);

            jobject j_obj_messageTypeList = env->GetObjectField(j_obj_getOption,j_field_array_[FieldIDMessageTypeList]);
            if (j_obj_messageTypeList){
                int size = ArrayListJni::Size(j_obj_messageTypeList);
                for (int i = 0; i < size; ++i) {
                    int messageType = IntegerJni::IntValue(ArrayListJni::Get(j_obj_messageTypeList,i));
                    get_msg_param_json[kTIMMsgGetMsgListMessageTypeArray].ToArray().push_back(messageType);
                }
                env->DeleteLocalRef(j_obj_messageTypeList);
            }
            return true;
        }

    }// namespace jni
}// namespace tim