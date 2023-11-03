//
// Created by EDY on 2022/9/30.
//

#include "group_info_jni.h"
#include "jni_helper.h"
#include "java_basic_jni.h"
#include "TIMCloudDef.h"

namespace tim {
    namespace jni {
        jclass GroupInfoJni::j_cls_;
        jfieldID GroupInfoJni::j_field_array_[FieldIDMax];
        jmethodID GroupInfoJni::j_method_id_array_[MethodIDMax];


        bool GroupInfoJni::InitIDs(JNIEnv *env) {
            if (nullptr != j_cls_) {
                return true;
            }

            jclass cls = env->FindClass("com/tencent/imsdk/v2/V2TIMGroupInfo");
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
            jfield = env->GetFieldID(j_cls_, "groupID", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDGroupID] = jfield;

            jfield = env->GetFieldID(j_cls_, "groupType", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDGroupType] = jfield;

            jfield = env->GetFieldID(j_cls_, "supportTopic", "Z");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDSupportTopic] = jfield;

            jfield = env->GetFieldID(j_cls_, "groupName", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDGroupName] = jfield;

            jfield = env->GetFieldID(j_cls_, "notification", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDNotification] = jfield;

            jfield = env->GetFieldID(j_cls_, "introduction", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDIntroduction] = jfield;

            jfield = env->GetFieldID(j_cls_, "faceUrl", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDFaceUrl] = jfield;

            jfield = env->GetFieldID(j_cls_, "isAllMuted", "Z");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDIsAllMuted] = jfield;

            jfield = env->GetFieldID(j_cls_, "owner", "Ljava/lang/String;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDOwner] = jfield;

            jfield = env->GetFieldID(j_cls_, "createTime", "J");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDCreateTime] = jfield;


            jfield = env->GetFieldID(j_cls_, "groupAddOpt", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDGroupAddOpt] = jfield;

            jfield = env->GetFieldID(j_cls_, "groupApproveOpt", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDGroupApproveOpt] = jfield;

