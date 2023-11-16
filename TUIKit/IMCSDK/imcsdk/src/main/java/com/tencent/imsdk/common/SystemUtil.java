package com.tencent.imsdk.common;

import android.annotation.SuppressLint;
import android.app.ActivityManager;
import android.content.Context;
import android.content.SharedPreferences;
import android.os.Build;
import android.text.TextUtils;
import android.util.Log;

import java.io.File;
import java.util.List;
import java.util.UUID;
import java.util.regex.Pattern;

public class SystemUtil {
    private static final String TAG = SystemUtil.class.getSimpleName();

    private static boolean loadLibrarySuccess = false;

    public static boolean isLoadLibrarySuccess() {
        return loadLibrarySuccess;
    }

    private static String buildBrand = "";
    private static String buildManufacturer = "";
    private static String buildModel = "";
    private static String buildVersionRelease = "";
    private static int buildVersionSDKInt = 0;
    private static String currentProcessName = "";

    private static final String DEVICE_INFO = "DeviceInfo";
    private static final String DEVICE_ID = "DeviceID";

    public static boolean loadIMLibrary() {
        if (loadLibrarySuccess) {
            return true;
        }
        try {
            System.loadLibrary("ImCSDKDemo");
            loadLibrarySuccess = true;
            Log.i(TAG, "system load so library success, libImSDK.so");
        } catch (UnsatisfiedLinkError e) {
            loadLibrarySuccess = false;
            Log.e(TAG, "system load so library failed, UnsatisfiedLinkError = " + e.getMessage());
        } catch (Exception e) {
            loadLibrarySuccess = false;
            Log.e(TAG, "system load so library failed, Exception = " + e.getMessage());
        }
        return loadLibrarySuccess;
    }

    public static void setBuildModel(String model) {
        buildModel = model;
    }

    public static void setBuildBrand(String brand) {
        buildBrand = brand;
    }

    public static void setBuildManufacturer(String manufacturer) {
        buildManufacturer = manufacturer;
    }

    public static void setBuildVersionRelease(String versionRelease) {
        buildVersionRelease = versionRelease;
    }

    public static void setBuildVersionSDKInt(int versionSDKInt) {
        buildVersionSDKInt = versionSDKInt;
    }

    public static String getDeviceType() {
        if (TextUtils.isEmpty(buildModel)) {
            buildModel = Build.MODEL;
        }

        return buildModel;
    }

    public static String getSystemVersion() {
        if (TextUtils.isEmpty(buildVersionRelease)) {
            buildVersionRelease = Build.VERSION.RELEASE;
        }

        return buildVersionRelease;
    }

    public static int getSDKVersion() {
        if (buildVersionSDKInt == 0) {
            buildVersionSDKInt = Build.VERSION.SDK_INT;
        }

        return buildVersionSDKInt;
    }

    public static String getDeviceID() {
        Context context = IMContext.getInstance().getApplicationContext();
        if (null == context) {
            return "";
        }

        String deviceId = "";
        boolean isNeedSave = false;
        SharedPreferences preferences = context.getSharedPreferences(DEVICE_INFO, Context.MODE_PRIVATE);
        if (!preferences.contains(DEVICE_ID)) {
            deviceId = UUID.randomUUID().toString();
            isNeedSave = true;
        } else {
            deviceId = preferences.getString(DEVICE_ID, "");

            // 检测是否符合UUID的形式，不符合则重新生成
            String pattern = "\\w{8}(-\\w{4}){3}-\\w{12}";
            boolean isMatch = Pattern.matches(pattern, deviceId);
            if (!isMatch || TextUtils.isEmpty(deviceId)) {
                deviceId = UUID.randomUUID().toString();
                isNeedSave = true;
            }
        }

        if (isNeedSave) {
            SharedPreferences.Editor editor = preferences.edit();
            editor.putString(DEVICE_ID, deviceId);
            editor.apply();
        }

        return deviceId;
    }

    public static int getInstanceType() {
        int vendorId = 2002;

        if (isBrandXiaoMi()) {
            vendorId = 2000;
        } else if (isBrandNewHonor()) {
            vendorId = 2006;
        } else if (isBrandHuawei()) {
            vendorId = 2001;
        } else if (isBrandMeizu()) {
            vendorId = 2003;
        } else if (isBrandOppo()) {
            vendorId = 2004;
        } else if (isBrandVivo()) {
            vendorId = 2005;
        }

        return vendorId;
    }

    static boolean isBrandXiaoMi() {
        String brandXiaoMi = "xiaomi";
        return brandXiaoMi.equalsIgnoreCase(getBuildBrand()) || brandXiaoMi.equalsIgnoreCase(getBuildManufacturer());
    }

    static boolean isBrandHuawei() {
        String brandHuaWei = "huawei";
        String brandHonor = "honor";
        return brandHuaWei.equalsIgnoreCase(getBuildBrand()) || brandHuaWei.equalsIgnoreCase(getBuildManufacturer())
                || brandHonor.equalsIgnoreCase(getBuildBrand());
    }

