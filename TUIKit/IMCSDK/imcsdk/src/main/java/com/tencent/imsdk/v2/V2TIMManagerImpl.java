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
            setCustomServerInfo(param, _callback_);
        } else if (api.equals("setQuicChannelInfo")) {
            setQuicChannelInfo(param, _callback_);
        } else if (api.equals("setProxyInfo")) {
            setProxyInfo(param, _callback_);
        } else if (api.equals("initLocalStorage")) {
            initLocalStorage(param, _callback_);
        } else if (api.equals("setTestEnvironment")) {
            setTestEnvironment(param, _callback_);
        } else if (api.equals("setIPv6Prior")) {
            setIPv6Prior(param, _callback_);
        } else if (api.equals("setCosSaveRegionForConversation")) {
            setCosSaveRegion(param, _callback_);
        } else if (api.equals("setUIPlatform")) {
            setUIPlatform(param, _callback_);
        } else if (api.equals("setBuildInfo")) {
            setBuildInfo(param, _callback_);
        } else if (api.equals("setDatabaseEncryptInfo")) {
            setDatabaseEncryptInfo(param, _callback_);
        } else if (api.equals("isCommercialAbilityEnabled")) {
            isCommercialAbilityEnabled(param, _callback_);
        } else if (api.equals("setPacketRetryInfo")) {
            setPacketRetryInfo(param, _callback_);
        } else if (api.equals("setOfflinePushState")) {
            setOfflinePushState(param, _callback_);
        } else if (api.equals("getOfflinePushState")) {
            getOfflinePushState(_callback_);
        } else if (api.equals("getMessageRevoker")) {
            getMessageRevoker(param, _callback_);
        } else if (api.equals("writeLog")) {
            writeLog(param, _callback_);
        } else if (api.equals("getAIDenoiseSignature")) {
//            getAIDenoiseSignature(_callback_);
        } else if (api.equals("sendTRTCCustomData")) {
            sendTRTCCustomData(param, _callback_);
        } else if (api.equals("setMessageExtensionsBySequence")) {
//            setMessageExtensionsBySequence(param, _callback_);
        } else if (api.equals("getMessageExtensionsBySequence")) {
//            getMessageExtensionsBySequence(param, _callback_);
        } else if (api.equals("clearLocalHistoryMessage")) {
            clearLocalHistoryMessage(param, _callback_);
        } else if (api.equals("reportTUIComponentUsage")) {
            reportTUIComponentUsage(param, _callback_);
        } else if (api.equals("setConversationGroupOrderData")) {
//            setConversationGroupOrderData(param, _callback_);
        } else if (api.equals("getConversationGroupOrderData")) {
//            getConversationGroupOrderData(param, _callback_);
        } else if (api.equals("reportOfflinePushEvent")) {
//            reportOfflinePushEvent(param, _callback_);
        } else if (api.equals("setOfflinePushInfo")) {
//            setOfflinePushInfo(param, _callback_);
        } else if (api.equals("setApplicationID")) {
            setApplicationID(param, _callback_);
        } else {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "unsupported api");
        }
    }

//    private void setConversationGroupOrderData(Object param, final V2TIMValueCallback<Object> _callback_) {
//        if (null == param || !(param instanceof String)) {
//            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is not string");
//            return;
//        }
//        String apiTitle = "internal_operation_is_commercial_ability_enabled";
//        nativeCallExperimentalAPI(apiTitle, param, new IMCallback<Object>(_callback_) {
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
//    }

