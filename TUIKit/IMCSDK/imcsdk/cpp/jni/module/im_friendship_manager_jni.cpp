//
// Created by EDY on 2022/8/22.
//

#include <jni.h>
#include "jni_util.h"
#include "java_basic_jni.h"
#include "tim_engine.h"
#include "jni_helper.h"
#include "friend_listener_jni.h"
#include "tim_callback_impl.h"
#include "value_callback_impl.h"
#include "im_callbak_jni.h"
#include "friend_info_jni.h"
#include "friend_info_result_jni.h"
#include "friend_application_jni.h"
#include "friend_application_result_jni.h"
#include "friend_operation_result_jni.h"
#include "friend_search_param_jni.h"
#include "friend_add_application_jni.h"
#include "friend_check_result_jni.h"
#include "friend_group_jni.h"
#include "LogUtil.h"
#include "json.h"

#define DEFINE_NATIVE_FUNC(RETURN_TYPE, NAME, ...) \
    RETURN_TYPE NAME(JNIEnv *env ,jobject thiz, ##__VA_ARGS__)

#ifdef __cplusplus
extern "C" {
#endif

static std::shared_ptr<tim::jni::FriendListenerJni> friend_callback_jni;

DEFINE_NATIVE_FUNC(void, NativeAddFriendListener, jobject listener, jstring listenerPath) {
    if (!friend_callback_jni) {
        friend_callback_jni = std::make_shared<tim::jni::FriendListenerJni>();
    }

    friend_callback_jni->AddListener(env, listener, listenerPath);
}

DEFINE_NATIVE_FUNC(void, NativeRemoveFriendListener, jstring listenerPath) {
    if (friend_callback_jni) {
        friend_callback_jni->RemoveListener(env, listenerPath);
        if (friend_callback_jni->empty()) {
            friend_callback_jni.reset();
        }
    }
}

DEFINE_NATIVE_FUNC(void, NativeGetFriendList, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    tim::TIMEngine::GetInstance()->GetFriendList([](int32_t code, const char *desc, const char *json_params, const void *user_data) {
        tim::jni::ScopedJEnv scopedJEnv;
        auto *_env = scopedJEnv.GetEnv();
        auto _callback = (jobject) user_data;

        if (TIMErrCode::ERR_SUCC == code) {
            jobject friendInfoList = tim::jni::ArrayListJni::NewArrayList();

            json::Array friend_list_json;
            friend_list_json = json::Deserialize(json_params);

            for (int i = 0; i < friend_list_json.size(); ++i) {
                jobject friendInfoObj = tim::jni::FriendInfoJni::Convert2JObject(friend_list_json[i]);
                tim::jni::ArrayListJni::Add(friendInfoList, friendInfoObj);
                _env->DeleteLocalRef(friendInfoObj);
            }
            tim::jni::IMCallbackJNI::Success(_callback, friendInfoList);
            _env->DeleteLocalRef(friendInfoList);
        } else {
            tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
        }
        _env->DeleteGlobalRef(_callback);
    }, jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeGetFriendsInfo, jobject user_idlist, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    json::Array userIDList;
    int size = tim::jni::ArrayListJni::Size(user_idlist);
    for (int i = 0; i < size; ++i) {
        auto userID_jStr = (jstring) tim::jni::ArrayListJni::Get(user_idlist, i);
        userIDList.push_back(tim::jni::StringJni::Jstring2Cstring(env, userID_jStr).c_str());
        env->DeleteLocalRef(userID_jStr);
    }
    std::string userIDListStr = json::Serialize(userIDList);

    tim::TIMEngine::GetInstance()->GetFriendsInfo(userIDListStr.c_str(),[](int32_t code, const char* desc, const char* json_params, const void* user_data){
        tim::jni::ScopedJEnv scopedJEnv;
        auto _env = scopedJEnv.GetEnv();
        auto _callback = (jobject) user_data;

        if (TIMErrCode::ERR_SUCC == code) {
            json::Array result_array = json::Deserialize(json_params);
            jobject friendInfoResultList = tim::jni::ArrayListJni::NewArrayList();
            for (int i = 0; i < result_array.size(); ++i) {
                jobject friendInfoResultObj = tim::jni::FriendInfoResultJni::Convert2JObject(result_array[i]);
                tim::jni::ArrayListJni::Add(friendInfoResultList, friendInfoResultObj);
                _env->DeleteLocalRef(friendInfoResultObj);
            }
            tim::jni::IMCallbackJNI::Success(_callback, friendInfoResultList);
            _env->DeleteLocalRef(friendInfoResultList);

        } else {
            tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
        }
        _env->DeleteGlobalRef(_callback);
    }, jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeSetFriendInfo, jobject info, jobject callback) {
    json::Object info_c;
    tim::jni::FriendInfoJni::Convert2CoreObject(info, info_c);

    tim::TIMEngine::GetInstance()->SetFriendInfo(json::Serialize(info_c).c_str(), new tim::TIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeSearchFriends, jobject search_param, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    json::Object searchParam;
    tim::jni::FriendSearchParamJni::Convert2CoreObject(search_param, searchParam);
    std::string searchParamStr = json::Serialize(searchParam);

    tim::TIMEngine::GetInstance()->SearchFriends(searchParamStr.c_str(),[](int32_t code, const char* desc, const char* json_params, const void* user_data){
        tim::jni::ScopedJEnv scopedJEnv;
        auto _env = scopedJEnv.GetEnv();
        auto _callback = (jobject) user_data;

        if (TIMErrCode::ERR_SUCC == code) {
            json::Array result_array = json::Deserialize(json_params);
            jobject friendInfoResultList = tim::jni::ArrayListJni::NewArrayList();
            for (int i = 0; i < result_array.size(); ++i) {
                jobject friendInfoResultObj = tim::jni::FriendInfoResultJni::Convert2JObject(result_array[i]);
                tim::jni::ArrayListJni::Add(friendInfoResultList, friendInfoResultObj);
                _env->DeleteLocalRef(friendInfoResultObj);
            }
            tim::jni::IMCallbackJNI::Success(_callback, friendInfoResultList);
            _env->DeleteLocalRef(friendInfoResultList);

        } else {
            tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
        }
        _env->DeleteGlobalRef(_callback);
    }, jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeAddFriend, jobject application, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    json::Object friendAddApplication;
    tim::jni::FriendAddApplicationJni::Convert2CoreObject(application, friendAddApplication);
    std::string friendAddApplicationStr = json::Serialize(friendAddApplication);

    tim::TIMEngine::GetInstance()->AddFriend(friendAddApplicationStr.c_str(),[](int32_t code, const char* desc, const char* json_params, const void* user_data){
        tim::jni::ScopedJEnv scopedJEnv;
        auto _env = scopedJEnv.GetEnv();
        auto _callback = (jobject) user_data;

        if (TIMErrCode::ERR_SUCC == code) {
            json::Object result_obj = json::Deserialize(json_params);
            jobject operationResult_jObj = tim::jni::FriendOperationResultJni::Convert2JObject(result_obj);
            tim::jni::IMCallbackJNI::Success(_callback, operationResult_jObj);
            _env->DeleteLocalRef(operationResult_jObj);
        } else {
            tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
        }
        _env->DeleteGlobalRef(_callback);
    }, jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeDeleteFromFriendList, jobject user_idlist, jint delete_type, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    json::Object json_delete_friend_param;
    json_delete_friend_param[kTIMFriendshipDeleteFriendParamFriendType] = TIMFriendType(delete_type == 1?FriendTypeSingle:FriendTypeBoth);//java层和c层类型值不一致需要转换
    json::Array json_friend_array;

    int size = tim::jni::ArrayListJni::Size(user_idlist);
    for (int i = 0; i < size; ++i) {
        auto userID_jStr = (jstring) tim::jni::ArrayListJni::Get(user_idlist, i);
        json_friend_array.push_back(tim::jni::StringJni::Jstring2Cstring(env, userID_jStr).c_str());
        env->DeleteLocalRef(userID_jStr);
    }
    json_delete_friend_param[kTIMFriendshipDeleteFriendParamIdentifierArray] = json_friend_array;
    std::string paramStr = json::Serialize(json_delete_friend_param);

    tim::TIMEngine::GetInstance()->DeleteFromFriendList(paramStr.c_str(),[](int32_t code, const char* desc, const char* json_params, const void* user_data){
        tim::jni::ScopedJEnv scopedJEnv;
        auto _env = scopedJEnv.GetEnv();
        auto _callback = (jobject) user_data;

        if (TIMErrCode::ERR_SUCC == code) {
            json::Array result_array = json::Deserialize(json_params);
            jobject friendOperationResultList = tim::jni::ArrayListJni::NewArrayList();
            for (int i = 0; i < result_array.size(); ++i) {
                jobject friendOperationResultObj = tim::jni::FriendOperationResultJni::Convert2JObject(result_array[i]);
                tim::jni::ArrayListJni::Add(friendOperationResultList, friendOperationResultObj);
                _env->DeleteLocalRef(friendOperationResultObj);
            }
            tim::jni::IMCallbackJNI::Success(_callback, friendOperationResultList);
            _env->DeleteLocalRef(friendOperationResultList);
        } else {
            tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
        }
        _env->DeleteGlobalRef(_callback);
    }, jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeCheckFriend, jobject user_idlist, jint check_type, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    json::Object json_check_friend_list_param;
    json_check_friend_list_param[kTIMFriendshipCheckFriendTypeParamCheckType] = TIMFriendType(check_type);

    json::Array json_friend_array;
    int size = tim::jni::ArrayListJni::Size(user_idlist);
    for (int i = 0; i < size; ++i) {
        auto userID_jStr = (jstring) tim::jni::ArrayListJni::Get(user_idlist, i);
        json_friend_array.push_back(tim::jni::StringJni::Jstring2Cstring(env, userID_jStr).c_str());
        env->DeleteLocalRef(userID_jStr);
    }
    json_check_friend_list_param[kTIMFriendshipCheckFriendTypeParamIdentifierArray] = json_friend_array;

    std::string paramStr = json::Serialize(json_check_friend_list_param);
    tim::TIMEngine::GetInstance()->CheckFriend(paramStr.c_str(),[](int32_t code, const char* desc, const char* json_params, const void* user_data){
        tim::jni::ScopedJEnv scopedJEnv;
        auto _env = scopedJEnv.GetEnv();
        auto _callback = (jobject) user_data;

        if (TIMErrCode::ERR_SUCC == code) {
            json::Array result_array = json::Deserialize(json_params);
            jobject friendCheckResultList = tim::jni::ArrayListJni::NewArrayList();
            for (int i = 0; i < result_array.size(); ++i) {
                jobject friendCheckResultObj = tim::jni::FriendCheckResultJni::Convert2JObject(result_array[i]);
                tim::jni::ArrayListJni::Add(friendCheckResultList, friendCheckResultObj);
                _env->DeleteLocalRef(friendCheckResultObj);
            }
            tim::jni::IMCallbackJNI::Success(_callback, friendCheckResultList);
            _env->DeleteLocalRef(friendCheckResultList);

        } else {
            tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
        }
        _env->DeleteGlobalRef(_callback);
    }, jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeGetFriendApplicationList, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    json::Object json_get_pendency_list_param;
    json_get_pendency_list_param[kTIMFriendshipGetPendencyListParamType] = FriendPendencyTypeBoth;
    json_get_pendency_list_param[kTIMFriendshipGetPendencyListParamStartSeq] = 0;
    std::string paramStr = json::Serialize(json_get_pendency_list_param);

    tim::TIMEngine::GetInstance()->GetFriendApplicationList(paramStr.c_str(),[](int32_t code, const char* desc, const char* json_params, const void* user_data){
        tim::jni::ScopedJEnv scopedJEnv;
        auto _env = scopedJEnv.GetEnv();
        auto _callback = (jobject) user_data;

        if (TIMErrCode::ERR_SUCC == code) {
            json::Object result_obj = json::Deserialize(json_params);
            jobject applicationResult = tim::jni::FriendApplicationResultJni::Convert2JObject(result_obj);
            if (applicationResult) {
                tim::jni::IMCallbackJNI::Success(_callback, applicationResult);
                _env->DeleteLocalRef(applicationResult);
            }
        } else {
            tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
        }
        _env->DeleteGlobalRef(_callback);
    },jni_callback);
}

void CHandleFriendAddRequest(JNIEnv *env,const json::Object & param_obj,jobject callback){
    jobject jni_callback = env->NewGlobalRef(callback);

    std::string friendApplicationStr = json::Serialize(param_obj);
    tim::TIMEngine::GetInstance()->ProcessFriendApplication(friendApplicationStr.c_str(),[](int32_t code, const char* desc, const char* json_params, const void* user_data){
        tim::jni::ScopedJEnv scopedJEnv;
        auto _env = scopedJEnv.GetEnv();
        auto _callback = (jobject) user_data;

        if (TIMErrCode::ERR_SUCC == code) {
            json::Object result_obj = json::Deserialize(json_params);
            jobject operationResult_jObj = tim::jni::FriendOperationResultJni::Convert2JObject(result_obj);
            tim::jni::IMCallbackJNI::Success(_callback, operationResult_jObj);
            _env->DeleteLocalRef(operationResult_jObj);
        } else {
            tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
        }
        _env->DeleteGlobalRef(_callback);
    }, jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeAcceptFriendApplication, jobject application, jint response_type, jobject callback) {
    json::Object friendApplication;
    bool flag = tim::jni::FriendApplicationJni::Convert2CoreObject_Request(application, friendApplication);
    if (flag){
        friendApplication[kTIMFriendResponseAction] = TIMFriendResponseAction(response_type);
        CHandleFriendAddRequest(env,friendApplication,callback);
    }
}

DEFINE_NATIVE_FUNC(void, NativeRefuseFriendApplication, jobject application, jobject callback) {
    json::Object friendApplication;
    bool flag = tim::jni::FriendApplicationJni::Convert2CoreObject_Request(application, friendApplication);
    if (flag){
        friendApplication[kTIMFriendResponseAction] = ResponseActionReject;
        CHandleFriendAddRequest(env,friendApplication,callback);
    }
}

DEFINE_NATIVE_FUNC(void, NativeDeleteFriendApplication, jobject application, jobject callback) {

    json::Object json_delete_pendency_param;
    json_delete_pendency_param[kTIMFriendshipDeletePendencyParamType] = FriendPendencyTypeComeIn;

    json::Array json_application_array;
    std::string userID;
    tim::jni::FriendApplicationJni::Convert2CoreObject_Delete(application, userID);
    json_application_array.push_back(userID);
    json_delete_pendency_param[kTIMFriendshipDeletePendencyParamIdentifierArray] = json_application_array;

    std::string paramStr = json::Serialize(json_delete_pendency_param);

    tim::TIMEngine::GetInstance()->DeleteFriendApplication(paramStr.c_str(), new tim::TIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativesSetFriendApplicationRead, jobject callback) {
    tim::TIMEngine::GetInstance()->SetFriendApplicationRead(new tim::TIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeAddToBlackList, jobject user_idlist, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    json::Array userIDList;
    int size = tim::jni::ArrayListJni::Size(user_idlist);
    for (int i = 0; i < size; ++i) {
        auto userID_jStr = (jstring) tim::jni::ArrayListJni::Get(user_idlist, i);
        userIDList.push_back(tim::jni::StringJni::Jstring2Cstring(env, userID_jStr).c_str());
        env->DeleteLocalRef(userID_jStr);
    }
    std::string userIDListStr = json::Serialize(userIDList);

    tim::TIMEngine::GetInstance()->AddToBlackList(userIDListStr.c_str(),[](int32_t code, const char* desc, const char* json_params, const void* user_data){
        tim::jni::ScopedJEnv scopedJEnv;
        auto _env = scopedJEnv.GetEnv();
        auto _callback = (jobject) user_data;

        if (TIMErrCode::ERR_SUCC == code) {
            json::Array result_array = json::Deserialize(json_params);
            jobject friendOperationResultList = tim::jni::ArrayListJni::NewArrayList();
            for (int i = 0; i < result_array.size(); ++i) {
                jobject friendOperationResultObj = tim::jni::FriendOperationResultJni::Convert2JObject(result_array[i]);
                tim::jni::ArrayListJni::Add(friendOperationResultList, friendOperationResultObj);
                _env->DeleteLocalRef(friendOperationResultObj);
            }
            tim::jni::IMCallbackJNI::Success(_callback, friendOperationResultList);
            _env->DeleteLocalRef(friendOperationResultList);
        } else {
            tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
        }
        _env->DeleteGlobalRef(_callback);
    }, jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeDeleteFromBlackList, jobject user_idlist, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    json::Array userIDList;
    int size = tim::jni::ArrayListJni::Size(user_idlist);
    for (int i = 0; i < size; ++i) {
        auto userID_jStr = (jstring) tim::jni::ArrayListJni::Get(user_idlist, i);
        userIDList.push_back(tim::jni::StringJni::Jstring2Cstring(env, userID_jStr).c_str());
        env->DeleteLocalRef(userID_jStr);
    }
    std::string userIDListStr = json::Serialize(userIDList);

    tim::TIMEngine::GetInstance()->DeleteFromBlackList(userIDListStr.c_str(),[](int32_t code, const char* desc, const char* json_params, const void* user_data){
        tim::jni::ScopedJEnv scopedJEnv;
        auto _env = scopedJEnv.GetEnv();
        auto _callback = (jobject) user_data;

        if (TIMErrCode::ERR_SUCC == code) {
            json::Array result_array = json::Deserialize(json_params);
            jobject friendOperationResultList = tim::jni::ArrayListJni::NewArrayList();
            for (int i = 0; i < result_array.size(); ++i) {
                jobject friendOperationResultObj = tim::jni::FriendOperationResultJni::Convert2JObject(result_array[i]);
                tim::jni::ArrayListJni::Add(friendOperationResultList, friendOperationResultObj);
                _env->DeleteLocalRef(friendOperationResultObj);
            }
            tim::jni::IMCallbackJNI::Success(_callback, friendOperationResultList);
            _env->DeleteLocalRef(friendOperationResultList);

        } else {
            tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
        }
        _env->DeleteGlobalRef(_callback);
    }, jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeGetBlackList, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    tim::TIMEngine::GetInstance()->GetBlackList([](int32_t code, const char* desc, const char* json_params, const void* user_data){
        tim::jni::ScopedJEnv scopedJEnv;
        auto _env = scopedJEnv.GetEnv();
        auto _callback = (jobject) user_data;

        if (TIMErrCode::ERR_SUCC == code) {
            json::Array black_array = json::Deserialize(json_params);
            jobject friendInfoList = tim::jni::ArrayListJni::NewArrayList();
            for (int i = 0; i < black_array.size(); ++i) {
                jobject friendInfoObj = tim::jni::FriendInfoJni::Convert2JObject(black_array[i]);
                tim::jni::ArrayListJni::Add(friendInfoList, friendInfoObj);
                _env->DeleteLocalRef(friendInfoObj);
            }
            tim::jni::IMCallbackJNI::Success(_callback, friendInfoList);
            _env->DeleteLocalRef(friendInfoList);

        } else {
            tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
        }
        _env->DeleteGlobalRef(_callback);
    },jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeCreateFriendGroup, jstring group_name, jobject user_idlist, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    json::Array json_group_name_array;
    json_group_name_array.push_back(tim::jni::StringJni::Jstring2Cstring(env, group_name));
    json::Array json_friend_array;
    int size = tim::jni::ArrayListJni::Size(user_idlist);
    for (int i = 0; i < size; ++i) {
        auto userID_jStr = (jstring) tim::jni::ArrayListJni::Get(user_idlist, i);
        json_friend_array.push_back(tim::jni::StringJni::Jstring2Cstring(env, userID_jStr).c_str());
        env->DeleteLocalRef(userID_jStr);
    }

    json::Object json_create_friend_group_param;
    json_create_friend_group_param[kTIMFriendshipCreateFriendGroupParamNameArray] = json_group_name_array;
    json_create_friend_group_param[kTIMFriendshipCreateFriendGroupParamIdentifierArray] = json_friend_array;

    std::string paramStr = json::Serialize(json_create_friend_group_param);
    tim::TIMEngine::GetInstance()->CreateFriendGroup(paramStr.c_str(),[](int32_t code, const char* desc, const char* json_params, const void* user_data){
        tim::jni::ScopedJEnv scopedJEnv;
        auto _env = scopedJEnv.GetEnv();
        auto _callback = (jobject) user_data;

        if (TIMErrCode::ERR_SUCC == code) {
            json::Array result_array = json::Deserialize(json_params);
            jobject friendOperationResultList = tim::jni::ArrayListJni::NewArrayList();
            for (int i = 0; i < result_array.size(); ++i) {
                jobject friendOperationResultObj = tim::jni::FriendOperationResultJni::Convert2JObject(result_array[i]);
                tim::jni::ArrayListJni::Add(friendOperationResultList, friendOperationResultObj);
                _env->DeleteLocalRef(friendOperationResultObj);
            }
            tim::jni::IMCallbackJNI::Success(_callback, friendOperationResultList);
            _env->DeleteLocalRef(friendOperationResultList);

        } else {
            tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
        }
        _env->DeleteGlobalRef(_callback);
    }, jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeGetFriendGroups, jobject group_name_list, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    json::Array groupNameList;
    int size = tim::jni::ArrayListJni::Size(group_name_list);
    for (int i = 0; i < size; ++i) {
        auto userID_jStr = (jstring) tim::jni::ArrayListJni::Get(group_name_list, i);
        groupNameList.push_back(tim::jni::StringJni::Jstring2Cstring(env, userID_jStr).c_str());
        env->DeleteLocalRef(userID_jStr);
    }
    std::string groupNameListStr = json::Serialize(groupNameList);

    tim::TIMEngine::GetInstance()->GetFriendGroups(groupNameListStr.c_str(),[](int32_t code, const char* desc, const char* json_params, const void* user_data){
        tim::jni::ScopedJEnv scopedJEnv;
        auto _env = scopedJEnv.GetEnv();
        auto _callback = (jobject) user_data;

        if (TIMErrCode::ERR_SUCC == code) {
            json::Array group_array = json::Deserialize(json_params);
            jobject friendGroupList = tim::jni::ArrayListJni::NewArrayList();
            for (int i = 0; i < group_array.size(); ++i) {
                jobject friendGroupObj = tim::jni::FriendGroupJni::Convert2JObject(group_array[i]);
                tim::jni::ArrayListJni::Add(friendGroupList, friendGroupObj);
                _env->DeleteLocalRef(friendGroupObj);
            }
            tim::jni::IMCallbackJNI::Success(_callback, friendGroupList);
            _env->DeleteLocalRef(friendGroupList);

        } else {
            tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
        }
        _env->DeleteGlobalRef(_callback);
    }, jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeDeleteFriendGroup, jobject group_name_list, jobject callback) {
    json::Array groupNameList;
    int size = tim::jni::ArrayListJni::Size(group_name_list);
    for (int i = 0; i < size; ++i) {
        auto userID_jStr = (jstring) tim::jni::ArrayListJni::Get(group_name_list, i);
        groupNameList.push_back(tim::jni::StringJni::Jstring2Cstring(env, userID_jStr).c_str());
        env->DeleteLocalRef(userID_jStr);
    }
    std::string groupNameListStr = json::Serialize(groupNameList);

    tim::TIMEngine::GetInstance()->DeleteFriendGroup(groupNameListStr.c_str(), new tim::TIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeRenameFriendGroup, jstring old_name, jstring new_name, jobject callback) {

    std::string oldNameStr = tim::jni::StringJni::Jstring2Cstring(env, old_name);
    std::string newNameStr = tim::jni::StringJni::Jstring2Cstring(env, new_name);

    json::Object json_modify_friend_group_param;
    json_modify_friend_group_param[kTIMFriendshipModifyFriendGroupParamName] = oldNameStr;
    json_modify_friend_group_param[kTIMFriendshipModifyFriendGroupParamNewName] = newNameStr;
    std::string paramStr = json::Serialize(json_modify_friend_group_param);

    tim::TIMEngine::GetInstance()->RenameFriendGroup(paramStr.c_str(), new tim::TIMCallbackIMpl(callback));
}

DEFINE_NATIVE_FUNC(void, NativeAddFriendsToFriendGroup, jstring group_name, jobject user_idlist, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    json::Object json_add_friend_to_group_param;
    json_add_friend_to_group_param[kTIMFriendshipModifyFriendGroupParamName] = tim::jni::StringJni::Jstring2Cstring(env, group_name);

    json::Array json_friend_add_array;
    int size = tim::jni::ArrayListJni::Size(user_idlist);
    for (int i = 0; i < size; ++i) {
        auto userID_jStr = (jstring) tim::jni::ArrayListJni::Get(user_idlist, i);
        json_friend_add_array.push_back(tim::jni::StringJni::Jstring2Cstring(env, userID_jStr).c_str());
        env->DeleteLocalRef(userID_jStr);
    }
    json_add_friend_to_group_param[kTIMFriendshipModifyFriendGroupParamAddIdentifierArray] = json_friend_add_array;
    std::string paramStr = json::Serialize(json_add_friend_to_group_param);

    tim::TIMEngine::GetInstance()->AddFriendsToFriendGroup(paramStr.c_str(),[](int32_t code, const char* desc, const char* json_params, const void* user_data){
        tim::jni::ScopedJEnv scopedJEnv;
        auto _env = scopedJEnv.GetEnv();
        auto _callback = (jobject) user_data;

        if (TIMErrCode::ERR_SUCC == code) {
            json::Array result_obj = json::Deserialize(json_params);
            jobject friendOperationResultList = tim::jni::ArrayListJni::NewArrayList();
            for (int i = 0; i < result_obj.size(); ++i) {
                jobject friendOperationResultObj = tim::jni::FriendOperationResultJni::Convert2JObject(result_obj[i]);
                tim::jni::ArrayListJni::Add(friendOperationResultList, friendOperationResultObj);
                _env->DeleteLocalRef(friendOperationResultObj);
            }
            tim::jni::IMCallbackJNI::Success(_callback, friendOperationResultList);
            _env->DeleteLocalRef(friendOperationResultList);

        } else {
            tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
        }
        _env->DeleteGlobalRef(_callback);
    }, jni_callback);
}

DEFINE_NATIVE_FUNC(void, NativeDeleteFriendsFromFriendGroup, jstring group_name, jobject user_idlist, jobject callback) {
    jobject jni_callback = env->NewGlobalRef(callback);

    json::Object json_delete_friend_group_param;
    json_delete_friend_group_param[kTIMFriendshipModifyFriendGroupParamName] = tim::jni::StringJni::Jstring2Cstring(env, group_name);

    json::Array json_friend_delete_array;
    int size = tim::jni::ArrayListJni::Size(user_idlist);
    for (int i = 0; i < size; ++i) {
        auto userID_jStr = (jstring) tim::jni::ArrayListJni::Get(user_idlist, i);
        json_friend_delete_array.push_back(tim::jni::StringJni::Jstring2Cstring(env, userID_jStr));
        env->DeleteLocalRef(userID_jStr);
    }
    json_delete_friend_group_param[kTIMFriendshipModifyFriendGroupParamDeleteIdentifierArray] = json_friend_delete_array;

    std::string paramStr = json::Serialize(json_delete_friend_group_param);

    tim::TIMEngine::GetInstance()->DeleteFriendsFromFriendGroup(paramStr.c_str(),[](int32_t code, const char* desc, const char* json_params, const void* user_data){
        tim::jni::ScopedJEnv scopedJEnv;
        auto _env = scopedJEnv.GetEnv();
        auto _callback = (jobject) user_data;

        if (TIMErrCode::ERR_SUCC == code) {
            json::Array result_array = json::Deserialize(json_params);
            jobject friendOperationResultList = tim::jni::ArrayListJni::NewArrayList();
            for (int i = 0; i < result_array.size(); ++i) {
                jobject friendOperationResultObj = tim::jni::FriendOperationResultJni::Convert2JObject(result_array[i]);
                tim::jni::ArrayListJni::Add(friendOperationResultList, friendOperationResultObj);
                _env->DeleteLocalRef(friendOperationResultObj);
            }
            tim::jni::IMCallbackJNI::Success(_callback, friendOperationResultList);
            _env->DeleteLocalRef(friendOperationResultList);

        } else {
            tim::jni::IMCallbackJNI::Fail(_callback, code, desc);
        }
        _env->DeleteGlobalRef(_callback);
    }, jni_callback);
}

// java 和 native 方法映射
static JNINativeMethod gMethods[] = {
        {"nativeAddFriendListener",            "(Lcom/tencent/imsdk/v2/V2TIMFriendshipListener;Ljava/lang/String;)V",                      (void *) NativeAddFriendListener},
        {"nativeRemoveFriendListener",         "(Ljava/lang/String;)V",                                                                    (void *) NativeRemoveFriendListener},
        {"nativeGetFriendList",                "(Lcom/tencent/imsdk/common/IMCallback;)V",                                                 (void *) NativeGetFriendList},
        {"nativeGetFriendsInfo",               "(Ljava/util/List;Lcom/tencent/imsdk/common/IMCallback;)V",                                 (void *) NativeGetFriendsInfo},
        {"nativeSetFriendInfo",                "(Lcom/tencent/imsdk/v2/V2TIMFriendInfo;Lcom/tencent/imsdk/common/IMCallback;)V",           (void *) NativeSetFriendInfo},
        {"nativeSearchFriends",                "(Lcom/tencent/imsdk/v2/V2TIMFriendSearchParam;Lcom/tencent/imsdk/common/IMCallback;)V",    (void *) NativeSearchFriends},
        {"nativeAddFriend",                    "(Lcom/tencent/imsdk/v2/V2TIMFriendAddApplication;Lcom/tencent/imsdk/common/IMCallback;)V", (void *) NativeAddFriend},
        {"nativeDeleteFromFriendList",         "(Ljava/util/List;ILcom/tencent/imsdk/common/IMCallback;)V",                                (void *) NativeDeleteFromFriendList},
        {"nativeCheckFriend",                  "(Ljava/util/List;ILcom/tencent/imsdk/common/IMCallback;)V",                                (void *) NativeCheckFriend},
        {"nativeGetFriendApplicationList",     "(Lcom/tencent/imsdk/common/IMCallback;)V",                                                 (void *) NativeGetFriendApplicationList},
        {"nativeAcceptFriendApplication",      "(Lcom/tencent/imsdk/v2/V2TIMFriendApplication;ILcom/tencent/imsdk/common/IMCallback;)V",   (void *) NativeAcceptFriendApplication},
        {"nativeRefuseFriendApplication",      "(Lcom/tencent/imsdk/v2/V2TIMFriendApplication;Lcom/tencent/imsdk/common/IMCallback;)V",    (void *) NativeRefuseFriendApplication},
        {"nativeDeleteFriendApplication",      "(Lcom/tencent/imsdk/v2/V2TIMFriendApplication;Lcom/tencent/imsdk/common/IMCallback;)V",    (void *) NativeDeleteFriendApplication},
        {"nativesSetFriendApplicationRead",    "(Lcom/tencent/imsdk/common/IMCallback;)V",                                                 (void *) NativesSetFriendApplicationRead},
        {"nativeAddToBlackList",               "(Ljava/util/List;Lcom/tencent/imsdk/common/IMCallback;)V",                                 (void *) NativeAddToBlackList},
        {"nativeDeleteFromBlackList",          "(Ljava/util/List;Lcom/tencent/imsdk/common/IMCallback;)V",                                 (void *) NativeDeleteFromBlackList},
        {"nativeGetBlackList",                 "(Lcom/tencent/imsdk/common/IMCallback;)V",                                                 (void *) NativeGetBlackList},
        {"nativeCreateFriendGroup",            "(Ljava/lang/String;Ljava/util/List;Lcom/tencent/imsdk/common/IMCallback;)V",               (void *) NativeCreateFriendGroup},
        {"nativeGetFriendGroups",              "(Ljava/util/List;Lcom/tencent/imsdk/common/IMCallback;)V",                                 (void *) NativeGetFriendGroups},
        {"nativeDeleteFriendGroup",            "(Ljava/util/List;Lcom/tencent/imsdk/common/IMCallback;)V",                                 (void *) NativeDeleteFriendGroup},
        {"nativeRenameFriendGroup",            "(Ljava/lang/String;Ljava/lang/String;Lcom/tencent/imsdk/common/IMCallback;)V",             (void *) NativeRenameFriendGroup},
        {"nativeAddFriendsToFriendGroup",      "(Ljava/lang/String;Ljava/util/List;Lcom/tencent/imsdk/common/IMCallback;)V",               (void *) NativeAddFriendsToFriendGroup},
        {"nativeDeleteFriendsFromFriendGroup", "(Ljava/lang/String;Ljava/util/List;Lcom/tencent/imsdk/common/IMCallback;)V",               (void *) NativeDeleteFriendsFromFriendGroup},
};

//注册 native 方法
int RegisterNativeMethodsForV2TIMFriendshipManager(JNIEnv *env) {
    return tim::jni::util::RegisterNativeMethods(env, "com/tencent/imsdk/v2/V2TIMFriendshipManager", gMethods,
                                                 sizeof(gMethods) / sizeof(gMethods[0]));
}

#ifdef __cplusplus
}
#endif
