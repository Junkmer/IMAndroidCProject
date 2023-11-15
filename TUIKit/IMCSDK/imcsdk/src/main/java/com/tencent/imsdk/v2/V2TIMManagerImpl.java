package com.tencent.imsdk.v2;

import android.annotation.SuppressLint;
import android.content.Context;
import android.os.Build;
import android.text.TextUtils;

import com.tencent.imsdk.BaseConstants;
import com.tencent.imsdk.common.IMCallback;
import com.tencent.imsdk.common.IMContext;
import com.tencent.imsdk.common.IMLog;
import com.tencent.imsdk.common.SystemUtil;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.lang.reflect.Field;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

public class V2TIMManagerImpl extends V2TIMManager {
    private final static String TAG = V2TIMManagerImpl.class.getSimpleName();

    private static class V2TIMManagerImplHolder {
        private static final V2TIMManagerImpl v2TIMManagerImpl = new V2TIMManagerImpl();
    }

    public static V2TIMManagerImpl getInstance() {
        return V2TIMManagerImplHolder.v2TIMManagerImpl;
    }

    protected V2TIMManagerImpl() {

    }

    @Override
    public boolean initSDK(Context context, int sdkAppID, V2TIMSDKConfig config) {
        if (!mLoadLibrarySuccess) {
            IMLog.e(TAG, "libimsdk.so is not loaded");
            return false;
        }

        if (sdkAppID <= 0) {
            IMLog.e(TAG, "invalid sdkAppID:" + sdkAppID);
            return false;
        }

        if (null == context) {
            IMLog.e(TAG, "null context");
            return false;
        }

        if (isInit) {
            IMLog.w(TAG, "Has initSDK");
            return true;
        }

        if (null == config) {
            config = new V2TIMSDKConfig();
        }

        IMContext.getInstance().init(context.getApplicationContext());

        getFieldByReflection(V2TIMSDKConfig.class,"sdkInitPath",config, SystemUtil.getSDKInitPath());
        getFieldByReflection(V2TIMSDKConfig.class,"sdkLogPath",config,SystemUtil.getSDKLogPath());

        isInit = nativeInitSDK(sdkAppID,config);
        IMLog.e(TAG,"init flag = "+isInit);
       return isInit;
    }

    @Override
    public void unInitSDK() {
        nativeUninitSDK();
        isInit = false;
    }

    @Override
    public void addIMSDKListener(V2TIMSDKListener listener) {
        nativeAddSDKListener(listener,listener.toString());
    }

    @Override
    public void removeIMSDKListener(V2TIMSDKListener listener) {
        nativeRemoveSDKListener(listener.toString());
    }

    @Override
    public String getVersion() {
        return nativeGetVersion();
    }

    @Override
    public long getServerTime() {
        return nativeGetServerTime();
    }

