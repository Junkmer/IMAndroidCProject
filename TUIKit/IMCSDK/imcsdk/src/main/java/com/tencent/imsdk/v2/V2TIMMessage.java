package com.tencent.imsdk.v2;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

/**
 * api docs链接：https://im.sdk.qcloud.com/doc/zh-cn/classcom_1_1tencent_1_1imsdk_1_1v2_1_1V2TIMMessage.html
 */
public class V2TIMMessage implements Serializable {
    public static final int V2TIM_MSG_STATUS_SENDING = 1;
    public static final int V2TIM_MSG_STATUS_SEND_SUCC = 2;
    public static final int V2TIM_MSG_STATUS_SEND_FAIL = 3;
    public static final int V2TIM_MSG_STATUS_HAS_DELETED = 4;
    public static final int V2TIM_MSG_STATUS_LOCAL_IMPORTED = 5;
    public static final int V2TIM_MSG_STATUS_LOCAL_REVOKED = 6;
    public static final int V2TIM_PRIORITY_DEFAULT = 0;
    public static final int V2TIM_PRIORITY_HIGH = 1;
    public static final int V2TIM_PRIORITY_NORMAL = 2;
    public static final int V2TIM_PRIORITY_LOW = 3;
    public static final int V2TIM_ELEM_TYPE_NONE = 0;
    public static final int V2TIM_ELEM_TYPE_TEXT = 1;
    public static final int V2TIM_ELEM_TYPE_CUSTOM = 2;
    public static final int V2TIM_ELEM_TYPE_IMAGE = 3;
    public static final int V2TIM_ELEM_TYPE_SOUND = 4;
    public static final int V2TIM_ELEM_TYPE_VIDEO = 5;
    public static final int V2TIM_ELEM_TYPE_FILE = 6;
    public static final int V2TIM_ELEM_TYPE_LOCATION = 7;
    public static final int V2TIM_ELEM_TYPE_FACE = 8;
    public static final int V2TIM_ELEM_TYPE_GROUP_TIPS = 9;
    public static final int V2TIM_ELEM_TYPE_MERGER = 10;
    public static final int V2TIM_RECEIVE_MESSAGE = 0;
    public static final int V2TIM_NOT_RECEIVE_MESSAGE = 1;
    public static final int V2TIM_RECEIVE_NOT_NOTIFY_MESSAGE = 2;
    public static final int V2TIM_GROUP_MESSAGE_READ_MEMBERS_FILTER_READ = 0;
    public static final int V2TIM_GROUP_MESSAGE_READ_MEMBERS_FILTER_UNREAD = 1;

    private String msgID;
    private String sender;
    private String nickName;
    private String friendRemark;
    private String faceUrl;
    private String nameCard;
    private String groupID;
    private String userID;
    private int status;

    private long clientTime;
    private long serverTime;
    private long senderTinyId ;
    private long receiverTinyId;

    private List<V2TIMElem> elemList = new ArrayList<>();

    private int localCustomInt;
    private String localCustomData;
    private String cloudCustomData;

    private boolean isSelf;
    private boolean isRead;
    private boolean isPeerRead;

   private boolean needReadReceipt;

    private boolean isBroadcastMessage;
    private int priority;
    private V2TIMOfflinePushInfo offlinePushInfo;
    private List<String> groupAtUserList;
    private long seq;
    private long random;
    private boolean isOnlineMsg;

    private boolean isExcludedFromUnreadCount;
    private boolean isExcludedFromLastMessage;
    private boolean isSupportMessageExtension;
    private boolean riskContent;
    private V2TIMUserFullInfo revokerInfo;
    private String 	revokeReason;
    private boolean isForwardMessage;//是否是转发消息，仅发送时需要
    private List<String> targetGroupMemberList;//发送定向群消息时设置

    public String getMsgID() {
        return msgID;
    }

    public long getTimestamp() {
        if (serverTime > 0){
            return serverTime;
        }
        return clientTime;
    }

    public String getSender() {
        return sender;
    }

    public String getNickName() {
        return nickName;
    }

    public String getFriendRemark() {
        return friendRemark;
    }

    public String getFaceUrl() {
        return faceUrl;
    }

    public String getNameCard() {
        return nameCard;
    }

    public String getGroupID() {
        return groupID;
    }

    public String getUserID() {
        return userID;
    }

