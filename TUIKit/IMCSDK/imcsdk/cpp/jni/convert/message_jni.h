//
// Created by EDY on 2022/9/20.
//

#ifndef IMCPLUSPLUSDEMO_MESSAGE_JNI_H
#define IMCPLUSPLUSDEMO_MESSAGE_JNI_H

#include <jni.h>
#include <memory>
#include "json.h"

namespace tim {
    namespace jni {
        class MessageJni{
        public:
            static bool InitIDs(JNIEnv *env);
            static jobject Convert2JObject(const json::Object &message_json_obj);
            static std::unique_ptr<json::Object> Convert2CoreObject(jobject &messageObj);

        private:
            enum FieldID {
                FieldIDMsgID = 0,
                FieldIDTimestamp,
                FieldIDSender,
                FieldIDNickName,
                FieldIDFriendRemark,
                FieldIDFaceUrl,
                FieldIDNameCard,
                FieldIDGroupID,
                FieldIDUserID,
                FieldIDStatus,
                FieldIDElemType,
                FieldIDElemList,

                FieldIDLocalCustomInt,
                FieldIDLocalCustomData,
                FieldIDCloudCustomData,

                FieldIDIsSelf,
                FieldIDIsRead,
                FieldIDIsPeerRead,

                FieldIDNeedReadReceipt,

                FieldIDIsBroadcastMessage,
                FieldIDPriority,
                FieldIDOfflinePushInfo,
                FieldIDGroupAtUserList,
                FieldIDSeq,
                FieldIDRandom,
                FieldIDIsOnlineMsg,
                FieldIDIsExcludedFromUnreadCount,
                FieldIDIsExcludedFromLastMessage,
                FieldIDIsSupportMessageExtension,
                FieldIDIsForwardMessage,
                FieldIDTargetGroupMemberList,

                FieldIDMax,
            };

            enum MethodID{
                MethodIDConstructor = 0,
                MethodIDAddMessageElem,

                MethodIDMax
            };

            static jclass j_cls_;
            static jfieldID j_filed_id_array[FieldIDMax];
            static jmethodID j_method_id_array[MethodIDMax];
        };
    }// namespace jni
}// namespace tim

#endif //IMCPLUSPLUSDEMO_MESSAGE_JNI_H
