//
// Created by EDY on 2022/9/20.
//

#ifndef IMCPLUSPLUSDEMO_MESSAGE_JNI_H
#define IMCPLUSPLUSDEMO_MESSAGE_JNI_H

#include <jni.h>
#include <memory>
#include "json.h"
#include "TIMCloudDef.h"

namespace tim {
    namespace jni {
        class MessageJni{
        public:
            static bool InitIDs(JNIEnv *env);
            static jobject Convert2JObject(const json::Object &message_json_obj);
            static std::unique_ptr<json::Object> Convert2CoreObject(jobject &messageObj);

        private:
            static int CElemType2JElemType(TIMElemType cElemType);
            static TIMElemType JElemType2CElemType(int jElemType);

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
            
            enum JavaElemType{
                V2TIM_ELEM_TYPE_NONE = 0,
                V2TIM_ELEM_TYPE_TEXT = 1,
                V2TIM_ELEM_TYPE_CUSTOM = 2,
                V2TIM_ELEM_TYPE_IMAGE = 3,
                V2TIM_ELEM_TYPE_SOUND = 4,
                V2TIM_ELEM_TYPE_VIDEO = 5,
                V2TIM_ELEM_TYPE_FILE = 6,
                V2TIM_ELEM_TYPE_LOCATION = 7,
                V2TIM_ELEM_TYPE_FACE = 8,
                V2TIM_ELEM_TYPE_GROUP_TIPS = 9,
                V2TIM_ELEM_TYPE_MERGER = 10,
            };

            static jclass j_cls_;
            static jfieldID j_filed_id_array[FieldIDMax];
            static jmethodID j_method_id_array[MethodIDMax];
        };
    }// namespace jni
}// namespace tim

#endif //IMCPLUSPLUSDEMO_MESSAGE_JNI_H