//    private void getConversationGroupOrderData(Object param, final V2TIMValueCallback<Object> _callback_) {
//        V2TIMValueCallback<String> v2TIMValueCallback = new V2TIMValueCallback<String>() {
//            @Override
//            public void onSuccess(String orderData) {
//                callback.onSuccess(orderData);
//            }
//
//            @Override
//            public void onError(int code, String desc) {
//                if (callback != null) {
//                    callback.onError(code, desc);
//                }
//            }
//        };
//
////        ConversationManager.getInstance().getConversationGroupOrderData(
////                new IMCallback<String>(v2TIMValueCallback) {
////                    @Override
////                    public void success(String orderData) {
////                        super.success(orderData);
////                    }
////
////                    @Override
////                    public void fail(int code, String errorMessage) {
////                        super.fail(code, errorMessage);
////                    }
////                });
//
//        String apiTitle = "";
//        nativeCallExperimentalAPI(apiTitle, param, new IMCallback<Object>(_callback_) {
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
//    }

    private void reportTUIComponentUsage(Object param, final V2TIMValueCallback<Object> _callback_) {
        if (null == param || !(param instanceof String)) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is not string");
            return;
        }

        String json = (String) param;
        if (TextUtils.isEmpty(json)) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is empty");
            return;
        }

        String apiTitle = "internal_operation_report_tuicomponent_usage";
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

//    private void setMessageExtensionsBySequence(Object param, final V2TIMValueCallback<Object> _callback_) {
//        if (null == param || !(param instanceof String)) {
//            callbackOnError(callback, BaseConstants.ERR_INVALID_PARAMETERS, "param is not string");
//            return;
//        }
//
//        String json = (String) param;
//        if (TextUtils.isEmpty(json)) {
//            callbackOnError(callback, BaseConstants.ERR_INVALID_PARAMETERS, "param is empty");
//            return;
//        }
//
//        String apiTitle = "";
//        nativeCallExperimentalAPI(apiTitle, param, new IMCallback<Object>(_callback_) {
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

//        String groupID = "";
//        long messageSequence = 0;
//        List<MessageExtension> extensionList = new ArrayList<>();
//        try {
//            JSONObject jsonObject = new JSONObject(json);
//            groupID = jsonObject.optString("groupID");
//            messageSequence = jsonObject.optLong("messageSequence");
//
//            JSONArray extensionJsonArray = jsonObject.optJSONArray("messageExtensionList");
//            for (int i = 0; i < extensionJsonArray.length(); i++) {
//                JSONObject extensionJson = extensionJsonArray.getJSONObject(i);
//                MessageExtension extension = new MessageExtension();
//                extension.setExtensionKey(extensionJson.optString("extensionKey"));
//                extension.setExtensionValue(extensionJson.optString("extensionValue"));
//                extensionList.add(extension);
//            }
//        } catch (JSONException e) {
//            e.printStackTrace();
//        }
//
//        final V2TIMValueCallback<List<MessageExtensionResult>> v2TIMValueCallback =
//                new V2TIMValueCallback<List<MessageExtensionResult>>() {
//                    @Override
//                    public void onSuccess(List<MessageExtensionResult> messageExtensionResults) {
//                        if (callback != null) {
//                            List<V2TIMMessageExtensionResult> v2TIMMessageExtensionResults = new ArrayList<>();
//                            for (MessageExtensionResult result : messageExtensionResults) {
//                                V2TIMMessageExtensionResult v2TIMMessageExtensionResult = new V2TIMMessageExtensionResult();
//                                v2TIMMessageExtensionResult.setMessageExtensionResult(result);
//                                v2TIMMessageExtensionResults.add(v2TIMMessageExtensionResult);
//                            }
//                            callback.onSuccess(v2TIMMessageExtensionResults);
//                        }
//                    }
//
//                    @Override
//                    public void onError(int code, String desc) {
//                        if (callback != null) {
//                            callback.onError(code, desc);
//                        }
//                    }
//                };
//
//        Message message = new Message();
//        message.setMessageStatus(Message.V2TIM_MSG_STATUS_SUCCESS);
//        message.setSupportMessageExtension(true);
//        message.addElement(new TextElement());
//        message.setMessageType(V2TIMConversation.V2TIM_GROUP);
//        message.setGroupID(groupID);
//        message.setSeq(messageSequence);
//
//        MessageCenter.getInstance().setMessageExtensions(
//                message, extensionList, new IMCallback<List<MessageExtensionResult>>(v2TIMValueCallback) {
//                    @Override
//                    public void success(List<MessageExtensionResult> data) {
//                        super.success(data);
//                    }
//
//                    @Override
//                    public void fail(int code, String errorMessage) {
//                        super.fail(code, errorMessage);
//                    }
//                });
//    }