    public int getStatus() {
        return status;
    }

    public int getElemType() {
        int elemType = 0;
        if (this.elemList == null) {
            return elemType;
        } else if (this.elemList.size() == 0) {
            return elemType;
        } else {
            V2TIMElem v2TIMElem = elemList.get(0);
            if (v2TIMElem instanceof V2TIMTextElem) {
                elemType = V2TIM_ELEM_TYPE_TEXT;
            } else if (v2TIMElem instanceof V2TIMImageElem) {
                elemType = V2TIM_ELEM_TYPE_IMAGE;
            } else if (v2TIMElem instanceof V2TIMVideoElem) {
                elemType = V2TIM_ELEM_TYPE_VIDEO;
            } else if (v2TIMElem instanceof V2TIMSoundElem) {
                elemType = V2TIM_ELEM_TYPE_SOUND;
            } else if (v2TIMElem instanceof V2TIMCustomElem) {
                elemType = V2TIM_ELEM_TYPE_CUSTOM;
            } else if (v2TIMElem instanceof V2TIMGroupTipsElem) {
                elemType = V2TIM_ELEM_TYPE_GROUP_TIPS;
            }  else if (v2TIMElem instanceof V2TIMFaceElem) {
                elemType = V2TIM_ELEM_TYPE_FACE;
            } else if (v2TIMElem instanceof V2TIMFileElem) {
                elemType = V2TIM_ELEM_TYPE_FILE;
            } else if (v2TIMElem instanceof V2TIMLocationElem) {
                elemType = V2TIM_ELEM_TYPE_LOCATION;
            }else if (v2TIMElem instanceof V2TIMMergerElem) {
                elemType = V2TIM_ELEM_TYPE_MERGER;
            }
            return elemType;
        }
    }

    public V2TIMTextElem getTextElem() {
        if (getElemType() == V2TIM_ELEM_TYPE_NONE){
            return null;
        }
        if (getElemType() == V2TIM_ELEM_TYPE_TEXT){
            V2TIMTextElem timTextElem = (V2TIMTextElem) elemList.get(0);
            timTextElem.setElemList(elemList);
            timTextElem.setElemIndex(0);
            return timTextElem;
        }
        return null;
    }

    public V2TIMCustomElem getCustomElem() {
        if (getElemType() == V2TIM_ELEM_TYPE_NONE){
            return null;
        }
        if (getElemType() == V2TIM_ELEM_TYPE_CUSTOM){
            V2TIMCustomElem customElem = (V2TIMCustomElem) elemList.get(0);
            customElem.setElemList(elemList);
            customElem.setElemIndex(0);
            return customElem;
        }
        return null;
    }

    public V2TIMImageElem getImageElem() {
        if (getElemType() == V2TIM_ELEM_TYPE_NONE){
            return null;
        }
        if (getElemType() == V2TIM_ELEM_TYPE_IMAGE){
            V2TIMImageElem imageElem = (V2TIMImageElem) elemList.get(0);
            imageElem.setElemList(elemList);
            imageElem.setElemIndex(0);
            return imageElem;
        }
        return null;
    }

    public V2TIMSoundElem getSoundElem() {
        if (getElemType() == V2TIM_ELEM_TYPE_NONE){
            return null;
        }
        if (getElemType() == V2TIM_ELEM_TYPE_SOUND){
            V2TIMSoundElem soundElem = (V2TIMSoundElem) elemList.get(0);
            soundElem.setElemList(elemList);
            soundElem.setElemIndex(0);
            return soundElem;
        }
        return null;
    }

    public V2TIMVideoElem getVideoElem() {
        if (getElemType() == V2TIM_ELEM_TYPE_NONE){
            return null;
        }
        if (getElemType() == V2TIM_ELEM_TYPE_VIDEO){
            V2TIMVideoElem videoElem = (V2TIMVideoElem) elemList.get(0);
            videoElem.setElemList(elemList);
            videoElem.setElemIndex(0);
            return videoElem;
        }
        return null;
    }

    public V2TIMFileElem getFileElem() {
        if (getElemType() == V2TIM_ELEM_TYPE_NONE){
            return null;
        }
        if (getElemType() == V2TIM_ELEM_TYPE_FILE){
            V2TIMFileElem fileElem = (V2TIMFileElem) elemList.get(0);
            fileElem.setElemList(elemList);
            fileElem.setElemIndex(0);
            return fileElem;
        }
        return null;
    }

