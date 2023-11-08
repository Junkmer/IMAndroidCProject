//
// Created by Junker on 2022/9/27.
//

#include "message_receipt_jni.h"
#include "jni_helper.h"
#include "java_basic_jni.h"
#include "TIMCloudDef.h"

namespace tim {
    namespace jni {

        jclass MessageReceiptJni::j_cls_;
        jfieldID MessageReceiptJni::j_field_array_[FieldIDMax];
        jmethodID MessageReceiptJni::j_method_id_array[MethodIDMax];

        bool MessageReceiptJni::InitIDs(JNIEnv *env) {
            if (nullptr != j_cls_) {
                return true;
            }

            jclass cls = env->FindClass("com/tencent/imsdk/v2/V2TIMMessageReceipt");
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
            j_method_id_array[MethodIDConstructor] = jmethod;

            jfieldID jfield = nullptr;

            jfield = env->GetFieldID(j_cls_, "msgID", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDMsgID] = jfield;

            jfield = env->GetFieldID(j_cls_, "userID", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDUserID] = jfield;

            jfield = env->GetFieldID(j_cls_, "peerRead", "Z");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDPeerRead] = jfield;

            jfield = env->GetFieldID(j_cls_, "timestamp", "J");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDTimestamp] = jfield;

            jfield = env->GetFieldID(j_cls_, "groupID", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDGroupID] = jfield;

            jfield = env->GetFieldID(j_cls_, "readCount", "J");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDReadCount] = jfield;

            jfield = env->GetFieldID(j_cls_, "unreadCount", "J");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDUnreadCount] = jfield;

            return true;
        }

        jobject MessageReceiptJni::Convert2JObject(const json::Object &messageReceipt_json) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return nullptr;
            }

            jobject messageReceiptObj = env->NewObject(j_cls_, j_method_id_array[MethodIDConstructor]);
            if (nullptr == messageReceiptObj) {
                return nullptr;
            }

            jstring jStr = nullptr;

            jStr = StringJni::Cstring2Jstring(env, messageReceipt_json[kTIMMsgReceiptMsgId]);
            if (jStr) {
                env->SetObjectField(messageReceiptObj, j_field_array_[FieldIDMsgID], jStr);
                env->DeleteLocalRef(jStr);
            }

            jStr = StringJni::Cstring2Jstring(env, messageReceipt_json[kTIMMsgReceiptConvId]);
            if (jStr) {
                bool isGroup = messageReceipt_json[kTIMMsgReceiptConvType].ToInt() == TIMConvType::kTIMConv_Group;
                env->SetObjectField(messageReceiptObj, isGroup?j_field_array_[FieldIDGroupID]:j_field_array_[FieldIDUserID], jStr);
                env->DeleteLocalRef(jStr);
            }

            env->SetBooleanField(messageReceiptObj, j_field_array_[FieldIDPeerRead], messageReceipt_json[kTIMMsgReceiptIsPeerRead].ToBool());
            env->SetLongField(messageReceiptObj, j_field_array_[FieldIDTimestamp], messageReceipt_json[kTIMMsgReceiptTimeStamp].ToInt64());
            env->SetLongField(messageReceiptObj, j_field_array_[FieldIDReadCount], messageReceipt_json[kTIMMsgReceiptReadCount].ToInt64());
            env->SetLongField(messageReceiptObj, j_field_array_[FieldIDUnreadCount], messageReceipt_json[kTIMMsgReceiptUnreadCount].ToInt64());
            return messageReceiptObj;
        }
    }
}