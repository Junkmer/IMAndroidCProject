package com.tencent.imsdk.v2;

import android.text.TextUtils;

import com.tencent.imsdk.common.IMCallback;

import java.util.List;

public class V2TIMConversationManagerImpl extends V2TIMConversationManager {

    private static class V2TIMConversationManagerImplHolder {
        private static final V2TIMConversationManagerImpl v2TIMConversationManager = new V2TIMConversationManagerImpl();
    }

    static V2TIMConversationManagerImpl getInstance() {
        return V2TIMConversationManagerImplHolder.v2TIMConversationManager;
    }

    @Override
    public void addConversationListener(V2TIMConversationListener listener) {
        nativeAddConversationListener(listener, listener.toString());
    }

    @Override
    public void removeConversationListener(V2TIMConversationListener listener) {
        nativeRemoveConversationListener(listener.toString());
    }

    @Override
    public void getConversationList(long nextSeq, int count, V2TIMValueCallback<V2TIMConversationResult> _callback_) {

        nativeGetConversationList(nextSeq, count, new IMCallback<V2TIMConversationResult>(_callback_) {
            @Override
            public void success(V2TIMConversationResult data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });
    }

    @Override
    public void getConversation(String conversationID, V2TIMValueCallback<V2TIMConversation> _callback_) {
        nativeGetConversation(conversationID, new IMCallback<V2TIMConversation>(_callback_) {
            @Override
            public void success(V2TIMConversation data) {
                if (data != null && TextUtils.isEmpty(data.getConversationID())){
                    data.setConversationID(conversationID);
                    if (conversationID.startsWith("c2c_")){
                        data.setType(V2TIMConversation.V2TIM_C2C);
                    }else {
                        data.setType(V2TIMConversation.V2TIM_GROUP);
                    }
                }
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });
    }

    @Override
    public void getConversationList(List<String> conversationIDList, V2TIMValueCallback<List<V2TIMConversation>> _callback_) {
        nativeGetConversationList(conversationIDList, new IMCallback<List<V2TIMConversation>>(_callback_) {
            @Override
            public void success(List<V2TIMConversation> data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });
    }

    @Override
    public void getConversationListByFilter(V2TIMConversationListFilter filter, long nextSeq, int count, V2TIMValueCallback<V2TIMConversationResult> _callback_) {
        nativeGetConversationListByFilter(filter, nextSeq, count, new IMCallback<V2TIMConversationResult>(_callback_) {
            @Override
            public void success(V2TIMConversationResult data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });
    }

    @Override
    public void deleteConversation(String conversationID, V2TIMCallback _callback_) {
        nativeDeleteConversation(conversationID, new IMCallback(_callback_) {
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
    public void deleteConversationList(List<String> conversationIDList, boolean clearMessage, V2TIMValueCallback<List<V2TIMConversationOperationResult>> _callback_) {
        nativeDeleteConversationList(conversationIDList, clearMessage, new IMCallback<List<V2TIMConversationOperationResult>>(_callback_) {
            @Override
            public void success(List<V2TIMConversationOperationResult> data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });
    }

    @Override
    public void setConversationDraft(String conversationID, String draftText, V2TIMCallback _callback_) {
        nativeSetConversationDraft(conversationID, draftText, new IMCallback(_callback_) {
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
    public void setConversationCustomData(List<String> conversationIDList, String customData, V2TIMValueCallback<List<V2TIMConversationOperationResult>> _callback_) {
        nativeSetConversationCustomData(conversationIDList, customData, new IMCallback<List<V2TIMConversationOperationResult>>(_callback_) {
            @Override
            public void success(List<V2TIMConversationOperationResult> data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });
    }

    @Override
    public void pinConversation(String conversationID, boolean isPinned, V2TIMCallback _callback_) {
        nativePinConversation(conversationID, isPinned, new IMCallback(_callback_) {
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
    public void markConversation(List<String> conversationIDList, long markType, boolean enableMark, V2TIMValueCallback<List<V2TIMConversationOperationResult>> _callback_) {
        nativeMarkConversation(conversationIDList, markType, enableMark, new IMCallback<List<V2TIMConversationOperationResult>>(_callback_) {
            @Override
            public void success(List<V2TIMConversationOperationResult> data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });
    }

    @Override
    public void getTotalUnreadMessageCount(V2TIMValueCallback<Long> _callback_) {
        nativeGetTotalUnreadMessageCount(new IMCallback<Long>(_callback_) {
            @Override
            public void success(Long data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });
    }

    @Override
    public void getUnreadMessageCountByFilter(V2TIMConversationListFilter filter, V2TIMValueCallback<Long> _callback_) {
        nativeGetUnreadMessageCountByFilter(filter, new IMCallback<Long>(_callback_) {
            @Override
            public void success(Long data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });
    }

    @Override
    public void subscribeUnreadMessageCountByFilter(V2TIMConversationListFilter filter) {
        nativeSubscribeUnreadMessageCountByFilter(filter);
    }

    @Override
    public void unsubscribeUnreadMessageCountByFilter(V2TIMConversationListFilter filter) {
        nativeUnsubscribeUnreadMessageCountByFilter(filter);
    }

    @Override
    public void cleanConversationUnreadMessageCount(String conversationID, long cleanTimestamp, long cleanSequence, V2TIMCallback _callback_) {
        nativeCleanConversationUnreadMessageCount(conversationID, cleanTimestamp, cleanSequence, new IMCallback(_callback_) {
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
    public void createConversationGroup(String groupName, List<String> conversationIDList, V2TIMValueCallback<List<V2TIMConversationOperationResult>> _callback_) {
        nativeCreateConversationGroup(groupName, conversationIDList, new IMCallback<List<V2TIMConversationOperationResult>>(_callback_) {
            @Override
            public void success(List<V2TIMConversationOperationResult> data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });
    }

    @Override
    public void getConversationGroupList(V2TIMValueCallback<List<String>> _callback_) {
        nativeGetConversationGroupList(new IMCallback<List<String>>(_callback_) {
            @Override
            public void success(List<String> data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });
    }

    @Override
    public void deleteConversationGroup(String groupName, V2TIMCallback _callback_) {
        nativeDeleteConversationGroup(groupName, new IMCallback(_callback_) {
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
    public void renameConversationGroup(String oldName, String newName, V2TIMCallback _callback_) {
        nativeRenameConversationGroup(oldName, newName, new IMCallback(_callback_) {
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
    public void addConversationsToGroup(String groupName, List<String> conversationIDList, V2TIMValueCallback<List<V2TIMConversationOperationResult>> _callback_) {
        nativeAddConversationsToGroup(groupName, conversationIDList, new IMCallback<List<V2TIMConversationOperationResult>>(_callback_) {
            @Override
            public void success(List<V2TIMConversationOperationResult> data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });
    }

    @Override
    public void deleteConversationsFromGroup(String groupName, List<String> conversationIDList, V2TIMValueCallback<List<V2TIMConversationOperationResult>> _callback_) {
        nativeDeleteConversationsFromGroup(groupName, conversationIDList, new IMCallback<List<V2TIMConversationOperationResult>>(_callback_) {
            @Override
            public void success(List<V2TIMConversationOperationResult> data) {
                super.success(data);
            }

            @Override
            public void fail(int code, String errorMessage) {
                super.fail(code, errorMessage);
            }
        });
    }
}
