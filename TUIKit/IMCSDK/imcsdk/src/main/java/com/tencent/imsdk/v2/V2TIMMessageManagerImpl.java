package com.tencent.imsdk.v2;

import com.tencent.imsdk.common.IMCallback;
import com.tencent.imsdk.common.IMLog;

import java.util.HashMap;
import java.util.List;

public class V2TIMMessageManagerImpl extends V2TIMMessageManager {
    private static final String TAG = "V2TIMMessageManagerImpl";

    private static final int MAX_FORWARD_COUNT = 300;
    private static final int MAX_ABSTRACT_COUNT = 5;
    private static final int MAX_ABSTRACT_LENGTH = 100;

    private static class V2TIMMessageManagerImplHolder {
        private static final V2TIMMessageManagerImpl v2TIMMessageManagerImpl = new V2TIMMessageManagerImpl();
    }

    static V2TIMMessageManagerImpl getInstance() {
        return V2TIMMessageManagerImplHolder.v2TIMMessageManagerImpl;
    }

    @Override
    public void addAdvancedMsgListener(V2TIMAdvancedMsgListener listener) {
        nativeAddAdvancedMsgListener(listener,listener.toString());
    }

    @Override
    public void removeAdvancedMsgListener(V2TIMAdvancedMsgListener listener) {
        nativeRemoveAdvancedMsgListener(listener.toString());
    }

    @Override
    public V2TIMMessage createTextMessage(String text) {
        V2TIMMessage timMessage = new V2TIMMessage();
        V2TIMTextElem textElem = new V2TIMTextElem();
        textElem.setText(text);
        timMessage.addElem(textElem);
        return timMessage;
    }

    @Override
    public V2TIMMessage createTextAtMessage(String text, List<String> atUserList) {
        V2TIMMessage timMessage = new V2TIMMessage();
        V2TIMTextElem textElem = new V2TIMTextElem();
        textElem.setText(text);
        timMessage.addElem(textElem);
        return timMessage;
    }

    @Override
    public V2TIMMessage createCustomMessage(byte[] data) {
        V2TIMMessage timMessage = new V2TIMMessage();
        V2TIMCustomElem customElem = new V2TIMCustomElem();
        customElem.setData(data);
        timMessage.addElem(customElem);
        return timMessage;
    }

    @Override
    public V2TIMMessage createCustomMessage(byte[] data, String description, byte[] extension) {
        V2TIMMessage timMessage = new V2TIMMessage();
        V2TIMCustomElem customElem = new V2TIMCustomElem();
        customElem.setData(data);
        customElem.setDescription(description);
        customElem.setExtension(extension);
        timMessage.addElem(customElem);
        return timMessage;
    }

    @Override
    public V2TIMMessage createImageMessage(String imagePath) {
        V2TIMMessage timMessage = new V2TIMMessage();
        V2TIMImageElem imageElem = new V2TIMImageElem();
        imageElem.setPath(imagePath);
        timMessage.addElem(imageElem);
        return timMessage;
    }

    @Override
    public V2TIMMessage createSoundMessage(String soundPath, int duration) {
        V2TIMMessage timMessage = new V2TIMMessage();
        V2TIMSoundElem soundElem = new V2TIMSoundElem();
        soundElem.setPath(soundPath);
        soundElem.setDuration(duration);
        timMessage.addElem(soundElem);
        return timMessage;
    }

    @Override
    public V2TIMMessage createVideoMessage(String videoFilePath, String type, int duration, String snapshotPath) {
        V2TIMMessage timMessage = new V2TIMMessage();
        V2TIMVideoElem videoElem = new V2TIMVideoElem();
        videoElem.setVideoPath(videoFilePath);
        videoElem.setVideoType(type);
        videoElem.setDuration(duration);
        videoElem.setSnapshotPath(snapshotPath);
        timMessage.addElem(videoElem);
        return timMessage;
    }

    @Override
    public V2TIMMessage createFileMessage(String filePath, String fileName) {
        V2TIMMessage timMessage = new V2TIMMessage();
        V2TIMFileElem fileElem = new V2TIMFileElem();
        fileElem.setPath(filePath);
        fileElem.setFileName(fileName);
        timMessage.addElem(fileElem);
        return timMessage;
    }

    @Override
    public V2TIMMessage createLocationMessage(String desc, double longitude, double latitude) {
        V2TIMMessage timMessage = new V2TIMMessage();
        V2TIMLocationElem locationElem = new V2TIMLocationElem();
        locationElem.setDesc(desc);
        locationElem.setLongitude(longitude);
        locationElem.setLatitude(latitude);
        timMessage.addElem(locationElem);
        return timMessage;
    }