//    private void getMessageExtensionsBySequence(Object param, final V2TIMValueCallback<Object> _callback_) {
//        if (null == param || !(param instanceof String)) {
//            callbackOnError(callback, BaseConstants.ERR_INVALID_PARAMETERS, "param is not string");
//            return;
//        }
//
//        String json = (String) param;
//        if (TextUtils.isEmpty(json)) {
//            callbackOnError(callback, BaseConstants.ERR_INVALID_PARAMETERS, "param is empty");
//            return;
//        }
//
//        String apiTitle = "";
//        nativeCallExperimentalAPI(apiTitle, param, new IMCallback<Object>(_callback_) {
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
//
////        String groupID = "";
////        long messageSequence = 0;
////        List<MessageExtension> extensionList = new ArrayList<>();
////        try {
////            JSONObject jsonObject = new JSONObject(json);
////            groupID = jsonObject.optString("groupID");
////            messageSequence = jsonObject.optLong("messageSequence");
////        } catch (JSONException e) {
////            e.printStackTrace();
////        }
////
////        final V2TIMValueCallback<List<MessageExtension>> v2TIMValueCallback =
////                new V2TIMValueCallback<List<MessageExtension>>() {
////                    @Override
////                    public void onSuccess(List<MessageExtension> extensions) {
////                        if (callback != null) {
////                            List<V2TIMMessageExtension> v2TIMMessageExtensions = new ArrayList<>();
////                            for (MessageExtension extension : extensions) {
////                                V2TIMMessageExtension v2TIMMessageExtension = new V2TIMMessageExtension();
////                                v2TIMMessageExtension.setMessageExtension(extension);
////                                v2TIMMessageExtensions.add(v2TIMMessageExtension);
////                            }
////                            callback.onSuccess(v2TIMMessageExtensions);
////                        }
////                    }
////
////                    @Override
////                    public void onError(int code, String desc) {
////                        if (callback != null) {
////                            callback.onError(code, desc);
////                        }
////                    }
////                };
////
////        Message message = new Message();
////        message.setMessageStatus(Message.V2TIM_MSG_STATUS_SUCCESS);
////        message.setSupportMessageExtension(true);
////        message.addElement(new TextElement());
////        message.setMessageType(V2TIMConversation.V2TIM_GROUP);
////        message.setGroupID(groupID);
////        message.setSeq(messageSequence);
////
////        MessageCenter.getInstance().getMessageExtensions(
////                message, new IMCallback<List<MessageExtension>>(v2TIMValueCallback) {
////                    @Override
////                    public void success(List<MessageExtension> data) {
////                        super.success(data);
////                    }
////
////                    @Override
////                    public void fail(int code, String errorMessage) {
////                        super.fail(code, errorMessage);
////                    }
////                });
//    }

    private void clearLocalHistoryMessage(Object param, final V2TIMValueCallback<Object> _callback_) {
        if (null == param || !(param instanceof String)) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is not string");
            return;
        }

        String json = (String) param;
        if (TextUtils.isEmpty(json)) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is empty");
            return;
        }

        String apiTitle = "internal_operation_clear_local_history_message";
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

    private void sendTRTCCustomData(Object param, V2TIMValueCallback<Object> _callback_) {
        if (null == param || !(param instanceof byte[])) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is not byte array");
            return;
        }

        String apiTitle = "internal_operation_send_trtc_custom_data";
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

