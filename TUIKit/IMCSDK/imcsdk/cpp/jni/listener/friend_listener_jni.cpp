//
// Created by EDY on 2022/9/27.
//

#include <LogUtil.h>
#include <tim_engine.h>
#include <jni_helper.h>
#include "friend_listener_jni.h"
#include "java_basic_jni.h"
#include "friend_info_jni.h"
#include "friend_application_jni.h"
#include "TIMCloud.h"
#include "friend_info_result_jni.h"

namespace tim {
    namespace jni {

        jclass FriendListenerJni::j_cls_;
        jmethodID FriendListenerJni::j_method_id_array_[MethodIDMax];

        void FriendListenerJni::initListener() {
            TIMSetOnAddFriendCallback(ImplTIMOnAddFriendCallback, &listener_friend_map);
            TIMSetOnDeleteFriendCallback(ImplTIMOnDeleteFriendCallback, &listener_friend_map);
            TIMSetUpdateFriendProfileCallback(ImplTIMUpdateFriendProfileCallback, &listener_friend_map);
            TIMSetFriendAddRequestCallback(ImplTIMFriendAddRequestCallback, &listener_friend_map);
            TIMSetFriendApplicationListDeletedCallback(ImplTIMFriendApplicationListDeletedCallback, &listener_friend_map);
            TIMSetFriendApplicationListReadCallback(ImplTIMFriendApplicationListReadCallback, &listener_friend_map);
            TIMSetFriendBlackListAddedCallback(ImplTIMFriendBlackListAddedCallback, &listener_friend_map);
            TIMSetFriendBlackListDeletedCallback(ImplTIMFriendBlackListDeletedCallback, &listener_friend_map);
        }

        void FriendListenerJni::unInitListener() {
            TIMSetOnAddFriendCallback(ImplTIMOnAddFriendCallback, nullptr);
            TIMSetOnDeleteFriendCallback(ImplTIMOnDeleteFriendCallback, nullptr);
            TIMSetUpdateFriendProfileCallback(ImplTIMUpdateFriendProfileCallback, nullptr);
            TIMSetFriendAddRequestCallback(ImplTIMFriendAddRequestCallback, nullptr);
            TIMSetFriendApplicationListDeletedCallback(ImplTIMFriendApplicationListDeletedCallback, nullptr);
            TIMSetFriendApplicationListReadCallback(ImplTIMFriendApplicationListReadCallback, nullptr);
            TIMSetFriendBlackListAddedCallback(ImplTIMFriendBlackListAddedCallback, nullptr);
            TIMSetFriendBlackListDeletedCallback(ImplTIMFriendBlackListDeletedCallback, nullptr);
        }

        void FriendListenerJni::AddListener(JNIEnv *env, jobject listener_friend, jstring listenerPath) {
            if (nullptr == listener_friend) {
                LOGE("FriendListenerJni | AddListener listener_simple is null");
                return;
            }
            if (listener_friend_map.empty()) {
                tim::ObserverManager::getInstance().addListener(this);
            }
            std::string path = StringJni::Jstring2Cstring(env, listenerPath);
            for (auto &item: listener_friend_map) {
                if (path.empty() || path == item.first) {
                    return;
                }
            }

            jobject j_obj = env->NewGlobalRef(listener_friend);
            listener_friend_map.insert(std::make_pair(path, j_obj));
        }

        void FriendListenerJni::RemoveListener(JNIEnv *env, jstring listenerPath) {
            if (nullptr == listenerPath) {
                LOGE("FriendListenerJni | RemoveListener listener_simple is null");
                return;
            }
            listener_friend_map.erase(StringJni::Jstring2Cstring(env, listenerPath));
            if (listener_friend_map.empty()) {
                tim::ObserverManager::getInstance().removeListener(this);
            }
        }

        bool FriendListenerJni::InitIDs(JNIEnv *env) {
            if (j_cls_ != nullptr) {
                return true;
            }
            jclass cls = env->FindClass("com/tencent/imsdk/v2/V2TIMFriendshipListener");
            if (cls == nullptr) {
                return false;
            }
            j_cls_ = (jclass) env->NewGlobalRef(cls);
            env->DeleteLocalRef(cls);

            jmethodID jmethod = nullptr;

            jmethod = env->GetMethodID(j_cls_, "onFriendApplicationListAdded", "(Ljava/util/List;)V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDOnFriendApplicationListAdded] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "onFriendApplicationListDeleted", "(Ljava/util/List;)V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDOnFriendApplicationListDeleted] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "onFriendApplicationListRead", "()V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDOnFriendApplicationListRead] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "onFriendListAdded", "(Ljava/util/List;)V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDOnFriendListAdded] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "onFriendListDeleted", "(Ljava/util/List;)V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDOnFriendListDeleted] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "onBlackListAdd", "(Ljava/util/List;)V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDOnBlackListAdd] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "onBlackListDeleted", "(Ljava/util/List;)V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDOnBlackListDeleted] = jmethod;