    @Override
    public V2TIMMessage createFaceMessage(int index, byte[] data) {
        V2TIMMessage timMessage = new V2TIMMessage();
        V2TIMFaceElem faceElem = new V2TIMFaceElem();
        faceElem.setIndex(index);
        faceElem.setData(data);
        timMessage.addElem(faceElem);
        return timMessage;
    }

    @Override
    public V2TIMMessage createMergerMessage(List<V2TIMMessage> messageList, String title, List<String> abstractList, String compatibleText) {
        if (messageList == null || messageList.size() == 0 || messageList.size() > 10) {
            IMLog.e(TAG, "messageList invalid, the number of messageList must be between 1 and " + MAX_FORWARD_COUNT);
            return null;
        }

        for (V2TIMMessage v2TIMMessage : messageList) {
            if (V2TIMMessage.V2TIM_MSG_STATUS_SEND_SUCC != v2TIMMessage.getStatus()) {
                IMLog.e(TAG, "message status must be V2TIM_MSG_STATUS_SEND_SUCC");
                return null;
            }
            if (V2TIMMessage.V2TIM_ELEM_TYPE_GROUP_TIPS == v2TIMMessage.getElemType()) {
                IMLog.e(TAG, "group tips message is not support");
                return null;
            }
        }

        if (null == compatibleText) {
            IMLog.e(TAG, "compatibleText invalid, compatibleText cannot be null");
            return null;
        }

        V2TIMMessage timMessage = new V2TIMMessage();
        V2TIMMergerElem mergerElem = new V2TIMMergerElem();
        mergerElem.setMessageList(messageList);
        mergerElem.setTitle(title);
        mergerElem.setAbstractList(abstractList);
        mergerElem.setCompatibleText(compatibleText);
        timMessage.addElem(mergerElem);
        return timMessage;
    }

    @Override
    public V2TIMMessage createForwardMessage(V2TIMMessage message) {
        if (V2TIMMessage.V2TIM_MSG_STATUS_SEND_SUCC != message.getStatus()) {
            IMLog.e(TAG, "message status must be V2TIM_MSG_STATUS_SEND_SUCC");
            return null;
        }
        message.setForwardMessage(true);
        return message;
    }

    @Override
    public V2TIMMessage createTargetedGroupMessage(V2TIMMessage message, List<String> receiverList) {
        if (receiverList == null || receiverList.size() == 0) {
            IMLog.e(TAG, "receiverList invalid, receiverList must is not null and size > 0");
            return null;
        }
        if (message.getElemList().size() == 0) {
            IMLog.e(TAG, "elemList invalid, elemList must have data");
            return null;
        }
        V2TIMMessage newMessage = new V2TIMMessage();
        for (V2TIMElem v2TIMElem : message.getElemList()) {
            newMessage.addElem(v2TIMElem);
        }
        newMessage.setTargetGroupMemberList(receiverList);
        return message;
    }

    @Override
    public V2TIMMessage createAtSignedGroupMessage(V2TIMMessage message, List<String> atUserList) {
        if (atUserList == null || atUserList.size() == 0) {
            IMLog.e(TAG, "atUserList invalid, atUserList must is not null and size > 0");
            return null;
        }
        if (message.getElemList().size() == 0) {
            IMLog.e(TAG, "elemList invalid, elemList must have data");
            return null;
        }
        V2TIMMessage newMessage = new V2TIMMessage();
        for (V2TIMElem v2TIMElem : message.getElemList()) {
            newMessage.addElem(v2TIMElem);
        }
        newMessage.setGroupAtUserList(atUserList);
        return message;
    }

