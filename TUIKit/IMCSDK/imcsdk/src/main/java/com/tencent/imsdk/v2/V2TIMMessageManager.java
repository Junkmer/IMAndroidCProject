package com.tencent.imsdk.v2;

import com.tencent.imsdk.common.IMCallback;

import java.util.HashMap;
import java.util.List;

public abstract class V2TIMMessageManager {

/////////////////////////////////////////////////////////////////////////////////
//                               java层-抽象函数
/////////////////////////////////////////////////////////////////////////////////

    public abstract void addAdvancedMsgListener(V2TIMAdvancedMsgListener listener);

    public abstract void removeAdvancedMsgListener(V2TIMAdvancedMsgListener listener);

    public abstract V2TIMMessage createTextMessage(String text);

    public abstract V2TIMMessage createTextAtMessage(String text, List<String> atUserList);

    public abstract V2TIMMessage createCustomMessage(byte[] data);

    public abstract V2TIMMessage createCustomMessage(byte[] data, String description, byte[] extension);

    public abstract V2TIMMessage createImageMessage(String imagePath);

    public abstract V2TIMMessage createSoundMessage(String soundPath, int duration);

    public abstract V2TIMMessage createVideoMessage(String videoFilePath, String type, int duration, String snapshotPath);

    public abstract V2TIMMessage createFileMessage(String filePath, String fileName);

    public abstract V2TIMMessage createLocationMessage(String desc, double longitude, double latitude);

    public abstract V2TIMMessage createFaceMessage(int index, byte[] data);

    public abstract V2TIMMessage createMergerMessage(List<V2TIMMessage> messageList, String title, List<String> abstractList, String compatibleText);

    public abstract V2TIMMessage createForwardMessage(V2TIMMessage message);

    public abstract V2TIMMessage createTargetedGroupMessage(V2TIMMessage message, List<String> receiverList);

    public abstract V2TIMMessage createAtSignedGroupMessage(V2TIMMessage message, List<String> atUserList);

    public abstract String sendMessage(V2TIMMessage message, String receiver, String groupID, int priority, boolean onlineUserOnly, V2TIMOfflinePushInfo offlinePushInfo, V2TIMSendCallback<V2TIMMessage> callback);

    public abstract void setC2CReceiveMessageOpt(List<String> userIDList, int opt, V2TIMCallback callback);

    public abstract void getC2CReceiveMessageOpt(List<String> userIDList, V2TIMValueCallback<List<V2TIMReceiveMessageOptInfo>> callback);

    public abstract void setGroupReceiveMessageOpt(String groupID, int opt, V2TIMCallback callback);

    public abstract void setAllReceiveMessageOpt(int opt, int startHour, int startMinute, int startSecond, long duration, V2TIMCallback callback);

    public abstract void setAllReceiveMessageOpt(int opt, long startTimeStamp, long duration, V2TIMCallback callback);

    public abstract void getAllReceiveMessageOpt(V2TIMValueCallback<V2TIMReceiveMessageOptInfo> callback);

    public abstract void getC2CHistoryMessageList(String userID, int count, V2TIMMessage lastMsg, V2TIMValueCallback<List<V2TIMMessage>> callback);

    public abstract void getGroupHistoryMessageList(String groupID, int count, V2TIMMessage lastMsg, V2TIMValueCallback<List<V2TIMMessage>> callback);

    public abstract void getHistoryMessageList(V2TIMMessageListGetOption option, V2TIMValueCallback<List<V2TIMMessage>> callback);

    public abstract void revokeMessage(V2TIMMessage msg, V2TIMCallback callback);

    public abstract void modifyMessage(V2TIMMessage msg, V2TIMCompleteCallback<V2TIMMessage> callback);

    public abstract void deleteMessageFromLocalStorage(V2TIMMessage msg, V2TIMCallback callback);

    public abstract void deleteMessages(List<V2TIMMessage> messages, V2TIMCallback callback);

