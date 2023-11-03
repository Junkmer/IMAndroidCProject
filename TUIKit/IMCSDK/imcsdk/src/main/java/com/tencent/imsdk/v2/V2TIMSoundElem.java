package com.tencent.imsdk.v2;

import android.text.TextUtils;

import com.tencent.imsdk.common.IMCallback;

/**
 * api docs链接：https://im.sdk.qcloud.com/doc/zh-cn/classcom_1_1tencent_1_1imsdk_1_1v2_1_1V2TIMSoundElem.html
 */
public class V2TIMSoundElem extends V2TIMElem {
    private String path;
    private String UUID;
    private int dataSize;
    private int duration;

    //仅用于下载，不对外公开
    private String fileId;
    private String url;

    public V2TIMSoundElem() {
        setElementType(V2TIMMessage.V2TIM_ELEM_TYPE_SOUND);
    }

    protected void setPath(String path) {
        this.path = path;
    }

    public String getPath() {
        return path;
    }

    public String getUUID() {
        return UUID;
    }

    public int getDataSize() {
        return dataSize;
    }

    protected void setDuration(int duration) {
        this.duration = duration;
    }

    public int getDuration() {
        return duration;
    }

    public void downloadSound(String path, final V2TIMDownloadCallback v2TIMDownloadCallback) {
        nativeDownloadSound(fileId, UUID, url, path, new IMCallback(v2TIMDownloadCallback) {
            @Override
            public void downloadProgress(long currentSize, long totalSize) {
                super.downloadProgress(currentSize, totalSize);
            }

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

    public void getUrl(final V2TIMValueCallback<String> callback) {
        if (url != null && !TextUtils.isEmpty(url)) {
            callback.onSuccess(url);
        } else {
            callback.onError(6017, "get sound url is fail.");
        }
    }

    public void convertVoiceToText(String language, final V2TIMValueCallback<String> callback){
        if (url == null || TextUtils.isEmpty(url)) {
            callback.onError(6017, "convertVoiceToText is fail.");
        }

        nativeConvertVoiceToText(url, language, new IMCallback(callback) {
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
    public String toString() {
        return "V2TIMSoundElem{" +
                "path='" + path + '\'' +
                ", UUID='" + UUID + '\'' +
                ", dataSize=" + dataSize +
                ", duration=" + duration +
                '}';
    }

    //底层方法
    native void nativeDownloadSound(String fileId, String uuid, String url, String path, IMCallback callback);

    native void nativeConvertVoiceToText(String url, String language, IMCallback callback);
}