//    private void getAIDenoiseSignature(final V2TIMValueCallback<Object> _callback_) {
//        String apiTitle = "";
//        nativeCallExperimentalAPI(apiTitle, null, new IMCallback<Object>(_callback_) {
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
//
////        BaseManager.getInstance().getAIDenoiseSignature(new IMCallback(callback) {
////            @Override
////            public void success(Object data) {
////                super.success(data);
////            }
////
////            @Override
////            public void fail(int code, String errorMessage) {
////                super.fail(code, errorMessage);
////            }
////        });
//    }

    private void writeLog(Object param, final V2TIMValueCallback<Object> _callback_) {
        if (null == param || !(param instanceof String)) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is not string");
            return;
        }

        String json = (String) param;
        if (TextUtils.isEmpty(json)) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is empty");
            return;
        }

        String apiTitle = "internal_operation_write_log";
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

    private void getMessageRevoker(Object param, final V2TIMValueCallback<Object> _callback_) {
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

        String apiTitle = "internal_operation_get_message_revoker";
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

    private void setOfflinePushState(Object param, V2TIMValueCallback<Object> _callback_) {
        if (null == param || !(param instanceof Integer)) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is not int");
            return;
        }

        String apiTitle = "internal_operation_set_offline_push_state";
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

    private void getOfflinePushState(V2TIMValueCallback<Object> valueCallback) {
        String apiTitle = "internal_operation_get_offline_push_state";
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

    private void setBuildInfo(Object param, V2TIMValueCallback<Object> _callback_) {
        if (null == param || !(param instanceof String)) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is not string");
            return;
        }

        String json = (String) param;
        if (TextUtils.isEmpty(json)) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is empty");
            return;
        }

        String apiTitle = "";
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

    private void setDatabaseEncryptInfo(Object param, V2TIMValueCallback<Object> _callback_) {
        if (null == param || !(param instanceof String)) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is not string");
            return;
        }

        String json = (String) param;
        if (TextUtils.isEmpty(json)) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is empty");
            return;
        }

        String apiTitle = "internal_operation_set_database_encrypt_info";
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

    private void isCommercialAbilityEnabled(Object param, V2TIMValueCallback<Object> _callback_) {
        if (null == param || !(param instanceof Long)) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is not int");
            return;
        }

        String apiTitle = "internal_operation_is_commercial_ability_enabled";
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

    private void setPacketRetryInfo(Object param, V2TIMValueCallback<Object> _callback_) {
        if (null == param || !(param instanceof String)) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is not string");
            return;
        }

        String json = (String) param;
        if (TextUtils.isEmpty(json)) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is empty");
            return;
        }

        String apiTitle = "internal_operation_set_max_retry_count";
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

    private void setCosSaveRegion(Object param, V2TIMValueCallback<Object> _callback_) {
        if (null == param || !(param instanceof String)) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is not string");
            return;
        }

        String json = (String) param;
        if (TextUtils.isEmpty(json)) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is empty");
            return;
        }

        String apiTitle = "internal_operation_set_cos_save_region_for_conversation";
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

    private void initLocalStorage(Object param, V2TIMValueCallback<Object> _callback_) {
        if (!(param instanceof String)) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is not string");
            return;
        }

        String userID = (String) param;
        if (TextUtils.isEmpty(userID)) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "invalid userID");
            return;
        }

        String apiTitle = "internal_operation_init_local_storage";
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

    private void setCustomServerInfo(Object param, V2TIMValueCallback<Object> _callback_) {
        if (null == param || !(param instanceof String)) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is not string");
            return;
        }

        String json = (String) param;
        if (TextUtils.isEmpty(json)) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is empty");
            return;
        }

        String apiTitle = "internal_operation_set_custom_server_info";
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

    private void setQuicChannelInfo(Object param, V2TIMValueCallback<Object> _callback_) {
        if (null == param || !(param instanceof String)) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is not string");
            return;
        }

        String json = (String) param;
        if (TextUtils.isEmpty(json)) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is empty");
            return;
        }

        String apiTitle = "internal_operation_set_quic_channel_info";
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

    private void setProxyInfo(Object param, V2TIMValueCallback<Object> _callback_) {
        if (null == param || !(param instanceof String)) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is not string");
            return;
        }

        String json = (String) param;
        if (TextUtils.isEmpty(json)) {
            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is empty");
            return;
        }

        String apiTitle = "internal_operation_update_proxy_info";
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

    private void setUIPlatform(Object param, V2TIMValueCallback<Object> _callback_) {
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

        String apiTitle = "internal_operation_set_ui_platform";
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

    private void setTestEnvironment(Object param, V2TIMValueCallback<Object> _callback_) {
        if (param != null && param instanceof Boolean) {
            String apiTitle = "internal_operation_set_env";
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

    private void setIPv6Prior(Object param, V2TIMValueCallback<Object> _callback_) {
        if (param != null && param instanceof Boolean) {
            String apiTitle = "internal_operation_set_ipv6_prior";
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

    private void setApplicationID(Object param, V2TIMValueCallback<Object> _callback_) {
        if (param != null && param instanceof Integer) {
//            BaseManager.getInstance().setApplicationID(((Integer) param).intValue());
            String apiTitle = "internal_operation_set_application_id";
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

//    private void reportOfflinePushEvent(Object param, final V2TIMValueCallback<Object> _callback_) {
//        if (null == param || !(param instanceof String)) {
//            callbackOnError(callback, BaseConstants.ERR_INVALID_PARAMETERS, "param is not string");
//            return;
//        }
//
//        String json = (String) param;
//        if (TextUtils.isEmpty(json)) {
//            callbackOnError(callback, BaseConstants.ERR_INVALID_PARAMETERS, "param is empty");
//            return;
//        }
//
//        String apiTitle = "internal_operation_get_offline_push_state";
//        nativeCallExperimentalAPI(apiTitle, null, new IMCallback<Object>(_callback_) {
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
//
////        List<OfflinePushEventItem> reportEventItemList = new ArrayList<>();
////        try {
////            JSONObject jsonObject = new JSONObject(json);
////            JSONArray reportEventList = jsonObject.optJSONArray("eventList");
////            if (reportEventList != null && reportEventList.length() > 0) {
////                for (int i = 0; i < reportEventList.length(); i++) {
////                    JSONObject eventItemJson = reportEventList.getJSONObject(i);
////                    OfflinePushEventItem eventItem = new OfflinePushEventItem();
////                    eventItem.setEventType(eventItemJson.optInt("type"));
////                    eventItem.setEventTime(eventItemJson.getLong("time"));
////                    eventItem.setPushId(eventItemJson.optString("pushId"));
////                    reportEventItemList.add(eventItem);
////                }
////            }
////
////            OfflinePushManager.getInstance().reportOfflinePushEvent(reportEventItemList, new IMCallback(callback) {
////                @Override
////                public void success(Object data) {
////                    super.success(data);
////                }
////
////                @Override
////                public void fail(int code, String errorMessage) {
////                    super.fail(code, errorMessage);
////                }
////            });
////        } catch (JSONException e) {
////            callbackOnError(callback, BaseConstants.ERR_INVALID_PARAMETERS, "convert param to json failed");
////            e.printStackTrace();
////        }
//    }

//    private void setOfflinePushInfo(Object param, final V2TIMValueCallback<Object> _callback_) {
//        if (null == param || !(param instanceof String)) {
//            callbackOnError(_callback_, BaseConstants.ERR_INVALID_PARAMETERS, "param is not string");
//            return;
//        }
//
//        String apiTitle = "internal_operation_set_offline_push_state";
//        nativeCallExperimentalAPI(apiTitle, null, new IMCallback<Object>(_callback_) {
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
//
////        String json = (String) param;
////        if (TextUtils.isEmpty(json)) {
////            callbackOnError(callback, BaseConstants.ERR_INVALID_PARAMETERS, "param is empty");
////            return;
////        }
////
////        try {
////            JSONObject jsonObject = new JSONObject(json);
////            int status = jsonObject.optInt("notification_bar_state");
////            String version = jsonObject.optString("tim_push_plugin_version");
////
////            // 开关状态：1，开启； 2，关闭； 0，未采集
////            if (status < 0 || status > 2) {
////                status = 0;
////            }
////
////            OfflinePushManager.getInstance().setOfflinePushInfo(version, status);
////            callbackOnSuccess(callback, null);
////        } catch (JSONException e) {
////            callbackOnError(callback, BaseConstants.ERR_INVALID_PARAMETERS, "convert param to json failed");
////            e.printStackTrace();
////        }
//    }

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