    public abstract void clearC2CHistoryMessage(String userID, V2TIMCallback callback);

    public abstract void clearGroupHistoryMessage(String groupID, V2TIMCallback callback);

    public abstract String insertGroupMessageToLocalStorage(V2TIMMessage msg, String groupID, String sender, V2TIMValueCallback<V2TIMMessage> callback);

    public abstract String insertC2CMessageToLocalStorage(V2TIMMessage msg, String userID, String sender, V2TIMValueCallback<V2TIMMessage> callback);

    public abstract void findMessages(List<String> messageIDList, V2TIMValueCallback<List<V2TIMMessage>> callback);

    public abstract void searchLocalMessages(V2TIMMessageSearchParam searchParam, V2TIMValueCallback<V2TIMMessageSearchResult> callback);

    public abstract void searchCloudMessages(V2TIMMessageSearchParam searchParam, V2TIMValueCallback<V2TIMMessageSearchResult> callback);

    public abstract void sendMessageReadReceipts(List<V2TIMMessage> messageList, V2TIMCallback callback);

    public abstract void getMessageReadReceipts(List<V2TIMMessage> messageList, V2TIMValueCallback<List<V2TIMMessageReceipt>> callback);

    public abstract void getGroupMessageReadMemberList(V2TIMMessage message, int filter, long nextSeq, int count, V2TIMValueCallback<V2TIMGroupMessageReadMemberList> callback);

    public abstract void setMessageExtensions(V2TIMMessage message, List<V2TIMMessageExtension> extensions, V2TIMValueCallback<List<V2TIMMessageExtensionResult>> callback);

    public abstract void getMessageExtensions(V2TIMMessage message, V2TIMValueCallback<List<V2TIMMessageExtension>> callback);

    public abstract void deleteMessageExtensions(V2TIMMessage message, List<String> keys, V2TIMValueCallback<List<V2TIMMessageExtensionResult>> callback);

    public abstract void addMessageReaction(V2TIMMessage message, String reactionID, V2TIMCallback callback);

    public abstract void removeMessageReaction(V2TIMMessage message, String reactionID, V2TIMCallback callback);

    public abstract void getMessageReactions(List<V2TIMMessage> messageList, int maxUserCountPerReaction, V2TIMValueCallback<List<V2TIMMessageReactionResult>> callback);

    public abstract void getAllUserListOfMessageReaction(V2TIMMessage message, String reactionID, int nextSeq, int count, V2TIMValueCallback<V2TIMMessageReactionUserResult> callback);

    public abstract void translateText(List<String> sourceTextList, String sourceLanguage, String targetLanguage, V2TIMValueCallback<HashMap<String, String>> callback);

    public abstract void markC2CMessageAsRead(String userID, V2TIMCallback callback);

    public abstract void markGroupMessageAsRead(String groupID, V2TIMCallback callback);

    public abstract void markAllMessageAsRead(V2TIMCallback callback);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------  native层-本地接口
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    native void nativeAddAdvancedMsgListener(V2TIMAdvancedMsgListener listener, String listenerPath);

    native void nativeRemoveAdvancedMsgListener(String listenerPath);

    native String nativeSendMessage(V2TIMMessage message, String receiver, String groupID, int priority, boolean onlineUserOnly, V2TIMOfflinePushInfo offlinePushInfo, IMCallback<V2TIMMessage> callback);

    native void nativeSetMsgLocalCustomData(V2TIMMessage message);

    native void nativeSetC2CReceiveMessageOpt(List<String> userIDList, int opt, IMCallback callback);

    native void nativeGetC2CReceiveMessageOpt(List<String> userIDList, IMCallback<List<V2TIMReceiveMessageOptInfo>> callback);

    native void nativeSetGroupReceiveMessageOpt(String groupID, int opt, IMCallback callback);

    native void nativeGetC2CHistoryMessageList(String userID, int count, V2TIMMessage lastMsg, IMCallback<List<V2TIMMessage>> callback);