            jfield = env->GetFieldID(j_cls_, "customInfo", "Ljava/util/Map;");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDCustomInfo] = jfield;

            jfield = env->GetFieldID(j_cls_, "lastInfoTime", "J");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDLastInfoTime] = jfield;

            jfield = env->GetFieldID(j_cls_, "lastMessageTime", "J");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDLastMessageTime] = jfield;

            jfield = env->GetFieldID(j_cls_, "memberCount", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDMemberCount] = jfield;

            jfield = env->GetFieldID(j_cls_, "onlineCount", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDOnlineCount] = jfield;

            jfield = env->GetFieldID(j_cls_, "memberMaxCount", "J");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDMemberMaxCount] = jfield;

            jfield = env->GetFieldID(j_cls_, "role", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDRole] = jfield;

            jfield = env->GetFieldID(j_cls_, "recvOpt", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDRecvOpt] = jfield;

            jfield = env->GetFieldID(j_cls_, "joinTime", "J");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDJoinTime] = jfield;

            jfield = env->GetFieldID(j_cls_, "modifyFlag", "I");
            if (nullptr == jfield) {
                return false;
            }
            j_field_array_[FieldIDModifyFlag] = jfield;

            return true;
        }

        jobject GroupInfoJni::Convert2JObject(const json::Object &groupInfo_json) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return nullptr;
            }

            jobject jObj = env->NewObject(j_cls_, j_method_id_array_[MethodIDConstructor]);
            if (!jObj) {
                return nullptr;
            }

            env->SetObjectField(jObj, j_field_array_[FieldIDGroupID], StringJni::Cstring2Jstring(env, groupInfo_json[kTIMGroupDetailInfoGroupId]));
            env->SetObjectField(jObj, j_field_array_[FieldIDGroupType], StringJni::Cstring2Jstring(env, groupInfo_json[kTIMGroupDetailInfoGroupType]));
            env->SetBooleanField(jObj, j_field_array_[FieldIDSupportTopic], groupInfo_json[kTIMGroupDetailInfoIsSupportTopic].ToBool());
            env->SetObjectField(jObj, j_field_array_[FieldIDGroupName], StringJni::Cstring2Jstring(env, groupInfo_json[kTIMGroupDetailInfoGroupName]));
            env->SetObjectField(jObj, j_field_array_[FieldIDNotification], StringJni::Cstring2Jstring(env, groupInfo_json[kTIMGroupDetailInfoNotification]));
            env->SetObjectField(jObj, j_field_array_[FieldIDIntroduction], StringJni::Cstring2Jstring(env, groupInfo_json[kTIMGroupDetailInfoIntroduction]));
            env->SetObjectField(jObj, j_field_array_[FieldIDFaceUrl], StringJni::Cstring2Jstring(env, groupInfo_json[kTIMGroupDetailInfoFaceUrl]));
            env->SetBooleanField(jObj, j_field_array_[FieldIDIsAllMuted], groupInfo_json[kTIMGroupDetailInfoIsShutupAll].ToBool());
            env->SetObjectField(jObj, j_field_array_[FieldIDOwner], StringJni::Cstring2Jstring(env, groupInfo_json[kTIMGroupDetailInfoOwnerIdentifier]));
            env->SetLongField(jObj, j_field_array_[FieldIDCreateTime], groupInfo_json[kTIMGroupDetailInfoCreateTime].ToInt64());
            env->SetIntField(jObj, j_field_array_[FieldIDGroupAddOpt], groupInfo_json[kTIMGroupDetailInfoAddOption]);
            env->SetIntField(jObj, j_field_array_[FieldIDGroupApproveOpt], groupInfo_json[kTIMGroupDetailInfoApproveOption]);

            if (groupInfo_json.HasKey(kTIMGroupDetailInfoCustomInfo)){
                jobject mapObj = HashMapJni::NewHashMap();
                json::Array custom_array = groupInfo_json[kTIMGroupDetailInfoCustomInfo];
                for (int i = 0; i < custom_array.size(); ++i) {
                    json::Object item = custom_array[i];
                    jstring keyStr = StringJni::Cstring2Jstring(env, item[kTIMGroupInfoCustomStringInfoKey].ToString());
                    if (keyStr){
                        jobject valueByte = StringJni::Cstring2JbyteArray(env,item[kTIMGroupInfoCustomStringInfoValue].ToString());
                        if (valueByte){
                            HashMapJni::Put(mapObj,keyStr,valueByte);
                            env->DeleteLocalRef(valueByte);
                        }
                        env->DeleteLocalRef(keyStr);
                    }
                }
                env->SetObjectField(jObj,j_field_array_[FieldIDCustomInfo],mapObj);
                env->DeleteLocalRef(mapObj);
            }

            env->SetLongField(jObj, j_field_array_[FieldIDLastInfoTime], groupInfo_json[kTIMGroupDetailInfoLastInfoTime].ToInt64());
            env->SetLongField(jObj, j_field_array_[FieldIDLastMessageTime], groupInfo_json[kTIMGroupDetailInfoLastMsgTime].ToInt64());
            env->SetIntField(jObj, j_field_array_[FieldIDMemberCount], groupInfo_json[kTIMGroupDetailInfoMemberNum]);
            env->SetIntField(jObj, j_field_array_[FieldIDOnlineCount], groupInfo_json[kTIMGroupDetailInfoOnlineMemberNum]);
            env->SetLongField(jObj, j_field_array_[FieldIDMemberMaxCount], groupInfo_json[kTIMGroupDetailInfoMaxMemberNum].ToInt64());
            //TODO::IM C SDK 获取群资料时，没有群成员角色、消息接收选项、加群时间字段，待完善
