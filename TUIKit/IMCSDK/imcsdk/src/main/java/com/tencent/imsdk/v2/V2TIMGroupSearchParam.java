package com.tencent.imsdk.v2;

import java.io.Serializable;
import java.util.List;

/**
 * api docs链接：https://im.sdk.qcloud.com/doc/zh-cn/classcom_1_1tencent_1_1imsdk_1_1v2_1_1V2TIMGroupSearchParam.html
 */
public class V2TIMGroupSearchParam implements Serializable {
    private List< String > keywordList;
    private boolean isSearchGroupID;
    private boolean isSearchGroupName;

    public void setKeywordList(List<String> keywordList) {
        this.keywordList = keywordList;
    }

    public void setSearchGroupID(boolean searchGroupID) {
        isSearchGroupID = searchGroupID;
    }

    public void setSearchGroupName(boolean searchGroupName) {
        isSearchGroupName = searchGroupName;
    }
}
