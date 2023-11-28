//
// Created by EDY on 2022/9/28.
//

#include "friend_application_jni.h"
#include "jni_helper.h"
#include "java_basic_jni.h"
#include "TIMFriendshipManager.h"

namespace tim {
    namespace jni {
        jclass FriendApplicationJni::j_cls_;
        jfieldID FriendApplicationJni::j_field_id_array_[FieldIDMax];
        jmethodID FriendApplicationJni::j_method_id_array_[MethodIDMax];

        bool FriendApplicationJni::InitIDs(JNIEnv *env) {
            if (nullptr != j_cls_) {
                return true;
            }

            jclass cls = env->FindClass("com/tencent/imsdk/v2/V2TIMFriendApplication");
            if (nullptr == cls) {
                return false;
            }
            j_cls_ = (jclass) env->NewGlobalRef(cls);
            env->DeleteLocalRef(cls);

            jmethodID jmethod = nullptr;

            jmethod = env->GetMethodID(j_cls_, "<init>", "()V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDConstruct] = jmethod;

            jfieldID jfield = nullptr;

            jfield = env->GetFieldID(j_cls_, "userID", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_id_array_[FieldIDUserID] = jfield;

            jfield = env->GetFieldID(j_cls_, "nickname", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_id_array_[FieldIDNickname] = jfield;

            jfield = env->GetFieldID(j_cls_, "faceUrl", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_id_array_[FieldIDFaceUrl] = jfield;

            jfield = env->GetFieldID(j_cls_, "addTime", "J");
            if (nullptr == jfield) {
                return false;
            }
            j_field_id_array_[FieldIDAddTime] = jfield;

            jfield = env->GetFieldID(j_cls_, "addSource", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_id_array_[FieldIDAddSource] = jfield;

            jfield = env->GetFieldID(j_cls_, "addWording", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_id_array_[FieldIDAddWording] = jfield;

            jfield = env->GetFieldID(j_cls_, "type", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_id_array_[FieldIDType] = jfield;

            return true;
        }

        jobject FriendApplicationJni::Convert2JObject(const json::Object &application_json) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return nullptr;
            }

            jobject applicationObj = env->NewObject(j_cls_, j_method_id_array_[MethodIDConstruct]);
            if (nullptr == applicationObj) {
                return nullptr;
            }

            jstring jStr = nullptr;

            jStr = StringJni::Cstring2Jstring(env, application_json[kTIMFriendAddPendencyInfoIdentifier]);
            if (jStr) {
                env->SetObjectField(applicationObj, j_field_id_array_[FieldIDUserID], jStr);
                env->DeleteLocalRef(jStr);
            }

            jStr = StringJni::Cstring2Jstring(env, application_json[kTIMFriendAddPendencyInfoNickName]);
            if (jStr) {
                env->SetObjectField(applicationObj, j_field_id_array_[FieldIDNickname], jStr);
                env->DeleteLocalRef(jStr);
            }

            //TODO::拉取好友申请未决列表，缺少用户头像，待IM C SDK适配
//            //缺少用户头像
//            jStr = StringJni::Cstring2Jstring(env, application_json[kTIMFriendAddPendencyInfoFaceUrl]);
//            if (jStr) {
//                env->SetObjectField(applicationObj, j_field_id_array_[FieldIDFaceUrl], jStr);
//                env->DeleteLocalRef(jStr);
//            }

            env->SetLongField(applicationObj, j_field_id_array_[FieldIDAddTime], application_json[kTIMFriendAddPendencyInfoAddTime].ToInt64());

            jStr = StringJni::Cstring2Jstring(env, application_json[kTIMFriendAddPendencyInfoAddSource]);
            if (jStr) {
                env->SetObjectField(applicationObj, j_field_id_array_[FieldIDAddSource], jStr);
                env->DeleteLocalRef(jStr);
            }

            jStr = StringJni::Cstring2Jstring(env, application_json[kTIMFriendAddPendencyInfoAddWording]);
            if (jStr) {
                env->SetObjectField(applicationObj, j_field_id_array_[FieldIDAddWording], jStr);
                env->DeleteLocalRef(jStr);
            }

            auto type = TIMFriendPendencyType(application_json[kTIMFriendAddPendencyInfoType].ToInt());
            env->SetIntField(applicationObj, j_field_id_array_[FieldIDType], CPendencyType2JApplicationType(type));

            return applicationObj;
        }

        bool FriendApplicationJni::Convert2CoreObject_Request(const jobject &j_obj_application,json::Object &application_json) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return false;
            }
            jstring jStr = nullptr;

            jStr = (jstring) env->GetObjectField(j_obj_application, j_field_id_array_[FieldIDUserID]);
            if (jStr) {
                application_json[kTIMFriendResponseIdentifier] = StringJni::Jstring2Cstring(env, jStr).c_str();
                env->DeleteLocalRef(jStr);
            }
            return true;
        }

        bool FriendApplicationJni::Convert2CoreObject_Delete(jobject const &j_obj_application, std::string &userid) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return false;
            }
            jstring jStr = nullptr;

            jStr = (jstring) env->GetObjectField(j_obj_application, j_field_id_array_[FieldIDUserID]);
            if (jStr) {
                userid = StringJni::Jstring2Cstring(env, jStr);
                env->DeleteLocalRef(jStr);
            }
            return true;
        }

        int FriendApplicationJni::CPendencyType2JApplicationType(TIMFriendPendencyType cPendencyType) {
            //im c sdk中的好友申请未决类型  转换 im native sdk 中的好友申请未决类型
            switch (cPendencyType) {
                case FriendPendencyTypeComeIn:
                    return JavaApplicationType::V2TIM_FRIEND_APPLICATION_COME_IN;
                case FriendPendencyTypeSendOut:
                    return JavaApplicationType::V2TIM_FRIEND_APPLICATION_SEND_OUT;
                case FriendPendencyTypeBoth:
                    return JavaApplicationType::V2TIM_FRIEND_APPLICATION_BOTH;
                default:
                    return JavaApplicationType::V2TIM_FRIEND_APPLICATION_COME_IN;
            }
        }
    }// namespace jni
}// namespace tim