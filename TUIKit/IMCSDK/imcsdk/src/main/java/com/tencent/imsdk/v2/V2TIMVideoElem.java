package com.tencent.imsdk.v2;

import android.text.TextUtils;

import com.tencent.imsdk.common.IMCallback;

/**
 * api docs链接：https://im.sdk.qcloud.com/doc/zh-cn/classcom_1_1tencent_1_1imsdk_1_1v2_1_1V2TIMVideoElem.html
 */
public class V2TIMVideoElem extends V2TIMElem {
    // 视频
    private String videoPath;
    private String videoType;
    private String videoUUID;
    private int videoSize;
    private int duration;
    // 缩略图
    private String snapshotPath;
    private String snapshotUUID;
    private int snapshotSize;
    private int snapshotWidth;
    private int snapshotHeight;

    //仅用于下载，不对外公开
    private String videoFileId;
    private String videoUrl;
    private String snapshotFileId;
    private String snapshotUrl;

    public V2TIMVideoElem() {
        setElementType(V2TIMMessage.V2TIM_ELEM_TYPE_VIDEO);
    }

    protected void setVideoPath(String videoPath) {
        this.videoPath = videoPath;
    }

    public String getVideoPath() {
        return videoPath;
    }

    protected void setVideoType(String videoType) {
        this.videoType = videoType;
    }

    public String getVideoUUID() {
        return videoUUID;
    }

    public int getVideoSize() {
        return videoSize;
    }

    protected void setDuration(int duration) {
        this.duration = duration;
    }

    public int getDuration() {
        return duration;
    }

    public void downloadVideo(final String path, final V2TIMDownloadCallback v2TIMDownloadCallback) {
        nativeDownloadVideo(videoFileId,videoUUID,videoUrl, path, new IMCallback(v2TIMDownloadCallback) {
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

    public void getVideoUrl(final V2TIMValueCallback<String> callback) {
        if (videoUrl != null && TextUtils.isEmpty(videoUrl)){
            callback.onSuccess(videoUrl);
        }else {
            callback.onError(6017,"get sound url is fail.");
        }
    }

    protected void setSnapshotPath(String snapshotPath) {
        this.snapshotPath = snapshotPath;
    }

    public String getSnapshotPath() {
        return snapshotPath;
    }

    public String getSnapshotUUID() {
        return snapshotUUID;
    }

    public int getSnapshotSize() {
        return snapshotSize;
    }

    public int getSnapshotWidth() {
        return snapshotWidth;
    }

    public int getSnapshotHeight() {
        return snapshotHeight;
    }

    public void downloadSnapshot(final String path, final V2TIMDownloadCallback v2TIMDownloadCallback) {
        nativeDownloadSnapshot(snapshotFileId,snapshotUUID,snapshotUrl, path, new IMCallback(v2TIMDownloadCallback) {
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

    public void getSnapshotUrl(final V2TIMValueCallback<String> callback) {
        if (snapshotUrl != null && TextUtils.isEmpty(snapshotUrl)){
            callback.onSuccess(snapshotUrl);
        }else {
            callback.onError(6017,"get sound url is fail.");
        }
    }

    @Override
    public String toString() {
        return "V2TIMVideoElem{" +
                "videoPath='" + videoPath + '\'' +
                ", videoUUID='" + videoUUID + '\'' +
                ", videoSize=" + videoSize +
                ", duration=" + duration +
                ", snapshotPath='" + snapshotPath + '\'' +
                ", snapshotUUID='" + snapshotUUID + '\'' +
                ", snapshotSize=" + snapshotSize +
                ", snapshotWidth=" + snapshotWidth +
                ", snapshotHeight=" + snapshotHeight +
                '}';
    }


    //底层方法
    native void nativeDownloadVideo(String fileId,String uuid,String url, String path, IMCallback callback);
    native void nativeDownloadSnapshot(String fileId,String uuid,String url, String path, IMCallback callback);
}