    public V2TIMLocationElem getLocationElem() {
        if (getElemType() == V2TIM_ELEM_TYPE_NONE){
            return null;
        }
        if (getElemType() == V2TIM_ELEM_TYPE_LOCATION){
            V2TIMLocationElem locationElem = (V2TIMLocationElem) elemList.get(0);
            locationElem.setElemList(elemList);
            locationElem.setElemIndex(0);
            return locationElem;
        }
        return null;
    }

    public V2TIMFaceElem getFaceElem() {
        if (getElemType() == V2TIM_ELEM_TYPE_NONE){
            return null;
        }
        if (getElemType() == V2TIM_ELEM_TYPE_FACE){
            V2TIMFaceElem faceElem = (V2TIMFaceElem) elemList.get(0);
            faceElem.setElemList(elemList);
            faceElem.setElemIndex(0);
            return faceElem;
        }
        return null;
    }

    public V2TIMMergerElem getMergerElem() {
        if (getElemType() == V2TIM_ELEM_TYPE_NONE){
            return null;
        }
        if (getElemType() == V2TIM_ELEM_TYPE_MERGER){
            V2TIMMergerElem mergerElem = (V2TIMMergerElem) elemList.get(0);
            mergerElem.setElemList(elemList);
            mergerElem.setElemIndex(0);
            return mergerElem;
        }
        return null;
    }

    public V2TIMGroupTipsElem getGroupTipsElem() {
        if (getElemType() == V2TIM_ELEM_TYPE_NONE){
            return null;
        }
        if (getElemType() == V2TIM_ELEM_TYPE_GROUP_TIPS){
            V2TIMGroupTipsElem groupTipsElem = (V2TIMGroupTipsElem) elemList.get(0);
            groupTipsElem.setElemList(elemList);
            groupTipsElem.setElemIndex(0);
            return groupTipsElem;
        }
        return null;
    }

    public int getLocalCustomInt() {
        return localCustomInt;
    }

    public void setLocalCustomInt(int localCustomInt) {
        this.localCustomInt = localCustomInt;
        V2TIMManager.getMessageManager().nativeSetMsgLocalCustomData(this);
    }

    public String getLocalCustomData() {
        return localCustomData;
    }

    public void setLocalCustomData(String localCustomData) {
        this.localCustomData = localCustomData;
        //更新本地消息缓存
        V2TIMManager.getMessageManager().nativeSetMsgLocalCustomData(this);
    }

    public String getCloudCustomData() {
        return cloudCustomData;
    }

    public void setCloudCustomData(String cloudCustomData) {
        this.cloudCustomData = cloudCustomData;
    }

    protected void setSelf(boolean self) {
        isSelf = self;
    }

    public boolean isSelf() {
        return isSelf;
    }

    protected void setRead(boolean read) {
        isRead = read;
    }

    public boolean isRead() {
        return isRead;
    }

    public boolean isPeerRead() {
        return isPeerRead;
    }

    public boolean isNeedReadReceipt() {
        return needReadReceipt;
    }

    public void setNeedReadReceipt(boolean needReadReceipt) {
        this.needReadReceipt = needReadReceipt;
    }

    public boolean isBroadcastMessage() {
        return isBroadcastMessage;
    }

    public int getPriority() {
        return priority;
    }

    public V2TIMOfflinePushInfo getOfflinePushInfo() {
        return offlinePushInfo;
    }

    public List<String> getGroupAtUserList() {
        return groupAtUserList;
    }

    public long getSeq() {
        return seq;
    }

    public long getRandom() {
        return random;
    }

    public boolean isExcludedFromUnreadCount() {
        return isExcludedFromUnreadCount;
    }

    public void setExcludedFromUnreadCount(boolean excludedFromUnreadCount) {
        this.isExcludedFromUnreadCount = excludedFromUnreadCount;
    }

    public boolean isExcludedFromLastMessage() {
        return isExcludedFromLastMessage;
    }

    public void setExcludedFromLastMessage(boolean excludedFromLastMessage) {
        this.isExcludedFromLastMessage = excludedFromLastMessage;
    }

    public boolean isSupportMessageExtension() {
        return isSupportMessageExtension;
    }

