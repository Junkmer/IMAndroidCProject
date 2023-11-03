package com.tencent.imsdk.v2;

import java.util.Map;

/**
 * api docs链接：https://im.sdk.qcloud.com/doc/zh-cn/classcom_1_1tencent_1_1imsdk_1_1v2_1_1V2TIMGroupMemberFullInfo.html
 */
public class V2TIMGroupMemberFullInfo extends V2TIMGroupMemberInfo {
    public static final int V2TIM_GROUP_MEMBER_UNDEFINED = 0;
    public static final int V2TIM_GROUP_MEMBER_ROLE_MEMBER = 200;
    public static final int V2TIM_GROUP_MEMBER_ROLE_ADMIN = 300;
    public static final int V2TIM_GROUP_MEMBER_ROLE_OWNER = 400;
    public static final int V2TIM_GROUP_MEMBER_FILTER_ALL = 0x00;
    public static final int V2TIM_GROUP_MEMBER_FILTER_OWNER = 0x01;
    public static final int V2TIM_GROUP_MEMBER_FILTER_ADMIN = 0x02;
    public static final int V2TIM_GROUP_MEMBER_FILTER_COMMON = 0x04;

    private Map<String, byte[]> customInfo;
    private int role;
    private long muteUntil;
    private long joinTime;

    public void setUserID(String userID) {
        super.setUserID(userID);
    }

    public void setNameCard(String nameCard) {
        super.setNameCard(nameCard);
    }

    public Map<String, byte[]> getCustomInfo() {
        return customInfo;
    }

    public void setCustomInfo(Map<String, byte[]> customInfo) {
        this.customInfo = customInfo;
    }

    public int getRole() {
        return role;
    }

    public long getMuteUntil() {
        return muteUntil;
    }

    public long getJoinTime() {
        return joinTime;
    }

    @Override
    public String toString() {
        return "V2TIMGroupMemberFullInfo{" +
                "userID='" + getUserID() + '\'' +
                ", nameCard='" + getNameCard() + '\'' +
                ", customInfo=" + customInfo +
                ", role=" + role +
                ", muteUntil=" + muteUntil +
                ", joinTime=" + joinTime +
                ", nickName='" + getNickName() + '\'' +
                ", friendRemark='" + getFriendRemark() + '\'' +
                ", faceUrl='" + getFaceUrl() + '\'' +
                '}';
    }
}