    @Override
    public void login(String userID, String userSig, V2TIMCallback _callback_) {
        if (!isInit){
            if (_callback_ != null){
                _callback_.onError(BaseConstants.ERR_SDK_NOT_INITIALIZED,"sdk not init");
            }
            return;
        }
        if (TextUtils.isEmpty(userID) || TextUtils.isEmpty(userSig)) {
            _callback_.onError(BaseConstants.ERR_INVALID_PARAMETERS, "userID or userSig is empty");
        }
        nativeLogin(userID, userSig, new IMCallback(_callback_) {
            @Override
            public void success(Object data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });
    }

    @Override
    public void logout(V2TIMCallback _callback_) {
        nativeLogout(new IMCallback(_callback_) {
            @Override
            public void success(Object data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });
    }

    @Override
    public String getLoginUser() {
        return nativeGetLoginUser();
    }

    @Override
    public int getLoginStatus() {
        return nativeGetLoginStatus();
    }

    @Override
    public void addSimpleMsgListener(V2TIMSimpleMsgListener v2TIMSimpleMsgListener) {
        nativeAddSimpleMsgListener(v2TIMSimpleMsgListener,v2TIMSimpleMsgListener.toString());
    }

    @Override
    public void removeSimpleMsgListener(V2TIMSimpleMsgListener v2TIMSimpleMsgListener) {
        nativeRemoveSimpleMsgListener(v2TIMSimpleMsgListener.toString());
    }

    @Override
    public String sendC2CTextMessage(String text, String userID, V2TIMValueCallback<V2TIMMessage> _callback_) {
        return nativeSendC2CTextMessage(text, userID, new IMCallback<V2TIMMessage>(_callback_) {
            @Override
            public void success(V2TIMMessage data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });
    }

    @Override
    public String sendC2CCustomMessage(byte[] customData, String userID, V2TIMValueCallback<V2TIMMessage> _callback_) {
        return nativeSendC2CCustomMessage(customData, userID, new IMCallback<V2TIMMessage>(_callback_) {
            @Override
            public void success(V2TIMMessage data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });
    }

    @Override
    public String sendGroupTextMessage(String text, String groupID, int priority, V2TIMValueCallback<V2TIMMessage> _callback_) {
        return nativeSendGroupTextMessage(text, groupID, priority, new IMCallback<V2TIMMessage>(_callback_) {
            @Override
            public void success(V2TIMMessage data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });
    }

    @Override
    public String sendGroupCustomMessage(byte[] customData, String groupID, int priority, V2TIMValueCallback<V2TIMMessage> _callback_) {
        return nativeSendGroupCustomMessage(customData, groupID, priority, new IMCallback<V2TIMMessage>(_callback_) {
            @Override
            public void success(V2TIMMessage data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });
    }

    @Override
    public void addGroupListener(V2TIMGroupListener listener) {
        nativeAddGroupListener(listener,listener.toString());
    }

    @Override
    public void removeGroupListener(V2TIMGroupListener listener) {
        nativeRemoveGroupListener(listener.toString());
    }

    @Override
    public void createGroup(String groupType, String groupID, String groupName, V2TIMValueCallback<String> _callback_) {
        nativeCreateGroup(groupType, groupID, groupName, new IMCallback<String>(_callback_) {
            @Override
            public void success(String data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });
    }

    @Override
    public void joinGroup(String groupID, String message, V2TIMCallback _callback_) {
        nativeJoinGroup(groupID, message, new IMCallback(_callback_) {
            @Override
            public void success(Object data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });
    }

    @Override
    public void quitGroup(String groupID, V2TIMCallback _callback_) {
        nativeQuitGroup(groupID, new IMCallback(_callback_) {
            @Override
            public void success(Object data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });
    }

    @Override
    public void dismissGroup(String groupID, V2TIMCallback _callback_) {
        nativeDismissGroup(groupID, new IMCallback(_callback_) {
            @Override
            public void success(Object data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });
    }

    @Override
    public void getUsersInfo(List<String> userIDList, V2TIMValueCallback<List<V2TIMUserFullInfo>> _callback_) {
        nativeGetUsersInfo(userIDList, new IMCallback<List<V2TIMUserFullInfo>>(_callback_) {
            @Override
            public void success(List<V2TIMUserFullInfo> data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });
    }

    @Override
    public void setSelfInfo(V2TIMUserFullInfo info, V2TIMCallback _callback_) {
        nativeSetSelfInfo(info, new IMCallback(_callback_) {
            @Override
            public void success(Object data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });
    }

    @Override
    public void subscribeUserInfo(List<String> userIDList, V2TIMCallback _callback_) {
        nativeSubscribeUserInfo(userIDList, new IMCallback(_callback_) {
            @Override
            public void success(Object data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });
    }

    @Override
    public void unsubscribeUserInfo(List<String> userIDList, V2TIMCallback _callback_) {
        nativeUnsubscribeUserInfo(userIDList, new IMCallback(_callback_) {
            @Override
            public void success(Object data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });
    }

    @Override
    public void getUserStatus(List<String> userIDList, V2TIMValueCallback<List<V2TIMUserStatus>> _callback_) {
        nativeGetUserStatus(userIDList, new IMCallback<List<V2TIMUserStatus>>(_callback_) {
            @Override
            public void success(List<V2TIMUserStatus> data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });
    }

    @Override
    public void setSelfStatus(V2TIMUserStatus status, V2TIMCallback _callback_) {
        nativeSetSelfStatus(status, new IMCallback(_callback_) {
            @Override
            public void success(Object data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });
    }

    @Override
    public void subscribeUserStatus(List<String> userIDList, V2TIMCallback _callback_) {
        nativeSubscribeUserStatus(userIDList, new IMCallback(_callback_) {
            @Override
            public void success(Object data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });
    }

    @Override
    public void unsubscribeUserStatus(List<String> userIDList, V2TIMCallback _callback_) {
        nativeUnsubscribeUserStatus(userIDList, new IMCallback(_callback_) {
            @Override
            public void success(Object data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });
    }

    @Override
    public void callExperimentalAPI(String api, Object param, V2TIMValueCallback<Object> _callback_) {
        if (TextUtils.isEmpty(api)) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "invalid api");
            return;
        }

        if (api.equals("setCustomServerInfo")) {
            setCustomServerInfo(api,param, _callback_);
        } else if (api.equals("setQuicChannelInfo")) {
            setQuicChannelInfo(api, param, _callback_);
        } else if (api.equals("setProxyInfo")) {
            setProxyInfo(api, param, _callback_);
        } else if (api.equals("initLocalStorage")) {
            initLocalStorage(api, param, _callback_);
        } else if (api.equals("setTestEnvironment")) {
            setTestEnvironment(api, param, _callback_);
        } else if (api.equals("setIPv6Prior")) {
            setIPv6Prior(api, param, _callback_);
        } else if (api.equals("setCosSaveRegionForConversation")) {
            setCosSaveRegion(api, param, _callback_);
        } else if (api.equals("setUIPlatform")) {
            setUIPlatform(api, param, _callback_);
        } else if (api.equals("setBuildInfo")) {
            setBuildInfo(api, param, _callback_);
        } else if (api.equals("setDatabaseEncryptInfo")) {
            setDatabaseEncryptInfo(api, param, _callback_);
        } else if (api.equals("isCommercialAbilityEnabled")) {
            isCommercialAbilityEnabled(api, param, _callback_);
        } else if (api.equals("setPacketRetryInfo")) {
            setPacketRetryInfo(api, param, _callback_);
        } else if (api.equals("setOfflinePushState")) {
            setOfflinePushState(api, param, _callback_);
        } else if (api.equals("getOfflinePushState")) {
            getOfflinePushState(api,_callback_);
        } else if (api.equals("getMessageRevoker")) {
            getMessageRevoker(api, param, _callback_);
        } else if (api.equals("writeLog")) {
            writeLog(api, param, _callback_);
        } else if (api.equals("sendTRTCCustomData")) {
            sendTRTCCustomData(api, param, _callback_);
        } else if (api.equals("clearLocalHistoryMessage")) {
            clearLocalHistoryMessage(api, param, _callback_);
        } else if (api.equals("reportTUIComponentUsage")) {
            reportTUIComponentUsage(api, param, _callback_);
        } else if (api.equals("setApplicationID")) {
            setApplicationID(api, param, _callback_);
        } else {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "unsupported api");
        }
    }

    private void setCustomServerInfo(String apiTitle , Object param, V2TIMValueCallback<Object> _callback_) {
        if (null == param || !(param instanceof String)) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is not string");
            return;
        }

        String json = (String) param;
        if (TextUtils.isEmpty(json)) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is empty");
            return;
        }

        nativeCallExperimentalAPI(apiTitle, null, new IMCallback<Object>(_callback_) {
            @Override
            public void success(Object data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });

//        CustomServerInfo customServerInfo = new CustomServerInfo();
//        try {
//            // 仅长连接支持 Quic 通道
//            JSONObject jsonObject = new JSONObject(json);
//            JSONArray longconnectionAddressList = jsonObject.optJSONArray("longconnectionAddressList");
//            if (longconnectionAddressList != null && longconnectionAddressList.length() > 0) {
//                ArrayList list = new ArrayList();
//                for (int i = 0; i < longconnectionAddressList.length(); i++) {
//                    JSONObject address = longconnectionAddressList.getJSONObject(i);
//                    CustomServerInfo.ServerAddress serverAddress = new CustomServerInfo.ServerAddress();
//                    serverAddress.ip = address.optString("ip");
//                    serverAddress.port = address.optInt("port");
//                    serverAddress.isIPv6 = (address.has("isIPv6") ? address.optBoolean("isIPv6") : false);
//                    serverAddress.isQuic = (address.has("isQuic") ? address.optBoolean("isQuic") : false);
//
//                    list.add(serverAddress);
//                }
//                customServerInfo.longconnectionAddressList = list;
//            }
//
//            JSONArray shortconnectionAddressList = jsonObject.optJSONArray("shortconnectionAddressList");
//            if (shortconnectionAddressList != null && shortconnectionAddressList.length() > 0) {
//                ArrayList list = new ArrayList();
//                for (int i = 0; i < shortconnectionAddressList.length(); i++) {
//                    JSONObject address = shortconnectionAddressList.getJSONObject(i);
//                    CustomServerInfo.ServerAddress serverAddress = new CustomServerInfo.ServerAddress();
//                    serverAddress.ip = address.optString("ip");
//                    serverAddress.port = address.optInt("port");
//                    serverAddress.isIPv6 = (address.has("isIPv6") ? address.optBoolean("isIPv6") : false);
//
//                    list.add(serverAddress);
//                }
//                customServerInfo.shortconnectionAddressList = list;
//            }
//
//            customServerInfo.serverPublicKey = jsonObject.optString("serverPublicKey");
//
//            BaseManager.getInstance().setCustomServerInfo(customServerInfo);
//        } catch (JSONException e) {
//            callbackOnError(callback, BaseConstants.ERR_INVALID_PARAMETERS, "convert param to json failed");
//            e.printStackTrace();
//        }

    }

    private void setQuicChannelInfo(String apiTitle , Object param, V2TIMValueCallback<Object> _callback_) {
        if (null == param || !(param instanceof String)) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is not string");
            return;
        }

        String json = (String) param;
        if (TextUtils.isEmpty(json)) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is empty");
            return;
        }

        nativeCallExperimentalAPI(apiTitle, null, new IMCallback<Object>(_callback_) {
            @Override
            public void success(Object data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });

//        try {
//            JSONObject jsonObject = new JSONObject(json);
//            boolean forceUseQuicChannel = jsonObject.optBoolean("forceUseQuicChannel");
//
//            BaseManager.getInstance().setForceUseQuicChannel(forceUseQuicChannel);
//
//            callbackOnSuccess(callback, null);
//        } catch (JSONException e) {
//            callbackOnError(callback, BaseConstants.ERR_INVALID_PARAMETERS, "convert param to json failed");
//            e.printStackTrace();
//        }
    }

    private void setProxyInfo(String apiTitle , Object param, V2TIMValueCallback<Object> _callback_) {
        if (null == param || !(param instanceof String)) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is not string");
            return;
        }

        String json = (String) param;
        if (TextUtils.isEmpty(json)) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is empty");
            return;
        }

        nativeCallExperimentalAPI(apiTitle, null, new IMCallback<Object>(_callback_) {
            @Override
            public void success(Object data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });

//        try {
//            JSONObject jsonObject = new JSONObject(json);
//            if (!jsonObject.has("proxyType")) {
//                callbackOnError(callback, BaseConstants.ERR_INVALID_PARAMETERS, "proxyType must be set");
//                return;
//            }
//
//            // http 和 socks5 代理，需要设置 host 和 port，user_name 和 password 可选
//            // pac 代理，不需要设置 host 和 port，user_name 和 password 可选
//            // 因此，host 和 port，要么都设置，要么都不设置
//            if ((jsonObject.has("proxyHost") && !jsonObject.has("proxyPort"))
//                    || (!jsonObject.has("proxyHost") && jsonObject.has("proxyPort"))) {
//                callbackOnError(callback, BaseConstants.ERR_INVALID_PARAMETERS,
//                        "proxyHost and proxyPort must be set together if need");
//                return;
//            }
//
//            SDKConfig.ProxyInfo proxyInfo = new SDKConfig.ProxyInfo();
//            proxyInfo.proxyType = jsonObject.optInt("proxyType");
//
//            if (jsonObject.has("proxyHost") && jsonObject.has("proxyPort")) {
//                proxyInfo.proxyHost = jsonObject.optString("proxyHost");
//                proxyInfo.proxyPort = jsonObject.optInt("proxyPort");
//            }
//
//            if (jsonObject.has("proxyUsername")) {
//                proxyInfo.proxyUsername = jsonObject.optString("proxyUsername");
//            }
//
//            if (jsonObject.has("proxyPassword")) {
//                proxyInfo.proxyPassword = jsonObject.optString("proxyPassword");
//            }
//
//            BaseManager.getInstance().setProxyInfo(proxyInfo);
//
//            callbackOnSuccess(callback, null);
//        } catch (JSONException e) {
//            callbackOnError(callback, BaseConstants.ERR_INVALID_PARAMETERS, "convert param to json failed");
//            e.printStackTrace();
//        }
    }

    private void initLocalStorage(String apiTitle , Object param, V2TIMValueCallback<Object> _callback_) {
        if (!(param instanceof String)) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is not string");
            return;
        }

        String userID = (String) param;
        if (TextUtils.isEmpty(userID)) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "invalid userID");
            return;
        }

        nativeCallExperimentalAPI(apiTitle, null, new IMCallback<Object>(_callback_) {
            @Override
            public void success(Object data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });

//        BaseManager.getInstance().initLocalStorage(userID, new IMCallback(callback) {
//            @Override
//            public void success(Object data) {
//                super.success(data);
//            }
//
//            @Override
//            public void fail(int code, String errorMessage) {
//                super.fail(code, errorMessage);
//            }
//        });
    }

    private void setTestEnvironment(String apiTitle , Object param, V2TIMValueCallback<Object> _callback_) {
        if (param != null && param instanceof Boolean) {
            nativeCallExperimentalAPI(apiTitle, param, new IMCallback<Object>(_callback_) {
                @Override
                public void success(Object data) {
                    super.success(data);
                }

                @Override
                public void fail(int code, String errorMessage) {
                    super.fail(code, errorMessage);
                }
            });
        }
    }

    private void setIPv6Prior(String apiTitle , Object param, V2TIMValueCallback<Object> _callback_) {
        if (param != null && param instanceof Boolean) {
            nativeCallExperimentalAPI(apiTitle, param, new IMCallback<Object>(_callback_) {
                @Override
                public void success(Object data) {
                    super.success(data);
                }

                @Override
                public void fail(int code, String errorMessage) {
                    super.fail(code, errorMessage);
                }
            });
        }
    }

    private void setCosSaveRegion(String apiTitle , Object param, V2TIMValueCallback<Object> _callback_) {
        if (null == param || !(param instanceof String)) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is not string");
            return;
        }

        String json = (String) param;
        if (TextUtils.isEmpty(json)) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is empty");
            return;
        }

        nativeCallExperimentalAPI(apiTitle, null, new IMCallback<Object>(_callback_) {
            @Override
            public void success(Object data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });

//        try {
//            JSONObject jsonObject = new JSONObject(json);
//            String conversationID = jsonObject.optString("conversationID");
//            String cosSaveRegion = jsonObject.optString("cosSaveRegion");
//            if (TextUtils.isEmpty(conversationID) || TextUtils.isEmpty(cosSaveRegion)) {
//                callbackOnError(callback, BaseConstants.ERR_INVALID_PARAMETERS, "invalid param");
//                return;
//            }
//
//            ConversationKey conversationKey =
//                    V2TIMConversationManagerImpl.getInstance().getConversationKey(conversationID);
//            ConversationManager.getInstance().setCosSaveRegionForConversation(
//                    conversationKey, cosSaveRegion, new IMCallback(callback) {
//                        @Override
//                        public void success(Object data) {
//                            super.success(data);
//                        }
//
//                        @Override
//                        public void fail(int code, String errorMessage) {
//                            super.fail(code, errorMessage);
//                        }
//                    });
//
//        } catch (JSONException e) {
//            e.printStackTrace();
//        }
    }

    private void setUIPlatform(String apiTitle , Object param, V2TIMValueCallback<Object> _callback_) {
        if (null == param) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is null");
            return;
        }

        String stringUIPlatform = "";
        int numberUIPlatform = 0;
        if (param instanceof String) {
            stringUIPlatform = (String) param;
        } else if (param instanceof Integer) {
            numberUIPlatform = (Integer) param;
        } else {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is invalid");
        }

        nativeCallExperimentalAPI(apiTitle, numberUIPlatform, new IMCallback<Object>(_callback_) {
            @Override
            public void success(Object data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });

//
//        BaseManager.getInstance().setCustomUIPlatform(stringUIPlatform, numberUIPlatform);
//
//        callbackOnSuccess(callback, null);
    }

    private void setBuildInfo(String apiTitle , Object param, V2TIMValueCallback<Object> _callback_) {
        if (null == param || !(param instanceof String)) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is not string");
            return;
        }

        String json = (String) param;
        if (TextUtils.isEmpty(json)) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is empty");
            return;
        }
        nativeCallExperimentalAPI(apiTitle, null, new IMCallback<Object>(_callback_) {
            @Override
            public void success(Object data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });

//        try {
//            JSONObject jsonObject = new JSONObject(json);
//            String buildBrand = jsonObject.optString("buildBrand");
//            String buildManufacturer = jsonObject.optString("buildManufacturer");
//            String buildModel = jsonObject.optString("buildModel");
//            String buildVersionRelease = jsonObject.optString("buildVersionRelease");
//            int buildVersionSDKInt = jsonObject.optInt("buildVersionSDKInt");
//
//            SystemUtil.setBuildBrand(buildBrand);
//            SystemUtil.setBuildManufacturer(buildManufacturer);
//            SystemUtil.setBuildModel(buildModel);
//            SystemUtil.setBuildVersionRelease(buildVersionRelease);
//            SystemUtil.setBuildVersionSDKInt(buildVersionSDKInt);
//
//            callbackOnSuccess(callback, null);
//        } catch (JSONException e) {
//            callbackOnError(callback, BaseConstants.ERR_INVALID_PARAMETERS, "convert param to json failed");
//            e.printStackTrace();
//        }
    }

    private void setDatabaseEncryptInfo(String apiTitle , Object param, V2TIMValueCallback<Object> _callback_) {
        if (null == param || !(param instanceof String)) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is not string");
            return;
        }

        String json = (String) param;
        if (TextUtils.isEmpty(json)) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is empty");
            return;
        }

        nativeCallExperimentalAPI(apiTitle, null, new IMCallback<Object>(_callback_) {
            @Override
            public void success(Object data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });

//        try {
//            JSONObject jsonObject = new JSONObject(json);
//            if (!jsonObject.has("encryptType") || !jsonObject.has("encryptKey")) {
//                callbackOnError(
//                        callback, BaseConstants.ERR_INVALID_PARAMETERS, "encryptType and encryptKey must be set");
//                return;
//            }
//
//            SDKConfig.DatabaseEncryptInfo databaseEncryptInfo = new SDKConfig.DatabaseEncryptInfo();
//            databaseEncryptInfo.encryptType = jsonObject.optInt("encryptType");
//            databaseEncryptInfo.encryptKey = jsonObject.optString("encryptKey");
//
//            BaseManager.getInstance().setDatabaseEncryptInfo(databaseEncryptInfo);
//
//            callbackOnSuccess(callback, null);
//        } catch (JSONException e) {
//            callbackOnError(callback, BaseConstants.ERR_INVALID_PARAMETERS, "convert param to json failed");
//            e.printStackTrace();
//        }
    }

    private void isCommercialAbilityEnabled(String apiTitle , Object param, V2TIMValueCallback<Object> _callback_) {
        if (null == param || !(param instanceof Long)) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is not int");
            return;
        }

        nativeCallExperimentalAPI(apiTitle, null, new IMCallback<Object>(_callback_) {
            @Override
            public void success(Object data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });

//        Long longParam = (Long) param;
//        BaseManager.getInstance().isCommercialAbilityEnabled(longParam.longValue(), new IMCallback<Object>(callback) {
//            @Override
//            public void success(Object data) {
//                super.success(data);
//            }
//
//            @Override
//            public void fail(int code, String errorMessage) {
//                super.fail(code, errorMessage);
//            }
//        });
    }

    private void setPacketRetryInfo(String apiTitle , Object param, V2TIMValueCallback<Object> _callback_) {
        if (null == param || !(param instanceof String)) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is not string");
            return;
        }

        String json = (String) param;
        if (TextUtils.isEmpty(json)) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is empty");
            return;
        }

        nativeCallExperimentalAPI(apiTitle, null, new IMCallback<Object>(_callback_) {
            @Override
            public void success(Object data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });

//        try {
//            JSONObject jsonObject = new JSONObject(json);
//            SDKConfig.PacketRetryInfo info = new SDKConfig.PacketRetryInfo();
//
//            if (jsonObject.has("maxRetryCount")) {
//                info.maxRetryCount = jsonObject.optInt("maxRetryCount");
//            }
//
//            if (jsonObject.has("packetRequestTimeout")) {
//                info.packetRequestTimeout = jsonObject.optInt("packetRequestTimeout");
//            }
//
//            BaseManager.getInstance().setPacketRetryInfo(info);
//
//            callbackOnSuccess(callback, null);
//        } catch (JSONException e) {
//            callbackOnError(callback, BaseConstants.ERR_INVALID_PARAMETERS, "convert param to json failed");
//            e.printStackTrace();
//        }
    }

    private void setOfflinePushState(String apiTitle , Object param, V2TIMValueCallback<Object> _callback_) {
        if (null == param || !(param instanceof Integer)) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is not int");
            return;
        }

        nativeCallExperimentalAPI(apiTitle, null, new IMCallback<Object>(_callback_) {
            @Override
            public void success(Object data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });

//        OfflinePushConfig pushConfig = new OfflinePushConfig();
//        pushConfig.setOpenOfflinePush((int) param);
//        OfflinePushManager.getInstance().setOfflinePushConfig(pushConfig, new IMCallback(callback) {
//            @Override
//            public void success(Object data) {
//                super.success(data);
//            }
//
//            @Override
//            public void fail(int code, String errorMessage) {
//                super.fail(code, errorMessage);
//            }
//        });
    }

    private void getOfflinePushState(String apiTitle,V2TIMValueCallback<Object> valueCallback) {
        nativeCallExperimentalAPI(apiTitle, null, new IMCallback<Object>(valueCallback) {
            @Override
            public void success(Object data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });
//        OfflinePushManager.getInstance().getOfflinePushConfig(new IMCallback(valueCallback) {
//            @Override
//            public void success(Object data) {
//                OfflinePushConfig pushConfig = (OfflinePushConfig) data;
//                final int openPush = pushConfig.getOpenOfflinePush();
//                IMContext.getInstance().runOnMainThread(new Runnable() {
//                    @Override
//                    public void run() {
//                        if (valueCallback != null) {
//                            valueCallback.onSuccess(openPush);
//                        }
//                    }
//                });
//            }
//
//            @Override
//            public void fail(int code, String errorMessage) {
//                super.fail(code, errorMessage);
//            }
//        });
    }

    private void getMessageRevoker(String apiTitle , Object param, final V2TIMValueCallback<Object> _callback_) {
        if (!(param instanceof List)) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is not list");
            return;
        }
        List messageIDList = (List) param;
        for (Object messageID : messageIDList) {
            if (!(messageID instanceof String)) {
                callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "item is no String type");
                return;
            }
            break;
        }

        V2TIMValueCallback<List<V2TIMMessage>> v2TIMValueCallback = new V2TIMValueCallback<List<V2TIMMessage>>() {
            @Override
            public void onSuccess(List<V2TIMMessage> messageList) {
                if (messageList.size() == 0) {
                    if (_callback_ != null) {
                        _callback_.onError(BaseConstants.ERR_INVALID_PARAMETERS, "local messages do not exist");
                    }
                } else {
                    if (_callback_ != null) {
                        HashMap<String, String> revokerMap = new HashMap<>();
                        for (V2TIMMessage message : messageList) {
                            revokerMap.put(message.getMsgID(), message.getRevokerInfo().getUserID());
                        }
                        _callback_.onSuccess(revokerMap);
                    }
                }
            }

            @Override
            public void onError(int code, String desc) {
                if (_callback_ != null) {
                    _callback_.onError(code, desc);
                }
            }
        };

        nativeCallExperimentalAPI(apiTitle, null, new IMCallback<Object>(_callback_) {
            @Override
            public void success(Object data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });

//        MessageCenter.getInstance().findMessageByMessageId(
//                messageIDList, new IMCallback<List<Message>>(v2TIMValueCallback) {
//                    @Override
//                    public void success(List<Message> data) {
//                        super.success(data);
//                    }
//
//                    @Override
//                    public void fail(int code, String errorMessage) {
//                        super.fail(code, errorMessage);
//                    }
//                });
    }

    private void writeLog(String apiTitle , Object param, final V2TIMValueCallback<Object> _callback_) {
        if (null == param || !(param instanceof String)) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is not string");
            return;
        }

        String json = (String) param;
        if (TextUtils.isEmpty(json)) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is empty");
            return;
        }

        nativeCallExperimentalAPI(apiTitle, null, new IMCallback<Object>(_callback_) {
            @Override
            public void success(Object data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });

//        try {
//            JSONObject jsonObject = new JSONObject(json);
//            int logLevel = jsonObject.optInt("logLevel");
//            String logContent = jsonObject.optString("logContent");
//            String fileName = jsonObject.optString("fileName");
//
//            switch (logLevel) {
//                case IMLog.LOG_LEVEL_VERBOSE:
//                    IMLog.v(fileName, logContent);
//                    break;
//                case IMLog.LOG_LEVEL_DEBUG:
//                    IMLog.d(fileName, logContent);
//                    break;
//                case IMLog.LOG_LEVEL_INFO:
//                    IMLog.i(fileName, logContent);
//                    break;
//                case IMLog.LOG_LEVEL_WARN:
//                    IMLog.w(fileName, logContent);
//                    break;
//                case IMLog.LOG_LEVEL_ERROR:
//                    IMLog.e(fileName, logContent);
//                    break;
//                default:
//                    callbackOnError(callback, BaseConstants.ERR_INVALID_PARAMETERS, "invalid logLevel");
//                    return;
//            }
//
//            callbackOnSuccess(callback, null);
//        } catch (JSONException e) {
//            callbackOnError(callback, BaseConstants.ERR_INVALID_PARAMETERS, "convert param to json failed");
//            e.printStackTrace();
//        }
    }

    private void sendTRTCCustomData(String apiTitle , Object param, V2TIMValueCallback<Object> _callback_) {
        if (null == param || !(param instanceof byte[])) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is not byte array");
            return;
        }

        nativeCallExperimentalAPI(apiTitle, param, new IMCallback<Object>(_callback_) {
            @Override
            public void success(Object data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });

//        byte[] data = (byte[]) param;
//
//        BaseManager.getInstance().sendTRTCCustomData(data, new IMCallback(callback) {
//            @Override
//            public void success(Object data) {
//                super.success(data);
//            }
//
//            @Override
//            public void fail(int code, String errorMessage) {
//                super.fail(code, errorMessage);
//            }
//        });
    }

    private void clearLocalHistoryMessage(String apiTitle , Object param, final V2TIMValueCallback<Object> _callback_) {
        if (null == param || !(param instanceof String)) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is not string");
            return;
        }

        String json = (String) param;
        if (TextUtils.isEmpty(json)) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is empty");
            return;
        }

        nativeCallExperimentalAPI(apiTitle, param, new IMCallback<Object>(_callback_) {
            @Override
            public void success(Object data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });

//        try {
//            JSONObject jsonObject = new JSONObject(json);
//            String conversationID = jsonObject.optString("conversationID");
//            ConversationKey conversationKey =
//                    V2TIMConversationManagerImpl.getInstance().getConversationKey(conversationID);
//            long beginTimestamp = jsonObject.optLong("beginTimestamp");
//            long endTimestamp = jsonObject.optLong("endTimestamp");
//
//            MessageCenter.getInstance().clearLocalHistoryMessage(
//                    conversationKey, beginTimestamp, endTimestamp, new IMCallback(callback) {
//                        @Override
//                        public void success(Object data) {
//                            super.success(data);
//                        }
//
//                        @Override
//                        public void fail(int code, String errorMessage) {
//                            super.fail(code, errorMessage);
//                        }
//                    });
//            callbackOnSuccess(callback, null);
//        } catch (JSONException e) {
//            callbackOnError(callback, BaseConstants.ERR_INVALID_PARAMETERS, "convert param to json failed");
//            e.printStackTrace();
//        }
    }

    private void reportTUIComponentUsage(String apiTitle , Object param, final V2TIMValueCallback<Object> _callback_) {
        if (null == param || !(param instanceof String)) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is not string");
            return;
        }

        String json = (String) param;
        if (TextUtils.isEmpty(json)) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is empty");
            return;
        }

        nativeCallExperimentalAPI(apiTitle, param, new IMCallback<Object>(_callback_) {
            @Override
            public void success(Object data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });

//        try {
//            JSONObject jsonObject = new JSONObject(json);
//            long uiComponentType = jsonObject.optLong("UIComponentType");
//            long uiStyleType = jsonObject.optLong("UIStyleType");
//
//            BaseManager.getInstance().reportTUIComponentUsage(uiComponentType, uiStyleType);
//
//            callbackOnSuccess(callback, null);
//        } catch (JSONException e) {
//            callbackOnError(callback, BaseConstants.ERR_INVALID_PARAMETERS, "convert param to json failed");
//            e.printStackTrace();
//        }


    }

    private void setApplicationID(String apiTitle , Object param, V2TIMValueCallback<Object> _callback_) {
        if (param != null && param instanceof Integer) {
//            BaseManager.getInstance().setApplicationID(((Integer) param).intValue());
            nativeCallExperimentalAPI(apiTitle, null, new IMCallback<Object>(_callback_) {
                @Override
                public void success(Object data) {
                    super.success(data);
                }

                @Override
                public void fail(int code, String errorMessage) {
                    super.fail(code, errorMessage);
                }
            });
        }

    }

    private void callbackOnError(V2TIMValueCallback<Object> callback, int code, String desc) {
        if (callback != null) {
            callback.onError(code, desc);
        }
    }

    private void callbackOnSuccess(V2TIMValueCallback<Object> callback, Object result) {
        if (callback != null) {
            callback.onSuccess(result);
        }
    }

    private static void getFieldByReflection(final Class<?> clazz, final String fieldName, Object obj, Object value){
        try {
            Field path = clazz.getDeclaredField(fieldName);
            path.setAccessible(true);//启用私有变量可赋值权限
            path.set(obj,value);
        } catch (@SuppressLint({"NewApi", "LocalSuppress"}) NoSuchFieldException | IllegalAccessException e) {
            e.printStackTrace();
        }
    }

}