    public void setSupportMessageExtension(boolean supportMessageExtension) {
        isSupportMessageExtension = supportMessageExtension;
    }

    public boolean hasRiskContent() {
        return riskContent;
    }

    public V2TIMUserFullInfo getRevokerInfo() {
        return revokerInfo;
    }

    public String getRevokeReason() {
        return revokeReason;
    }

    protected void setForwardMessage(boolean forwardMessage) {
        isForwardMessage = forwardMessage;
    }

    protected void setTargetGroupMemberList(List<String> targetGroupMemberList) {
        this.targetGroupMemberList = targetGroupMemberList;
    }

    protected void setGroupAtUserList(List<String> groupAtUserList) {
        this.groupAtUserList = groupAtUserList;
    }

    protected void addElem(V2TIMElem elem){
        if (null == elem){
            return;
        }
        elemList.add(elem);
    }

    protected List<V2TIMElem> getElemList() {
        return elemList;
    }

//    @Override
//    public String toString() {
//        StringBuilder stringBuilder = new StringBuilder();
//        stringBuilder.append("V2TIMMessage--->");
//
//        V2TIMElem v2TIMElem = null;
//        StringBuilder elemDescStringBuilder = new StringBuilder();
//        for (int i = 0; i < elemList.size(); i++) {
//            V2TIMElem baseElement = elemList.get(i);
//            if (baseElement instanceof V2TIMTextElem) {
//                if (v2TIMElem == null) {
//                    v2TIMElem = getTextElem();
//                } else {
//                    v2TIMElem = v2TIMElem.getNextElem();
//                }
//                V2TIMTextElem textElement = (V2TIMTextElem) v2TIMElem;
//                elemDescStringBuilder.append(textElement.toString());
//            } else if (baseElement instanceof V2TIMCustomElem) {
//                if (v2TIMElem == null) {
//                    v2TIMElem = getCustomElem();
//                } else {
//                    v2TIMElem = v2TIMElem.getNextElem();
//                }
//                V2TIMCustomElem customElement = (V2TIMCustomElem) v2TIMElem;
//                elemDescStringBuilder.append(customElement.toString());
//            } else if (baseElement instanceof V2TIMImageElem) {
//                if (v2TIMElem == null) {
//                    v2TIMElem = getImageElem();
//                } else {
//                    v2TIMElem = v2TIMElem.getNextElem();
//                }
//                V2TIMImageElem imageElement = (V2TIMImageElem) v2TIMElem;
//                elemDescStringBuilder.append(imageElement.toString());
//            } else if (baseElement instanceof V2TIMSoundElem) {
//                if (v2TIMElem == null) {
//                    v2TIMElem = getSoundElem();
//                } else {
//                    v2TIMElem = v2TIMElem.getNextElem();
//                }
//                V2TIMSoundElem soundElement = (V2TIMSoundElem) v2TIMElem;
//                elemDescStringBuilder.append(soundElement.toString());
//            } else if (baseElement instanceof V2TIMVideoElem) {
//                if (v2TIMElem == null) {
//                    v2TIMElem = getVideoElem();
//                } else {
//                    v2TIMElem = v2TIMElem.getNextElem();
//                }
//                V2TIMVideoElem videoElement = (V2TIMVideoElem) v2TIMElem;
//                elemDescStringBuilder.append(videoElement.toString());
//            } else if (baseElement instanceof V2TIMFileElem) {
//                if (v2TIMElem == null) {
//                    v2TIMElem = getFileElem();
//                } else {
//                    v2TIMElem = v2TIMElem.getNextElem();
//                }
//                V2TIMFileElem fileElem = (V2TIMFileElem) v2TIMElem;
//                elemDescStringBuilder.append(fileElem.toString());
//            } else if (baseElement instanceof V2TIMLocationElem) {
//                if (v2TIMElem == null) {
//                    v2TIMElem = getLocationElem();
//                } else {
//                    v2TIMElem = v2TIMElem.getNextElem();
//                }
//                V2TIMLocationElem locationElement = (V2TIMLocationElem) v2TIMElem;
//                elemDescStringBuilder.append(locationElement.toString());
//            } else if (baseElement instanceof V2TIMFaceElem) {
//                if (v2TIMElem == null) {
//                    v2TIMElem = getFaceElem();
//                } else {
//                    v2TIMElem = v2TIMElem.getNextElem();
//                }
//                V2TIMFaceElem faceElement = (V2TIMFaceElem) v2TIMElem;
//                elemDescStringBuilder.append(faceElement.toString());
//            } else if (baseElement instanceof V2TIMMergerElem) {
//                if (v2TIMElem == null) {
//                    v2TIMElem = getMergerElem();
//                } else {
//                    v2TIMElem = v2TIMElem.getNextElem();
//                }
//                V2TIMMergerElem mergerElem = (V2TIMMergerElem) v2TIMElem;
//                elemDescStringBuilder.append(mergerElem.toString());
//            } else if (baseElement instanceof V2TIMGroupTipsElem) {
//                if (v2TIMElem == null) {
//                    v2TIMElem = getGroupTipsElem();
//                } else {
//                    v2TIMElem = v2TIMElem.getNextElem();
//                }
//                V2TIMGroupTipsElem groupTipsElement = (V2TIMGroupTipsElem) v2TIMElem;
//                elemDescStringBuilder.append(groupTipsElement.toString());
//            }
//            elemDescStringBuilder.append("|");
//        }
//
//        stringBuilder.append("msgID:").append(getMsgID()).append(", timestamp:").append(getTimestamp()).
//                append(", sender:").append(getSender()).append(", nickname:").append(getNickName()).
//                append(", faceUrl:").append(getFaceUrl()).append(", friendRemark:").append(getFriendRemark()).
//                append(", friendRemark:").append(getFriendRemark()).append(", nameCard:").append(getNameCard()).
//                append(", groupID:").append(getGroupID()).append(", userID:").append(getUserID()).
//                append(", seq:").append(getSeq()).append(", random:").append(getRandom()).
//                append(", status:").append(getStatus()).append(", isSelf:").append(isSelf()).
//                append(", isRead:").append(isRead()).append(", isPeerRead:").append(isPeerRead()).
//                append(", needReadReceipt:").append(isNeedReadReceipt()).append(", priority:").append(getPriority()).
//                append(", groupAtUserList:").append(getGroupAtUserList()).append(", elemType:").append(getElemType()).
//                append(", localCustomData:").append(getLocalCustomData()).append(", localCustomInt:").append(getLocalCustomInt()).
//                append(", cloudCustomData:").append(getCloudCustomData()).append(", isExcludeFromUnreadCount:").append(isExcludedFromUnreadCount()).
//                append(", isExcludeFromLastMessage:").append(isExcludedFromLastMessage()).append(", offlinePushInfo:").append(getOfflinePushInfo()).
//                append(", isBroadcastMessage:").append(isBroadcastMessage()).
//                append(", elemDesc:").append(elemDescStringBuilder);
//        return stringBuilder.toString();
//    }