//            //暂不支持该字段
//            env->SetIntField(jObj, j_field_array_[FieldIDRole], groupInfo_json[kTIMGroupDetailInfo]);
//            env->SetIntField(jObj, j_field_array_[FieldIDRecvOpt], groupInfo_json[kTIMGroupDetailInfoApproveOption]);
//            env->SetLongField(jObj, j_field_array_[FieldIDJoinTime], groupInfo_json[kTIMGroupDetailInfo]);
            return jObj;
        }

        bool GroupInfoJni::Convert2CoreObject_Create(jobject const &j_obj_groupInfo, json::Object &create_group_info_json) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return false;
            }

            jstring jStr = nullptr;

            jStr = (jstring) env->GetObjectField(j_obj_groupInfo, j_field_array_[FieldIDGroupID]);
            if (jStr) {
                create_group_info_json[kTIMCreateGroupParamGroupId] = StringJni::Jstring2Cstring(env, jStr).c_str();
                env->DeleteLocalRef(jStr);
            }

            jStr = (jstring) env->GetObjectField(j_obj_groupInfo, j_field_array_[FieldIDGroupType]);
            if (jStr) {
                std::string groupTypeStr = tim::jni::StringJni::Jstring2Cstring(env, jStr);
                if (groupTypeStr == "Work") {
                    create_group_info_json[kTIMCreateGroupParamGroupType] = TIMGroupType::kTIMGroup_Private;
                } else if (groupTypeStr == "Public") {
                    create_group_info_json[kTIMCreateGroupParamGroupType] = TIMGroupType::kTIMGroup_Public;
                } else if (groupTypeStr == "Meeting") {
                    create_group_info_json[kTIMCreateGroupParamGroupType] = TIMGroupType::kTIMGroup_ChatRoom;
                } else if (groupTypeStr == "AVChatRoom") {
                    create_group_info_json[kTIMCreateGroupParamGroupType] = TIMGroupType::kTIMGroup_AVChatRoom;
                } else if (groupTypeStr == "Community") {
                    create_group_info_json[kTIMCreateGroupParamGroupType] = TIMGroupType::kTIMGroup_Community;
                }
                env->DeleteLocalRef(jStr);
            }

            create_group_info_json[kTIMCreateGroupIsSupportTopic] = env->GetBooleanField(j_obj_groupInfo, j_field_array_[FieldIDSupportTopic]);

            jStr = (jstring) env->GetObjectField(j_obj_groupInfo, j_field_array_[FieldIDGroupName]);
            if (jStr) {
                create_group_info_json[kTIMCreateGroupParamGroupName] = StringJni::Jstring2Cstring(env, jStr).c_str();
                env->DeleteLocalRef(jStr);
            }

            jStr = (jstring) env->GetObjectField(j_obj_groupInfo, j_field_array_[FieldIDNotification]);
            if (jStr) {
                create_group_info_json[kTIMCreateGroupParamNotification] = StringJni::Jstring2Cstring(env, jStr).c_str();
                env->DeleteLocalRef(jStr);
            }

            jStr = (jstring) env->GetObjectField(j_obj_groupInfo, j_field_array_[FieldIDIntroduction]);
            if (jStr) {
                create_group_info_json[kTIMCreateGroupParamIntroduction] = StringJni::Jstring2Cstring(env, jStr).c_str();
                env->DeleteLocalRef(jStr);
            }

            jStr = (jstring) env->GetObjectField(j_obj_groupInfo, j_field_array_[FieldIDFaceUrl]);
            if (jStr) {
                create_group_info_json[kTIMCreateGroupParamFaceUrl] = StringJni::Jstring2Cstring(env, jStr).c_str();
                env->DeleteLocalRef(jStr);
            }

            create_group_info_json[kTIMCreateGroupParamAddOption] = TIMGroupAddOption(env->GetIntField(j_obj_groupInfo, j_field_array_[FieldIDGroupAddOpt]));
            create_group_info_json[kTIMCreateGroupParamApproveOption] = TIMGroupAddOption(env->GetIntField(j_obj_groupInfo, j_field_array_[FieldIDGroupAddOpt]));

            jobject j_obj_customHashMap = env->GetObjectField(j_obj_groupInfo,j_field_array_[FieldIDCustomInfo]);
            jobject entry_set = HashMapJni::entrySet(j_obj_customHashMap);
            jobject iterator = HashMapJni::iterator(entry_set);
            while (HashMapJni::hasNext(iterator)) {
                jobject object = HashMapJni::next(iterator);
                if (nullptr == object) {
                    continue;
                }
                auto jStr_key = (jstring) HashMapJni::getKey(object);
                if (nullptr != jStr_key) {
                    auto jByte_value = (jbyteArray) HashMapJni::getValue(object);
                    json::Object item;
                    item[kTIMGroupInfoCustomStringInfoKey] = StringJni::Jstring2Cstring(env, jStr_key).c_str();
                    item[kTIMGroupInfoCustomStringInfoValue] = StringJni::JbyteArray2Cstring(env, jByte_value);
                    create_group_info_json[kTIMCreateGroupParamCustomInfo].ToArray().push_back(item);

                    env->DeleteLocalRef(jByte_value);
                    env->DeleteLocalRef(jStr_key);
                }
            }

            create_group_info_json[kTIMCreateGroupParamMaxMemberCount] = (long long) env->GetLongField(j_obj_groupInfo, j_field_array_[FieldIDMemberMaxCount]);
            return true;
        }

        bool GroupInfoJni::Convert2CoreObject_Update(jobject const &j_obj_groupInfo, json::Object &update_group_info_json) {
            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            if (!InitIDs(env)) {
                return false;
            }

            jstring jStr = nullptr;

            int modifyFlag = 0;

            jStr = (jstring) env->GetObjectField(j_obj_groupInfo, j_field_array_[FieldIDGroupID]);
            if (jStr) {
                modifyFlag = modifyFlag | TIMGroupModifyInfoFlag::kTIMGroupModifyInfoFlag_Name;
                update_group_info_json[kTIMGroupModifyInfoParamGroupId] = StringJni::Jstring2Cstring(env, jStr).c_str();
                env->DeleteLocalRef(jStr);
            }

            jStr = (jstring) env->GetObjectField(j_obj_groupInfo, j_field_array_[FieldIDGroupName]);
            if (jStr) {
                modifyFlag = modifyFlag | TIMGroupModifyInfoFlag::kTIMGroupModifyInfoFlag_Name;
                update_group_info_json[kTIMGroupModifyInfoParamGroupName] = StringJni::Jstring2Cstring(env, jStr).c_str();
                env->DeleteLocalRef(jStr);
            }

            jStr = (jstring) env->GetObjectField(j_obj_groupInfo, j_field_array_[FieldIDNotification]);
            if (jStr) {
                modifyFlag = modifyFlag | TIMGroupModifyInfoFlag::kTIMGroupModifyInfoFlag_Notification;
                update_group_info_json[kTIMGroupModifyInfoParamNotification] = StringJni::Jstring2Cstring(env, jStr).c_str();
                env->DeleteLocalRef(jStr);
            }

            jStr = (jstring) env->GetObjectField(j_obj_groupInfo, j_field_array_[FieldIDIntroduction]);
            if (jStr) {
                modifyFlag = modifyFlag | TIMGroupModifyInfoFlag::kTIMGroupModifyInfoFlag_Introduction;
                update_group_info_json[kTIMGroupModifyInfoParamIntroduction] = StringJni::Jstring2Cstring(env, jStr).c_str();
                env->DeleteLocalRef(jStr);
            }

            jStr = (jstring) env->GetObjectField(j_obj_groupInfo, j_field_array_[FieldIDFaceUrl]);
            if (jStr) {
                modifyFlag = modifyFlag | TIMGroupModifyInfoFlag::kTIMGroupModifyInfoFlag_FaceUrl;
                update_group_info_json[kTIMGroupModifyInfoParamFaceUrl] = StringJni::Jstring2Cstring(env, jStr).c_str();
                env->DeleteLocalRef(jStr);
            }

            update_group_info_json[kTIMGroupModifyInfoParamIsShutupAll] = env->GetBooleanField(j_obj_groupInfo, j_field_array_[FieldIDIsAllMuted]);
            modifyFlag = modifyFlag | TIMGroupModifyInfoFlag::kTIMGroupModifyInfoFlag_ShutupAll;

            int addOptType = TIMGroupAddOption(env->GetIntField(j_obj_groupInfo, j_field_array_[FieldIDGroupAddOpt]));
            if (addOptType != -1){
                update_group_info_json[kTIMGroupModifyInfoParamAddOption] = addOptType;
                modifyFlag = modifyFlag | TIMGroupModifyInfoFlag::kTIMGroupModifyInfoFlag_AddOption;
            }

            int approveOptType = TIMGroupAddOption(env->GetIntField(j_obj_groupInfo, j_field_array_[FieldIDGroupApproveOpt]));
            if (approveOptType != -1){
                update_group_info_json[kTIMGroupModifyInfoParamApproveOption] = approveOptType;
                modifyFlag = modifyFlag | TIMGroupModifyInfoFlag::kTIMGroupModifyInfoFlag_ApproveOption;
            }

            jobject j_obj_customHashMap = env->GetObjectField(j_obj_groupInfo,j_field_array_[FieldIDCustomInfo]);
            jobject entry_set = HashMapJni::entrySet(j_obj_customHashMap);
            jobject iterator = HashMapJni::iterator(entry_set);
            bool updateCustomFlag;
            while (HashMapJni::hasNext(iterator)) {
                jobject object = HashMapJni::next(iterator);
                if (nullptr == object) {
                    continue;
                }
                updateCustomFlag = true;
                auto jStr_key = (jstring) HashMapJni::getKey(object);
                if (nullptr != jStr_key) {
                    auto jByte_value = (jbyteArray) HashMapJni::getValue(object);
                    json::Object item;
                    item[kTIMGroupInfoCustomStringInfoKey] = StringJni::Jstring2Cstring(env, jStr_key).c_str();
                    item[kTIMGroupInfoCustomStringInfoValue] = StringJni::JbyteArray2Cstring(env, jByte_value);
                    update_group_info_json[kTIMGroupModifyInfoParamCustomInfo].ToArray().push_back(item);

                    env->DeleteLocalRef(jByte_value);
                    env->DeleteLocalRef(jStr_key);
                }
            }

            if (updateCustomFlag){
                modifyFlag = modifyFlag | TIMGroupModifyInfoFlag::kTIMGroupModifyInfoFlag_Custom;
            }

            long memberMaxCount = env->GetLongField(j_obj_groupInfo, j_field_array_[FieldIDMemberMaxCount]);
            if (memberMaxCount != -1){
                update_group_info_json[kTIMGroupModifyInfoParamMaxMemberNum] = (long long) memberMaxCount;
                modifyFlag = modifyFlag | TIMGroupModifyInfoFlag::kTIMGroupModifyInfoFlag_MaxMemberNum;
            }

            update_group_info_json[kTIMGroupModifyInfoParamModifyFlag] = modifyFlag;
            return true;
        }
    }//namespace jni
}//namespace tim