    static boolean isBrandMeizu() {
        String brandMeiZu = "meizu";
        return brandMeiZu.equalsIgnoreCase(getBuildBrand()) || brandMeiZu.equalsIgnoreCase(getBuildManufacturer());
    }

    static boolean isBrandOppo() {
        return "oppo".equalsIgnoreCase(getBuildBrand()) || "oppo".equalsIgnoreCase(getBuildManufacturer())
                || "realme".equalsIgnoreCase(getBuildBrand()) || "realme".equalsIgnoreCase(getBuildManufacturer())
                || "oneplus".equalsIgnoreCase(getBuildBrand()) || "oneplus".equalsIgnoreCase(getBuildManufacturer());
    }

    static boolean isBrandVivo() {
        String brandVivo = "vivo";
        return brandVivo.equalsIgnoreCase(getBuildBrand()) || brandVivo.equalsIgnoreCase(getBuildManufacturer());
    }

    static boolean isBrandNewHonor() {
        return "honor".equalsIgnoreCase(getBuildBrand()) && "honor".equalsIgnoreCase(getBuildManufacturer());
    }

    private static String getBuildBrand() {
        if (TextUtils.isEmpty(buildBrand)) {
            buildBrand = Build.BRAND;
        }

        return buildBrand;
    }

    private static String getBuildManufacturer() {
        if (TextUtils.isEmpty(buildManufacturer)) {
            buildManufacturer = Build.MANUFACTURER;
        }

        return buildManufacturer;
    }

    public static String getSDKInitPath() {
        Context context = IMContext.getInstance().getApplicationContext();
        if (null == context) {
            return "";
        }

        String initPath = context.getFilesDir().toString();
        String mainProcessName = context.getPackageName();
        String currentProcessName = getCurrentProcessName(context);
        if (!TextUtils.isEmpty(currentProcessName) && !mainProcessName.equals(currentProcessName)) {
            try {
                String pathSuffix = currentProcessName;
                int index = currentProcessName.lastIndexOf(":");
                if (index < 0) {
                    index = currentProcessName.lastIndexOf(".");
                }

                if (index >= 0) {
                    pathSuffix = currentProcessName.substring(index + 1);
                }

                initPath += File.separator + pathSuffix + File.separator;
            } catch (Exception e) {
                Log.e(TAG, "getSDKInitPath exception = " + e);
            }
        }

        Log.d(TAG, "SDK Init Path: " + initPath);

        return initPath;
    }

    @SuppressLint("SdCardPath")
    public static String getSDKLogPath() {
        Context context = IMContext.getInstance().getApplicationContext();
        if (null == context) {
            return "";
        }

        // Android 10 系统开始使用分区存储，因此使用应用专有目录。
        String logPath = "";
        File filesDir = context.getExternalFilesDir(null);
        if (filesDir == null) {
            logPath = "/sdcard/Android/data/" + context.getPackageName() + "/log/tencent/imsdk";
        } else {
            logPath = filesDir.getAbsolutePath() + File.separator + "log" + File.separator + "tencent" + File.separator + "imsdk";
        }

        File file = new File(logPath);
        if (!file.exists()) {
            if (!file.mkdirs()) {
                file = context.getFilesDir();
                Log.e(TAG, "create log folder failed");
            }
        }

        logPath = file.getAbsolutePath() + File.separator;
        String mainProcessName = context.getPackageName();
        String currentProcessName = getCurrentProcessName(context);
        if (!TextUtils.isEmpty(currentProcessName) && !mainProcessName.equals(currentProcessName)) {
            try {
                String pathSuffix = currentProcessName;
                int index = currentProcessName.lastIndexOf(":");
                if (index < 0) {
                    index = currentProcessName.lastIndexOf(".");
                }

                if (index >= 0) {
                    pathSuffix = currentProcessName.substring(index + 1);
                }

                logPath += pathSuffix + File.separator;
            } catch (Exception e) {
                Log.e(TAG, "getSDKLogPath exception = " + e);
            }
        }

        Log.d(TAG, "SDK LOG Path: " + logPath);

        return logPath;
    }

    private static String getCurrentProcessName(Context context) {
        if (!TextUtils.isEmpty(currentProcessName)) {
            return currentProcessName;
        }

        ActivityManager am = ((ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE));
        int myPid = android.os.Process.myPid();

        List<ActivityManager.RunningAppProcessInfo> processInfos = am.getRunningAppProcesses();
        if (processInfos == null) {
            Log.d(TAG, "RunningAppProcesses is null");
            List<ActivityManager.RunningServiceInfo> processList = am.getRunningServices(Integer.MAX_VALUE);
            if (processList == null) {
                Log.d(TAG, "RunningServices is null");
                return null;
            }
            for (ActivityManager.RunningServiceInfo rsi : processList) {
                if (rsi.pid == myPid) {
                    currentProcessName = rsi.service.getPackageName();
                }
            }
            return currentProcessName;
        }

        for (ActivityManager.RunningAppProcessInfo info : processInfos) {
            if (info.pid == myPid) {
                currentProcessName = info.processName;
            }
        }
        return currentProcessName;
    }

}