    @Override
    public String toString() {
        return "V2TIMMessage{" +
                "msgID='" + msgID + '\'' +
                ", timestamp=" + getTimestamp() +
                ", sender='" + sender + '\'' +
                ", nickName='" + nickName + '\'' +
                ", friendRemark='" + friendRemark + '\'' +
                ", faceUrl='" + faceUrl + '\'' +
                ", nameCard='" + nameCard + '\'' +
                ", groupID='" + groupID + '\'' +
                ", userID='" + userID + '\'' +
                ", status=" + status +
                ", elemList=" + elemList +
                ", localCustomInt=" + localCustomInt +
                ", localCustomData='" + localCustomData + '\'' +
                ", cloudCustomData='" + cloudCustomData + '\'' +
                ", isSelf=" + isSelf +
                ", isRead=" + isRead +
                ", isPeerRead=" + isPeerRead +
                ", needReadReceipt=" + needReadReceipt +
                ", isBroadcastMessage=" + isBroadcastMessage +
                ", priority=" + priority +
                ", offlinePushInfo=" + offlinePushInfo +
                ", groupAtUserList=" + groupAtUserList +
                ", seq=" + seq +
                ", random=" + random +
                ", isOnlineMsg=" + isOnlineMsg +
                ", isExcludedFromUnreadCount=" + isExcludedFromUnreadCount +
                ", isExcludedFromLastMessage=" + isExcludedFromLastMessage +
                ", isSupportMessageExtension=" + isSupportMessageExtension +
                ", isForwardMessage=" + isForwardMessage +
                ", targetGroupMemberList=" + targetGroupMemberList +
                '}';
    }
}
