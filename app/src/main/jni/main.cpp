#include "Unity/Call_Unity.h"
#include "Includes/Call_Inc.h"
#include "Includes/Logger.h"
#include "KittyMemory/MemoryPatch.h"
#include "And64InlineHook/And64InlineHook.hpp"
#include "Things/Call_Things.h"

// Screen_SetResolution hook is in Call_Things.h file

// And64InlineHook собирается только для arm64 (__aarch64__), а Substrate MSHookFunction
// работает только на armv7/x86. Выбираем нужный хук по архитектуре.
#if defined(__aarch64__)
#define HOOK_FN(target, replace, orig) A64HookFunction((void *)(target), (void *)(replace), (void **)(orig))
#else
#include <Substrate/SubstrateHook.h>
#define HOOK_FN(target, replace, orig) MSHookFunction((void *)(target), (void *)(replace), (void **)(orig))
#endif

void *hack_thread(void *) {
    ProcMap il2cppMap;

    do {
        il2cppMap = KittyMemory::getLibraryMap("libil2cpp.so");
        sleep(1);
    } while (!isLibraryLoaded("libil2cpp.so") && mlovinit());

    espManager = new ESPManager();

    do {
        sleep(1);
    } while (!isLibraryLoaded(OBFUSCATE("libMyLibName.so")));

    getAddress();

    uintptr_t base = getAbsoluteAddress("libil2cpp.so", 0);
    uintptr_t upA = UpDate.getAddr();
    uintptr_t deA = Destroyz.getAddr();
    LOGI("AIController base=%p Update=%p (rva %p) die=%p (rva %p)", (void *)base,
         (void *)upA, (void *)(upA - base), (void *)deA, (void *)(deA - base));

    if (upA > base && upA - base < 0x10000000) {
        HOOK_FN(upA, _update, &update);
    } else {
        LOGI("AIController.Update NOT FOUND - hook skipped");
    }

    if (deA > base && deA - base < 0x10000000) {
        HOOK_FN(deA, _destroy, &destroyP);
    } else {
        LOGI("AIController.die NOT FOUND - hook skipped");
    }

    return NULL;
}

extern "C" {

JNIEXPORT jobjectArray JNICALL
Java_uk_lgl_modmenu_FloatingModMenuService_getFeatureList(
    JNIEnv *env, jobject context) {

    const char *features[] = {
        OBFUSCATE("Collapse_++ ESP ++"),
        OBFUSCATE("1_CollapseAdd_ButtonOnOff_ENABLE"),
        OBFUSCATE("5_CollapseAdd_ButtonOnOff_COUNT"),
        OBFUSCATE("2_CollapseAdd_ButtonOnOff_LINE"),
        OBFUSCATE("3_CollapseAdd_ButtonOnOff_BOX"),
        OBFUSCATE("4_CollapseAdd_ButtonOnOff_DISTANCE"),
        OBFUSCATE("6_CollapseAdd_ButtonOnOff_HEALTH"),
        OBFUSCATE("RichTextView_+++Cheats+++"),
        OBFUSCATE("7_Toggle_Empty")
    };

    // В массиве features ровно 9 элементов.
    const int Total_Feature = 9;

    jclass stringClass = env->FindClass("java/lang/String");
    if (stringClass == nullptr) {
        return nullptr;
    }

    jobjectArray ret = env->NewObjectArray(
        Total_Feature,
        stringClass,
        env->NewStringUTF("")
    );

    if (ret == nullptr) {
        return nullptr;
    }

    for (int i = 0; i < Total_Feature; i++) {
        jstring feature = env->NewStringUTF(features[i]);
        if (feature == nullptr) {
            return nullptr;
        }

        env->SetObjectArrayElement(ret, i, feature);
        env->DeleteLocalRef(feature);
    }

    env->DeleteLocalRef(stringClass);
    return ret;
}

JNIEXPORT void JNICALL
Java_uk_lgl_modmenu_Preferences_Changes(
    JNIEnv *env,
    jclass clazz,
    jobject obj,
    jint featNum,
    jstring featName,
    jint value,
    jboolean boolean,
    jstring str) {

    switch (featNum) {
        case 1:
            ESP = boolean;
            if (!boolean && espManager) espManager->clear();
            break;

        case 2:
            ESPLine = boolean;
            break;

        case 3:
            ESPBox = boolean;
            break;

        case 4:
            ESPDistance = boolean;
            break;

        case 5:
            EspObject = boolean;
            break;

        case 6:
            ESPHealth = boolean;
            break;

        case 7:
            Ammo = boolean;
            break;
    }
}

} // extern "C"

__attribute__((constructor))
void lib_main() {
    pthread_t ptid;
    pthread_create(&ptid, NULL, hack_thread, NULL);
}