            jmethod = env->GetMethodID(j_cls_, "onFriendInfoChanged", "(Ljava/util/List;)V");
            if (nullptr == jmethod) {
                return false;
            }
            j_method_id_array_[MethodIDOnFriendInfoChanged] = jmethod;

            return true;
        }

        bool FriendListenerJni::empty() {
            return listener_friend_map.empty();
        }

        void FriendListenerJni::ImplTIMOnAddFriendCallback(const char *json_identifier_array, const void *user_data) {
            json::Array userIDList = json::Deserialize(json_identifier_array);
            std::string userIDListStr = json::Serialize(userIDList);

            tim::TIMEngine::GetInstance()->GetFriendsInfo(userIDListStr.c_str(), [](int32_t code, const char *desc, const char *json_params, const void *user_data) {
                if (TIMErrCode::ERR_SUCC == code) {
                    json::Array result_array = json::Deserialize(json_params);
                    json::Object result_obj = result_array[0];
                    if (result_obj.HasKey(kTIMFriendshipFriendInfoGetResultFriendInfo)) {
                        json::Array friendInfoArray;
                        friendInfoArray.push_back(result_obj[kTIMFriendshipFriendInfoGetResultFriendInfo]);
                        OnFriendListAdded(*(std::map<std::string, jobject> *) user_data, friendInfoArray);
                    }
                }
            }, user_data);

        }

        void FriendListenerJni::ImplTIMOnDeleteFriendCallback(const char *json_identifier_array, const void *user_data) {
            OnFriendListDeleted(*(std::map<std::string, jobject> *) user_data, json::Deserialize(json_identifier_array));
        }

        void FriendListenerJni::ImplTIMUpdateFriendProfileCallback(const char *json_friend_profile_update_array, const void *user_data) {
            OnFriendInfoChanged(*(std::map<std::string, jobject> *) user_data, json::Deserialize(json_friend_profile_update_array));
        }

        void FriendListenerJni::ImplTIMFriendAddRequestCallback(const char *json_friend_add_request_pendency_array, const void *user_data) {
            OnFriendApplicationListAdded(*(std::map<std::string, jobject> *) user_data, json::Deserialize(json_friend_add_request_pendency_array));
        }

        void FriendListenerJni::ImplTIMFriendApplicationListDeletedCallback(const char *json_identifier_array, const void *user_data) {
            OnFriendApplicationListDeleted(*(std::map<std::string, jobject> *) user_data, json::Deserialize(json_identifier_array));
        }

        void FriendListenerJni::ImplTIMFriendApplicationListReadCallback(const void *user_data) {
            OnFriendApplicationListRead(*(std::map<std::string, jobject> *) user_data);
        }

        void FriendListenerJni::ImplTIMFriendBlackListAddedCallback(const char *json_friend_black_added_array, const void *user_data) {
            OnBlackListAdded(*(std::map<std::string, jobject> *) user_data, json::Deserialize(json_friend_black_added_array));
        }

        void FriendListenerJni::ImplTIMFriendBlackListDeletedCallback(const char *json_identifier_array, const void *user_data) {
            OnBlackListDeleted(*(std::map<std::string, jobject> *) user_data, json::Deserialize(json_identifier_array));
        }

        void FriendListenerJni::OnFriendApplicationListAdded(const std::map<std::string, jobject> &_listener_, const json::Array &applicationList_json) {
            if (_listener_.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jobject applicationListObj = ArrayListJni::NewArrayList();
            for (int i = 0; i < applicationList_json.size(); ++i) {
                jobject applicationObj = FriendApplicationJni::Convert2JObject(applicationList_json[i]);
                if (applicationObj) {
                    ArrayListJni::Add(applicationListObj, applicationObj);
                    env->DeleteLocalRef(applicationObj);
                }
            }

            for (auto &item: _listener_) {
                env->CallVoidMethod(item.second, j_method_id_array_[MethodIDOnFriendApplicationListAdded], applicationListObj);
            }

            env->DeleteLocalRef(applicationListObj);
        }

        void FriendListenerJni::OnFriendApplicationListDeleted(const std::map<std::string, jobject> &_listener_, const json::Array &userIDList) {
            if (_listener_.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jobject userIDListObj = ArrayListJni::NewArrayList();
            for (int i = 0; i < userIDList.size(); ++i) {
                jstring userIDStr = StringJni::Cstring2Jstring(env, userIDList[i]);
                if (userIDStr) {
                    ArrayListJni::Add(userIDListObj, userIDStr);
                    env->DeleteLocalRef(userIDStr);
                }
            }

            for (auto &item: _listener_) {
                env->CallVoidMethod(item.second, j_method_id_array_[MethodIDOnFriendApplicationListDeleted], userIDListObj);
            }

            env->DeleteLocalRef(userIDListObj);
        }

        void FriendListenerJni::OnFriendApplicationListRead(const std::map<std::string, jobject> &_listener_) {
            if (_listener_.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            for (auto &item: _listener_) {
                env->CallVoidMethod(item.second, j_method_id_array_[MethodIDOnFriendApplicationListRead]);
            }
        }

        void FriendListenerJni::OnFriendListAdded(const std::map<std::string, jobject> &_listener_, const json::Array &friendInfoList) {
            if (_listener_.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jobject friendInfoListObj = ArrayListJni::NewArrayList();
            for (int i = 0; i < friendInfoList.size(); ++i) {
                jobject friendInfoObj = FriendInfoJni::Convert2JObject(friendInfoList[i]);
                if (friendInfoObj) {
                    ArrayListJni::Add(friendInfoListObj, friendInfoObj);
                    env->DeleteLocalRef(friendInfoObj);
                }
            }

            for (auto &item: _listener_) {
                env->CallVoidMethod(item.second, j_method_id_array_[MethodIDOnFriendListAdded], friendInfoListObj);
            }

            env->DeleteLocalRef(friendInfoListObj);
        }

        void FriendListenerJni::OnFriendListDeleted(const std::map<std::string, jobject> &_listener_, const json::Array &userIDList) {
            if (_listener_.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jobject userIDListObj = ArrayListJni::NewArrayList();
            for (int i = 0; i < userIDList.size(); ++i) {
                jstring userIDStr = StringJni::Cstring2Jstring(env, userIDList[i]);
                if (userIDStr) {
                    ArrayListJni::Add(userIDListObj, userIDStr);
                    env->DeleteLocalRef(userIDStr);
                }
            }

            for (auto &item: _listener_) {
                env->CallVoidMethod(item.second, j_method_id_array_[MethodIDOnFriendListDeleted], userIDListObj);
            }

            env->DeleteLocalRef(userIDListObj);
        }

        void FriendListenerJni::OnBlackListAdded(const std::map<std::string, jobject> &_listener_, const json::Array &friendInfoList) {
            if (_listener_.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jobject friendInfoListObj = ArrayListJni::NewArrayList();
            for (int i = 0; i < friendInfoList.size(); ++i) {
                jobject friendInfoObj = FriendInfoJni::Convert2JObject(friendInfoList[i]);
                if (friendInfoObj) {
                    ArrayListJni::Add(friendInfoListObj, friendInfoObj);
                    env->DeleteLocalRef(friendInfoObj);
                }
            }

            for (auto &item: _listener_) {
                env->CallVoidMethod(item.second, j_method_id_array_[MethodIDOnBlackListAdd], friendInfoListObj);
            }

            env->DeleteLocalRef(friendInfoListObj);
        }

        void FriendListenerJni::OnBlackListDeleted(const std::map<std::string, jobject> &_listener_, const json::Array &userIDList) {
            if (_listener_.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jobject userIDListObj = ArrayListJni::NewArrayList();
            for (int i = 0; i < userIDList.size(); ++i) {
                jstring userIDStr = StringJni::Cstring2Jstring(env, userIDList[i]);
                if (userIDStr) {
                    ArrayListJni::Add(userIDListObj, userIDStr);
                    env->DeleteLocalRef(userIDStr);
                }
            }

            for (auto &item: _listener_) {
                env->CallVoidMethod(item.second, j_method_id_array_[MethodIDOnBlackListDeleted], userIDListObj);
            }

            env->DeleteLocalRef(userIDListObj);
        }

        void FriendListenerJni::OnFriendInfoChanged(const std::map<std::string, jobject> &_listener_, const json::Array &friendInfoList) {
            if (_listener_.empty()) {
                return;
            }

            ScopedJEnv scopedJEnv;
            auto *env = scopedJEnv.GetEnv();

            jobject friendInfoListObj = ArrayListJni::NewArrayList();
            for (int i = 0; i < friendInfoList.size(); ++i) {
                jobject friendInfoObj = FriendInfoJni::Convert2JObject_UpdateFriendInfoCallback(friendInfoList[i]);
                if (friendInfoObj) {
                    ArrayListJni::Add(friendInfoListObj, friendInfoObj);
                    env->DeleteLocalRef(friendInfoObj);
                }
            }

            for (auto &item: _listener_) {
                env->CallVoidMethod(item.second, j_method_id_array_[MethodIDOnFriendInfoChanged], friendInfoListObj);
            }

            env->DeleteLocalRef(friendInfoListObj);
        }


    }// namespace jni
}// namespace tim