    @Override
    public String sendMessage(V2TIMMessage message, String receiver, String groupID, int priority, boolean onlineUserOnly, V2TIMOfflinePushInfo offlinePushInfo, V2TIMSendCallback<V2TIMMessage> _callback_) {
        if (message != null){
            message.setSelf(true);
            message.setRead(true);
        }
        return nativeSendMessage(message, receiver, groupID, priority, onlineUserOnly, offlinePushInfo, new IMCallback<V2TIMMessage>(_callback_) {
            @Override
            public void progress(int progress) {
                super.progress(progress);
            }

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
    public void setC2CReceiveMessageOpt(List<String> userIDList, int opt, V2TIMCallback _callback_) {
        nativeSetC2CReceiveMessageOpt(userIDList, opt, new IMCallback(_callback_) {
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
    public void getC2CReceiveMessageOpt(List<String> userIDList, V2TIMValueCallback<List<V2TIMReceiveMessageOptInfo>> _callback_) {
        nativeGetC2CReceiveMessageOpt(userIDList, new IMCallback<List<V2TIMReceiveMessageOptInfo>>(_callback_) {
            @Override
            public void success(List<V2TIMReceiveMessageOptInfo> data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });
    }

    @Override
    public void setGroupReceiveMessageOpt(String groupID, int opt, V2TIMCallback _callback_) {
        nativeSetGroupReceiveMessageOpt(groupID, opt, new IMCallback(_callback_) {
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
    public void setAllReceiveMessageOpt(int opt, int startHour, int startMinute, int startSecond, long duration, V2TIMCallback _callback_) {
        nativeSetAllReceiveMessageOpt(opt, startHour, startMinute, startSecond, duration, new IMCallback(_callback_) {
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
    public void setAllReceiveMessageOpt(int opt, long startTimeStamp, long duration, V2TIMCallback _callback_) {
        nativeSetAllReceiveMessageOpt(opt, startTimeStamp, duration, new IMCallback(_callback_) {
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
    public void getAllReceiveMessageOpt(V2TIMValueCallback<V2TIMReceiveMessageOptInfo> _callback_) {
        nativeGetAllReceiveMessageOpt(new IMCallback<V2TIMReceiveMessageOptInfo>(_callback_) {
            @Override
            public void success(V2TIMReceiveMessageOptInfo data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });
    }

    @Override
    public void getC2CHistoryMessageList(String userID, int count, V2TIMMessage lastMsg, V2TIMValueCallback<List<V2TIMMessage>> _callback_) {
        nativeGetC2CHistoryMessageList(userID, count, lastMsg, new IMCallback<List<V2TIMMessage>>(_callback_) {
            @Override
            public void success(List<V2TIMMessage> data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });
    }

    @Override
    public void getGroupHistoryMessageList(String groupID, int count, V2TIMMessage lastMsg, V2TIMValueCallback<List<V2TIMMessage>> _callback_) {
        nativeGetGroupHistoryMessageList(groupID, count, lastMsg, new IMCallback<List<V2TIMMessage>>(_callback_) {
            @Override
            public void success(List<V2TIMMessage> data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });
    }

    @Override
    public void getHistoryMessageList(V2TIMMessageListGetOption option, V2TIMValueCallback<List<V2TIMMessage>> _callback_) {
        nativeGetHistoryMessageList(option, new IMCallback<List<V2TIMMessage>>(_callback_) {
            @Override
            public void success(List<V2TIMMessage> data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });
    }

    @Override
    public void revokeMessage(V2TIMMessage msg, V2TIMCallback _callback_) {
        nativeRevokeMessage(msg, new IMCallback(_callback_) {
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
    public void modifyMessage(V2TIMMessage msg, V2TIMCompleteCallback<V2TIMMessage> _callback_) {
        nativeModifyMessage(msg, new IMCallback<V2TIMMessage>(_callback_) {
            @Override
            public void failOrComplete(int code, String errorMessage, V2TIMMessage data) {
                super.failOrComplete(code, errorMessage, data);
            }
        });
    }

    @Override
    public void deleteMessageFromLocalStorage(V2TIMMessage msg, V2TIMCallback _callback_) {
        nativeDeleteMessageFromLocalStorage(msg, new IMCallback(_callback_) {
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
    public void deleteMessages(List<V2TIMMessage> messages, V2TIMCallback _callback_) {
        nativeDeleteMessages(messages, new IMCallback(_callback_) {
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
    public void clearC2CHistoryMessage(String userID, V2TIMCallback _callback_) {
        nativeClearC2CHistoryMessage(userID, new IMCallback(_callback_) {
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
    public void clearGroupHistoryMessage(String groupID, V2TIMCallback _callback_) {
        nativeClearGroupHistoryMessage(groupID, new IMCallback(_callback_) {
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
    public String insertGroupMessageToLocalStorage(V2TIMMessage msg, String groupID, String sender, V2TIMValueCallback<V2TIMMessage> _callback_) {
        return nativeInsertGroupMessageToLocalStorage(msg, groupID, sender, new IMCallback<V2TIMMessage>(_callback_) {
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
    public String insertC2CMessageToLocalStorage(V2TIMMessage msg, String userID, String sender, V2TIMValueCallback<V2TIMMessage> _callback_) {
        return nativeInsertC2CMessageToLocalStorage(msg, userID, sender, new IMCallback<V2TIMMessage>(_callback_) {
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
    public void findMessages(List<String> messageIDList, V2TIMValueCallback<List<V2TIMMessage>> _callback_) {
        nativeFindMessages(messageIDList, new IMCallback<List<V2TIMMessage>>(_callback_) {
            @Override
            public void success(List<V2TIMMessage> data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });
    }

    @Override
    public void searchLocalMessages(V2TIMMessageSearchParam searchParam, V2TIMValueCallback<V2TIMMessageSearchResult> _callback_) {
        nativeSearchLocalMessages(searchParam, new IMCallback<V2TIMMessageSearchResult>(_callback_) {
            @Override
            public void success(V2TIMMessageSearchResult data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });
    }

    @Override
    public void searchCloudMessages(V2TIMMessageSearchParam searchParam, V2TIMValueCallback<V2TIMMessageSearchResult> _callback_) {
        nativeSearchCloudMessages(searchParam, new IMCallback<V2TIMMessageSearchResult>(_callback_) {
            @Override
            public void success(V2TIMMessageSearchResult data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });
    }

    @Override
    public void sendMessageReadReceipts(List<V2TIMMessage> messageList, V2TIMCallback _callback_) {
        nativeSendMessageReadReceipts(messageList, new IMCallback(_callback_) {
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
    public void getMessageReadReceipts(List<V2TIMMessage> messageList, V2TIMValueCallback<List<V2TIMMessageReceipt>> _callback_) {
        nativeGetMessageReadReceipts(messageList, new IMCallback<List<V2TIMMessageReceipt>>(_callback_) {
            @Override
            public void success(List<V2TIMMessageReceipt> data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });
    }

    @Override
    public void getGroupMessageReadMemberList(V2TIMMessage message, int filter, long nextSeq, int count, V2TIMValueCallback<V2TIMGroupMessageReadMemberList> _callback_) {
        nativeGetGroupMessageReadMemberList(message, filter, nextSeq, count, new IMCallback<V2TIMGroupMessageReadMemberList>(_callback_) {
            @Override
            public void success(V2TIMGroupMessageReadMemberList data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });
    }

    @Override
    public void setMessageExtensions(V2TIMMessage message, List<V2TIMMessageExtension> extensions, V2TIMValueCallback<List<V2TIMMessageExtensionResult>> _callback_) {
        nativeSetMessageExtensions(message, extensions, new IMCallback<List<V2TIMMessageExtensionResult>>(_callback_) {
            @Override
            public void success(List<V2TIMMessageExtensionResult> data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });
    }

    @Override
    public void getMessageExtensions(V2TIMMessage message, V2TIMValueCallback<List<V2TIMMessageExtension>> _callback_) {
        nativeGetMessageExtensions(message, new IMCallback<List<V2TIMMessageExtension>>(_callback_) {
            @Override
            public void success(List<V2TIMMessageExtension> data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });
    }

    @Override
    public void deleteMessageExtensions(V2TIMMessage message, List<String> keys, V2TIMValueCallback<List<V2TIMMessageExtensionResult>> _callback_) {
        nativeDeleteMessageExtensions(message, keys, new IMCallback<List<V2TIMMessageExtensionResult>>(_callback_) {
            @Override
            public void success(List<V2TIMMessageExtensionResult> data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });
    }

    @Override
    public void addMessageReaction(V2TIMMessage message, String reactionID, V2TIMCallback _callback_) {
        nativeAddMessageReaction(message, reactionID, new IMCallback(_callback_) {
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
    public void removeMessageReaction(V2TIMMessage message, String reactionID, V2TIMCallback _callback_) {
        nativeRemoveMessageReaction(message, reactionID, new IMCallback(_callback_) {
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
    public void getMessageReactions(List<V2TIMMessage> messageList, int maxUserCountPerReaction, V2TIMValueCallback<List<V2TIMMessageReactionResult>> _callback_) {
        nativeGetMessageReactions(messageList, maxUserCountPerReaction, new IMCallback<List<V2TIMMessageReactionResult>>(_callback_) {
            @Override
            public void success(List<V2TIMMessageReactionResult> data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });
    }

    @Override
    public void getAllUserListOfMessageReaction(V2TIMMessage message, String reactionID, int nextSeq, int count, V2TIMValueCallback<V2TIMMessageReactionUserResult> _callback_) {
        nativeGetAllUserListOfMessageReaction(message, reactionID, nextSeq, count, new IMCallback<V2TIMMessageReactionUserResult>(_callback_) {
            @Override
            public void success(V2TIMMessageReactionUserResult data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });
    }

    @Override
    public void translateText(List<String> sourceTextList, String sourceLanguage, String targetLanguage, V2TIMValueCallback<HashMap<String, String>> _callback_) {
        nativeTranslateText(sourceTextList, sourceLanguage, targetLanguage, new IMCallback<HashMap<String, String>>(_callback_) {
            @Override
            public void success(HashMap<String, String> data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });
    }

    @Override
    public void markC2CMessageAsRead(String userID, V2TIMCallback _callback_) {
        nativeMarkC2CMessageAsRead(userID, new IMCallback(_callback_) {
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
    public void markGroupMessageAsRead(String groupID, V2TIMCallback _callback_) {
        nativeMarkGroupMessageAsRead(groupID, new IMCallback(_callback_) {
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
    public void markAllMessageAsRead(V2TIMCallback _callback_) {
        nativeMarkAllMessageAsRead(new IMCallback(_callback_) {
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
