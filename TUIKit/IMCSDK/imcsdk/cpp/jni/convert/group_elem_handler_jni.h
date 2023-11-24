//
// Created by Junker on 2022/9/22.
//

#ifndef IMCPLUSPLUSDEMO_GROUP_ELEM_HANDLER_JNI_H
#define IMCPLUSPLUSDEMO_GROUP_ELEM_HANDLER_JNI_H

#include <jni.h>

#include "elem_handler_jni.h"
#include "TIMGroupManager.h"

namespace tim {
    namespace jni {
        class GroupElemHandlerJni : public IElemHandler{
        public:
            std::string GetHandlerType() override;
            bool InitIDs(JNIEnv *env) override;
            jobject Convert2JObject(const json::Object &elem_json) override;
            std::unique_ptr<json::Object> Convert2CoreObject(int elemType, jobject jElemObj) override;
            
        private:
            static TIMGroupTipType JTipType2CTipType(int jTipType);
            static int CTipType2JTipType(TIMGroupTipType cTipType);

        private:
            enum FieldID{
                FieldIDGroupID= 0,
                FieldIDType,
                FieldIDOpMember,
                FieldIDMemberList,
                FieldIDGroupChangeInfoList,
                FieldIDMemberChangeInfoList,
                FieldIDMemberCount,

                FieldIDMax,
            };

            enum MethodID{
                MethodIDConstructor = 0,
                MethodIDAddMember,
                MethodIDAddGroupChangeInfo,
                MethodIDAddMemberChangeInfo,

                MethodIDMax,

            };

            enum JavaTipType {
                GROUP_TIPS_TYPE_INVALID = 0,
                V2TIM_GROUP_TIPS_TYPE_JOIN = 1,
                V2TIM_GROUP_TIPS_TYPE_INVITE = 2,
                V2TIM_GROUP_TIPS_TYPE_QUIT = 3,
                V2TIM_GROUP_TIPS_TYPE_KICKED = 4,
                V2TIM_GROUP_TIPS_TYPE_SET_ADMIN = 5,
                V2TIM_GROUP_TIPS_TYPE_CANCEL_ADMIN = 6,
                V2TIM_GROUP_TIPS_TYPE_GROUP_INFO_CHANGE = 7,
                V2TIM_GROUP_TIPS_TYPE_MEMBER_INFO_CHANGE = 8,
            };

            jfieldID j_field_array_[FieldIDMax];
            jmethodID j_method_id_array_[MethodIDMax];
        };
    }// namespace jni
}// namespace tim




#endif //IMCPLUSPLUSDEMO_GROUP_ELEM_HANDLER_JNI_H
