//
// Created by EDY on 2022/9/30.
//

#ifndef IMCPLUSPLUSDEMO_GROUP_INFO_JNI_H
#define IMCPLUSPLUSDEMO_GROUP_INFO_JNI_H

#include <jni.h>
#include "json.h"

namespace tim {
    namespace jni {
        class GroupInfoJni {
        public:
            static bool InitIDs(JNIEnv *env);
            static jobject Convert2JObject(const json::Object &groupInfo_json);
            static bool Convert2CoreObject_Create(const jobject &j_obj_groupInfo,json::Object &create_group_info_json);
            static bool Convert2CoreObject_Update(const jobject &j_obj_groupInfo,json::Object &update_group_info_json);

        private:
            enum FieldID{
                FieldIDGroupID = 0,
                FieldIDGroupType,
                FieldIDSupportTopic,
                FieldIDGroupName,
                FieldIDNotification,
                FieldIDIntroduction,
                FieldIDFaceUrl,
                FieldIDIsAllMuted,
                FieldIDOwner,
                FieldIDCreateTime,
                FieldIDGroupAddOpt,
                FieldIDGroupApproveOpt,
                FieldIDCustomInfo,
                FieldIDLastInfoTime,
                FieldIDLastMessageTime,
                FieldIDMemberCount,
                FieldIDOnlineCount,
                FieldIDMemberMaxCount,
                FieldIDRole,
                FieldIDRecvOpt,
                FieldIDJoinTime,
                FieldIDModifyFlag,

                FieldIDMax,
            };

            enum MethodID{
                MethodIDConstructor = 0,

                MethodIDMax,
            };

            static jclass j_cls_;
            static jfieldID j_field_array_[FieldIDMax];
            static jmethodID j_method_id_array_[MethodIDMax];
        };
    }//namespace jni
}//namespace tim


#endif //IMCPLUSPLUSDEMO_GROUP_INFO_JNI_H
