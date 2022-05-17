package com.tencent.mtt.hippy.utils;

import android.graphics.Bitmap;
import android.graphics.Bitmap.CompressFormat;
import android.util.Base64;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewTreeObserver;
import android.view.ViewTreeObserver.OnDrawListener;
import androidx.annotation.NonNull;
import com.tencent.mtt.hippy.common.HippyMap;
import com.tencent.mtt.hippy.modules.Promise;
import com.tencent.renderer.NativeRender;
import com.tencent.renderer.NativeRendererManager;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.util.HashMap;
import java.util.List;

/**
 * devtools utils for screen shot
 */
public class DevtoolsUtil {
    public static final String GET_SCREEN_SHOT = "getScreenShot";
    public static final String ADD_FRAME_CALLBACK = "addFrameCallback";
    public static final String REMOVE_FRAME_CALLBACK = "removeFrameCallback";
    private static final String TAG = "tdf_DevtoolsUtil";
    private static final String SCREEN_SHOT = "screenShot";
    private static final String SCREEN_WIDTH = "width";
    private static final String SCREEN_HEIGHT = "height";
    private static final String SCREEN_SCALE = "screenScale";
    private static final String FRAME_CALLBACK_ID = "frameCallbackId";
    private static final float DEFAULT_SCALE = 0.5f;
    private static final int DEFAULT_QUALITY = 80;
    private final static HashMap<Integer, ViewTreeObserver.OnDrawListener> sDrawListeners = new HashMap<>();

    @SuppressWarnings("unchecked")  // instanceof can't check for (HashMap<String, Object>) , so use SuppressWarnings
    public static void addFrameCallback(@NonNull List params, @NonNull View view, @NonNull final Promise promise) {
        NativeRender nativeRenderer = NativeRendererManager.getNativeRenderer(view.getContext());
        if (nativeRenderer == null) {
            return;
        }
        ViewGroup rootView = nativeRenderer.getRootView();
        if (rootView == null) {
            return;
        }
        if (params.isEmpty()) {
            return;
        }
        Object paramMap = params.get(0);
        if (!(paramMap instanceof HashMap)) {
            return;
        }
        HashMap<String, Object> hashMap = (HashMap<String, Object>) paramMap;
        Object callbackId = hashMap.get(FRAME_CALLBACK_ID);
        if (!(callbackId instanceof Integer)) {
            return;
        }
        ViewTreeObserver.OnDrawListener drawListener = new OnDrawListener() {
            @Override
            public void onDraw() {
                promise.resolve(new HippyMap());
            }
        };
        sDrawListeners.put((Integer) callbackId, drawListener);
        rootView.getViewTreeObserver().addOnDrawListener(drawListener);
    }

    @SuppressWarnings("unchecked")  // instanceof can't check for (HashMap<String, Object>) , so use SuppressWarnings
    public static void removeFrameCallback(@NonNull List params, @NonNull View view, @NonNull Promise promise) {
        NativeRender nativeRenderer = NativeRendererManager.getNativeRenderer(view.getContext());
        if (nativeRenderer == null) {
            return;
        }
        ViewGroup rootView = nativeRenderer.getRootView();
        if (rootView == null) {
            return;
        }
        if (params.isEmpty()) {
            return;
        }
        Object paramMap = params.get(0);
        if (!(paramMap instanceof HashMap)) {
            return;
        }
        HashMap<String, Object> hashMap = (HashMap<String, Object>) paramMap;
        Object callbackId = hashMap.get(FRAME_CALLBACK_ID);
        if (!(callbackId instanceof Integer)) {
            return;
        }
        ViewTreeObserver.OnDrawListener drawListener = sDrawListeners.remove(callbackId);
        if (drawListener != null) {
            rootView.getViewTreeObserver().removeOnDrawListener(drawListener);
        }
    }

    public static void getScreenShot(@NonNull View view, @NonNull Promise promise) {
        boolean isEnableDrawingCache = view.isDrawingCacheEnabled();
        if (!isEnableDrawingCache) {
            view.setDrawingCacheEnabled(true);
        }
        Bitmap bitmap = view.getDrawingCache();
        String base64 = bitmapToBase64Str(bitmap);
        HippyMap resultMap = new HippyMap();
        resultMap.pushString(SCREEN_SHOT, base64);
        resultMap.pushInt(SCREEN_WIDTH, (int) (view.getWidth() * DEFAULT_SCALE));
        resultMap.pushInt(SCREEN_HEIGHT, (int) (view.getHeight() * DEFAULT_SCALE));
        resultMap.pushDouble(SCREEN_SCALE, view.getResources().getDisplayMetrics().density * DEFAULT_SCALE);
        promise.resolve(resultMap);
        view.setDrawingCacheEnabled(isEnableDrawingCache);
    }

    private static String bitmapToBase64Str(Bitmap bitmap) {
        String result = null;
        ByteArrayOutputStream baos = null;
        try {
            if (bitmap != null) {
                Bitmap scaleBitmap = Bitmap
                        .createScaledBitmap(bitmap, (int) (bitmap.getWidth() * DEFAULT_SCALE),
                                (int) (bitmap.getHeight() * DEFAULT_SCALE), false);
                baos = new ByteArrayOutputStream();
                scaleBitmap.compress(CompressFormat.JPEG, DEFAULT_QUALITY, baos);
                baos.flush();
                baos.close();
                byte[] bitmapBytes = baos.toByteArray();
                result = Base64.encodeToString(bitmapBytes, Base64.NO_WRAP);
            }
        } catch (IOException e) {
            LogUtils.e(TAG, "bitmapToBase64Str, scale exception:", e);
        } finally {
            try {
                if (baos != null) {
                    baos.close();
                }
            } catch (IOException e) {
                LogUtils.e(TAG, "bitmapToBase64Str, close exception:", e);
            }
        }
        return result;
    }

}
