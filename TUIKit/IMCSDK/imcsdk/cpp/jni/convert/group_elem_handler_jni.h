//
// Created by Junker on 2022/9/22.
//

#ifndef IMCPLUSPLUSDEMO_GROUP_ELEM_HANDLER_JNI_H
#define IMCPLUSPLUSDEMO_GROUP_ELEM_HANDLER_JNI_H

#include <jni.h>

#include "elem_handler_jni.h"

namespace tim {
    namespace jni {
        class GroupElemHandlerJni : public IElemHandler{
        public:
            std::string GetHandlerType() override;
            bool InitIDs(JNIEnv *env) override;
            jobject Convert2JObject(const json::Object &elem_json) override;
            std::unique_ptr<json::Object> Convert2CoreObject(int elemType, jobject jElemObj) override;

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

            jfieldID j_field_array_[FieldIDMax];
            jmethodID j_method_id_array_[MethodIDMax];
        };
    }// namespace jni
}// namespace tim




#endif //IMCPLUSPLUSDEMO_GROUP_ELEM_HANDLER_JNI_H