    native void nativeGetGroupHistoryMessageList(String groupID, int count, V2TIMMessage lastMsg, IMCallback<List<V2TIMMessage>> callback);

    native void nativeGetHistoryMessageList(V2TIMMessageListGetOption option, IMCallback<List<V2TIMMessage>> callback);

    native void nativeRevokeMessage(V2TIMMessage msg, IMCallback callback);

    native void nativeModifyMessage(V2TIMMessage msg, IMCallback<V2TIMMessage> callback);

    native void nativeDeleteMessageFromLocalStorage(V2TIMMessage msg, IMCallback callback);

    native void nativeDeleteMessages(List<V2TIMMessage> messages, IMCallback callback);

    native void nativeClearC2CHistoryMessage(String userID, IMCallback callback);

    native void nativeClearGroupHistoryMessage(String groupID, IMCallback callback);

    native String nativeInsertGroupMessageToLocalStorage(V2TIMMessage msg, String groupID, String sender, IMCallback<V2TIMMessage> callback);

    native String nativeInsertC2CMessageToLocalStorage(V2TIMMessage msg, String userID, String sender, IMCallback<V2TIMMessage> callback);

    native void nativeFindMessages(List<String> messageIDList, IMCallback<List<V2TIMMessage>> callback);

    native void nativeSearchLocalMessages(V2TIMMessageSearchParam searchParam, IMCallback<V2TIMMessageSearchResult> callback);

    native void nativeSendMessageReadReceipts(List<V2TIMMessage> messageList, IMCallback callback);

    native void nativeGetMessageReadReceipts(List<V2TIMMessage> messageList, IMCallback<List<V2TIMMessageReceipt>> callback);

    native void nativeGetGroupMessageReadMemberList(V2TIMMessage message, int filter, long nextSeq, int count, IMCallback<V2TIMGroupMessageReadMemberList> callback);

    native void nativeSetAllReceiveMessageOpt(int opt, int startHour, int startMinute, int startSecond, long duration, IMCallback callback);

    native void nativeSetAllReceiveMessageOpt(int opt, long startTimeStamp, long duration, IMCallback callback);

    native void nativeGetAllReceiveMessageOpt(IMCallback<V2TIMReceiveMessageOptInfo> callback);

    native void nativeSearchCloudMessages(V2TIMMessageSearchParam searchParam, IMCallback<V2TIMMessageSearchResult> callback);

    native void nativeSetMessageExtensions(V2TIMMessage message, List<V2TIMMessageExtension> extensions, IMCallback<List<V2TIMMessageExtensionResult>> callback);

    native void nativeGetMessageExtensions(V2TIMMessage message, IMCallback<List<V2TIMMessageExtension>> callback);

    native void nativeDeleteMessageExtensions(V2TIMMessage message, List<String> keys, IMCallback<List<V2TIMMessageExtensionResult>> callback);

    native void nativeAddMessageReaction(V2TIMMessage message, String reactionID, IMCallback callback);

    native void nativeRemoveMessageReaction(V2TIMMessage message, String reactionID, IMCallback callback);

    native void nativeGetMessageReactions(List<V2TIMMessage> messageList, int maxUserCountPerReaction, IMCallback<List<V2TIMMessageReactionResult>> callback);

    native void nativeGetAllUserListOfMessageReaction(V2TIMMessage message, String reactionID, int nextSeq, int count, IMCallback<V2TIMMessageReactionUserResult> callback);

    native void nativeTranslateText(List<String> sourceTextList, String sourceLanguage, String targetLanguage, IMCallback<HashMap<String, String>> callback);

    native void nativeMarkC2CMessageAsRead(String userID, IMCallback callback);

    native void nativeMarkGroupMessageAsRead(String groupID, IMCallback callback);

    native void nativeMarkAllMessageAsRead(IMCallback callback);
}
