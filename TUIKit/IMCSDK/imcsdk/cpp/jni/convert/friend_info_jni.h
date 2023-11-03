//
// Created by EDY on 2022/9/28.
//

#ifndef IMCPLUSPLUSDEMO_FRIEND_INFO_JNI_H
#define IMCPLUSPLUSDEMO_FRIEND_INFO_JNI_H

#include <jni.h>
#include "json.h"
#include "json.h"

namespace tim {
    namespace jni {
        class FriendInfoJni {
        public:
            static bool InitIDs(JNIEnv *env);
            static jobject Convert2JObject(const json::Object &friend_info_json);
            static jobject Convert2JObject_UpdateFriendInfoCallback(const json::Object &friend_info_item_json);
            static bool Convert2CoreObject(const jobject &j_obj_friendInfo, json::Object &json_modify_friend_profile_item);

        private:
            enum FieldID {
                FieldIDUserID = 0,
                FieldIDFriendRemark,
                FieldIDFriendAddTime,
                FieldIDFriendGroups,
                FieldIDFriendCustomInfo,
                FieldIDUserFullInfo,

                FieldIDMax,
            };

            enum MethodID {
                MethodIDConstruct = 0,
                MethodIDAddFriendGroup,
                MethodIDPutFriendCustomInfo,

                MethodIDMax,
            };

            static jclass j_cls_;
            static jfieldID j_field_id_array_[FieldIDMax];
            static jmethodID j_method_id_array_[MethodIDMax];
        };
    }// namespace tim
}// namespace jni


#endif //IMCPLUSPLUSDEMO_FRIEND_INFO_JNI_H
