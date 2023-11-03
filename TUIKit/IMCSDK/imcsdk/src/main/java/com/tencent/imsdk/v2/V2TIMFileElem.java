package com.tencent.imsdk.v2;

import android.text.TextUtils;

import com.tencent.imsdk.common.IMCallback;

/**
 * api docs链接：https://im.sdk.qcloud.com/doc/zh-cn/classcom_1_1tencent_1_1imsdk_1_1v2_1_1V2TIMFileElem.html
 */
public class V2TIMFileElem extends V2TIMElem {
    private String path;
    private String fileName;
    private String UUID;
    private int fileSize;

    //仅用于下载，不对外公开
    private String fileId;
    private String url;

    public V2TIMFileElem() {
        setElementType(V2TIMMessage.V2TIM_ELEM_TYPE_FILE);
    }

    protected void setPath(String path) {
        this.path = path;
    }

    public String getPath() {
        return path;
    }

    protected void setFileName(String fileName) {
        this.fileName = fileName;
    }

    public String getFileName() {
        return fileName;
    }

    public String getUUID() {
        return UUID;
    }

    public int getFileSize() {
        return fileSize;
    }

    public void downloadFile(final String path, final V2TIMDownloadCallback v2TIMDownloadCallback) {
        nativeDownloadFile(fileId,UUID,url, path, new IMCallback(v2TIMDownloadCallback) {
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
        if (url != null && TextUtils.isEmpty(url)){
            callback.onSuccess(url);
        }else {
            callback.onError(6017,"get sound url is fail.");
        }
    }

    @Override
    public String toString() {
        return "V2TIMFileElem{" +
                "path='" + path + '\'' +
                ", fileName='" + fileName + '\'' +
                ", UUID='" + UUID + '\'' +
                ", fileSize=" + fileSize +
                '}';
    }


    //底层方法
    native void nativeDownloadFile(String fileId,String uuid,String url, String path, IMCallback callback);
}
