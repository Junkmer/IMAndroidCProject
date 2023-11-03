package com.tencent.imsdk.v2;

import com.tencent.imsdk.common.IMCallback;

import java.util.ArrayList;
import java.util.List;

/**
 * api docs链接：https://im.sdk.qcloud.com/doc/zh-cn/classcom_1_1tencent_1_1imsdk_1_1v2_1_1V2TIMMergerElem.html
 */
public class V2TIMMergerElem extends V2TIMElem {
    private boolean layersOverLimit;
    //仅用于发送，不用于接收，查看合并消息通过 downloadMergerMessage 接口。
    private List<V2TIMMessage> messageList = new ArrayList<>();
    private String title;
    private List<String> abstractList = new ArrayList<>();
    private String compatibleText;
    //用于下载合并消息
    private String relayPbKey;  // string, 只读, native 端消息列表下载 key
    private String relayJsonKey;// string, 只读, web 端消息列表下载 key
    private String relayBuffer; // string, 只读, 转发消息的 buffer

    public V2TIMMergerElem() {
        setElementType(V2TIMMessage.V2TIM_ELEM_TYPE_MERGER);
    }

    protected void setMessageList(List<V2TIMMessage> messageList) {
        this.messageList = messageList;
    }

    private void addAbstract(String item) {
        abstractList.add(item);
    }

    public boolean isLayersOverLimit() {
        return layersOverLimit;
    }

    protected void setTitle(String title) {
        this.title = title;
    }

    public String getTitle() {
        return title;
    }

    protected void setAbstractList(List<String> abstractList) {
        this.abstractList = abstractList;
    }

    public List<String> getAbstractList() {
        return abstractList;
    }

    protected void setCompatibleText(String compatibleText) {
        this.compatibleText = compatibleText;
    }

    public void downloadMergerMessage(final V2TIMValueCallback<List<V2TIMMessage>> callback) {
        V2TIMMessage message = new V2TIMMessage();
        V2TIMMergerElem mergerElem = new V2TIMMergerElem();
        mergerElem.relayPbKey = relayPbKey;
        mergerElem.relayJsonKey = relayJsonKey;
        mergerElem.relayBuffer = relayBuffer;
        message.addElem(mergerElem);
        nativeDownloadMergerMessage(this, new IMCallback<List<V2TIMMessage>>(callback) {
            @Override
            public void success(List<V2TIMMessage> messageList) {
                super.success(messageList);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });
    }

    @Override
    public String toString() {
        return "V2TIMMergerElem{" +
                "layersOverLimit=" + layersOverLimit +
                ", title='" + title + '\'' +
                ", abstractList=" + abstractList +
                '}';
    }

    //底层方法
    native void nativeDownloadMergerMessage(V2TIMMergerElem elem, IMCallback callback);
}
