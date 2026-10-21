bool setHook(const char *offset, const char *update, int value, bool enabled) {
    void (*setHookData)(void* _this, int valuez);
    void (*_SetHook)(void *player);
    void SetHook(void *player) {
        if (player != NULL && enabled) {
            int sp = value;
            setHookData(player, sp);
        }
        return _UnHealth(player);
    }
    setHookData = (void (*)(void *, int))getAbsoluteAddress("libil2cpp.so", string2Offset(offset));
    MSHookFunction((void *) getAbsoluteAddress("libil2cpp.so", string2Offset(update)), (void *) &SetHook, (void **) &_SetHook);
}
void setHook(const char *offset, const char *update, bool value, bool enabled) {
    void (*setHookData)(void* _this, bool valuez);
    void (*_SetHook)(void *player);
    void SetHook(void *player) {
        if (player != NULL && enabled) {
            bool sp = value;
            setHookData(player, sp);
        }
        return _UnHealth(player);
    }
    setHookData = (void (*)(void *, bool))getAbsoluteAddress("libil2cpp.so", string2Offset(offset));
    MSHookFunction((void *) getAbsoluteAddress("libil2cpp.so", string2Offset(update)), (void *) &SetHook, (void **) &_SetHook);
}
void setHook(const char *offset, const char *update, float value, bool enabled) {
    void (*setHookData)(void* _this, float valuez);
    void (*_SetHook)(void *player);
    void SetHook(void *player) {
        if (player != NULL && enabled) {
            float sp = value;
            setHookData(player, sp);
        }
        return _UnHealth(player);
    }
    setHookData = (void (*)(void *, float))getAbsoluteAddress("libil2cpp.so", string2Offset(offset));
    MSHookFunction((void *) getAbsoluteAddress("libil2cpp.so", string2Offset(update)), (void *) &SetHook, (void **) &_SetHook);
}
