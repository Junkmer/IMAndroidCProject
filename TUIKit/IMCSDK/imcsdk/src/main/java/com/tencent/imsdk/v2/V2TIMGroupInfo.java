package com.tencent.imsdk.v2;

import java.io.Serializable;
import java.util.Map;

/**
 * api docs链接：https://im.sdk.qcloud.com/doc/zh-cn/classcom_1_1tencent_1_1imsdk_1_1v2_1_1V2TIMGroupInfo.html
 */
public class V2TIMGroupInfo implements Serializable {
    public static final int V2TIM_GROUP_ADD_FORBID = 0;
    public static final int V2TIM_GROUP_ADD_AUTH = 1;
    public static final int V2TIM_GROUP_ADD_ANY = 2;

    private String groupID;
    private String groupType;
    private boolean supportTopic;
    private String groupName;
    private String notification;
    private String introduction;
    private String faceUrl;
    private boolean isAllMuted;
    private String owner;
    private long createTime;
    private int groupAddOpt = -1;
    private int groupApproveOpt = -1;
    private Map<String, byte[]> customInfo;
    private long lastInfoTime;
    private long lastMessageTime;
    private int memberCount;
    private int onlineCount;
    private long memberMaxCount = -1;
    private int role;
    private int recvOpt;
    private long joinTime;

    public String getGroupID() {
        return groupID;
    }

    public void setGroupID(String groupID) {
        this.groupID = groupID;
    }

    public String getGroupType() {
        return groupType;
    }

    public void setGroupType(String groupType) {
        this.groupType = groupType;
    }

    public boolean isSupportTopic() {
        return supportTopic;
    }

    public void setSupportTopic(boolean supportTopic) {
        this.supportTopic = supportTopic;
    }

    public String getGroupName() {
        return groupName;
    }

    public void setGroupName(String groupName) {
        this.groupName = groupName;
    }

    public String getNotification() {
        return notification;
    }

    public void setNotification(String notification) {
        this.notification = notification;
    }

    public String getIntroduction() {
        return introduction;
    }

    public void setIntroduction(String introduction) {
        this.introduction = introduction;
    }

    public String getFaceUrl() {
        return faceUrl;
    }

    public void setFaceUrl(String faceUrl) {
        this.faceUrl = faceUrl;
    }

    public boolean isAllMuted() {
        return isAllMuted;
    }

    public void setAllMuted(boolean allMuted) {
        isAllMuted = allMuted;
    }

    public String getOwner() {
        return owner;
    }

    public long getCreateTime() {
        return createTime;
    }

    public int getGroupAddOpt() {
        return groupAddOpt;
    }

    public void setGroupAddOpt(int groupAddOpt) {
        this.groupAddOpt = groupAddOpt;
    }

    public int getGroupApproveOpt() {
        return groupApproveOpt;
    }

    public void setGroupApproveOpt(int groupApproveOpt) {
        this.groupApproveOpt = groupApproveOpt;
    }

    public Map<String, byte[]> getCustomInfo() {
        return customInfo;
    }

    public void setCustomInfo(Map<String, byte[]> customInfo) {
        this.customInfo = customInfo;
    }

    public long getLastInfoTime() {
        return lastInfoTime;
    }

    public long getLastMessageTime() {
        return lastMessageTime;
    }

    public int getMemberCount() {
        return memberCount;
    }

    public int getOnlineCount() {
        return onlineCount;
    }

    public long getMemberMaxCount() {
        return memberMaxCount;
    }

    public int getRole() {
        return role;
    }

    public int getRecvOpt() {
        return recvOpt;
    }

    public long getJoinTime() {
        return joinTime;
    }
}
