#pragma once
#include <vector>
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdint>
using namespace std;
#include "NepAU/UnityApi/api.cpp"
namespace NepSymbol {
    #define HOOKAU(offset, ptr, orig) MSHookFunction((void *)offset.getAddr(), (void *) ptr, (void **) &orig);
    uintptr_t getLib(const char *library) {
        char filename[0xFF] = {
            0
        },
        buffer[1024] = {
            0
        };
        FILE *fp = NULL;
        uintptr_t address = 0;
        sprintf(filename, ("/proc/self/maps"));
        fp = fopen(filename, ("rt"));
        if (fp == NULL) {
            perror(("fopen"));
            goto done;
        }
        while (fgets(buffer, sizeof(buffer), fp)) {
            if (strstr(buffer, library)) {
                address = (uintptr_t) strtoul(buffer, NULL, 16);
                goto done;
            }
        }
        done:
        if (fp) {
            fclose(fp);
        }
        return address;
    }
    const char* StringConvert(uintptr_t distance) {
    thread_local char buffer[32];
    snprintf(buffer, sizeof(buffer), "0x%llx",
             static_cast<unsigned long long>(distance));
    return buffer;
}
    typedef struct LibMain {
        bool libil2cpp;
        void *Lib;
        int _size;
        const char *name;
        int size() {
            if (libil2cpp) {
                return 1;
            }
            if (Lib != nullptr) {
                return 1;
            }
            return 0;
        }
    } LibMain;
    typedef struct AddressManager {
        uintptr_t address;
        uintptr_t offset;

        uintptr_t getAddr() {
            return address;
        }
        uintptr_t getOffset() {
            return offset;
        }
        const char *getAddrStr() {
            return NepSymbol::StringConvert(address);
        }
        const char *getOffsetStr() {
            return NepSymbol::StringConvert(offset);
        }
        bool isValid() {
            if (address != 0 && offset != 0) {
                return true;
            }
            return false;
        }
    } Address;
    typedef struct LibData {
        
    }gLib;
    typedef struct UnityClassManager69 {
        const char *NS;
        const char *name;
        LoadClass *klass;
        LibMain lib;
        bool init() {

            if (lib.size() == 1) {
                klass = new LoadClass(NS, name);

                return true;

            }
            return false;
        }
        Address dump(const char *symbol, int paramCount = 0, bool isField = false) {
            
            if(!init()) {
                Address ret;
                return ret;
            }
            uintptr_t address;
            uintptr_t libAddr;
            uintptr_t offset;

            if (lib.libil2cpp) {
                if (!isField) {
                    address = klass->GetMethodOffsetByName(symbol, paramCount);
                    libAddr = getLib(lib.name);
                    offset = reinterpret_cast < uintptr_t > (address - libAddr);
                } else {
                    address = klass->GetFieldOffset(symbol);
                    offset = address;
                }
            } else {
                Address ret;
                return ret;
            }
            Address ret;
            ret.address = address;
            ret.offset = offset;

            return ret;
        }
        Address dump(const char *symbol, std::vector < string > params_names, bool isField = false) {
           if(!init()) {
                Address ret;
                return ret;
            }
            uintptr_t address;
            uintptr_t libAddr;
            uintptr_t offset;

            if (lib.libil2cpp) {
                if (!isField) {
                    address = klass->GetMethodOffsetByName(symbol, params_names.size());
                    libAddr = getLib(lib.name);
                    offset = reinterpret_cast < uintptr_t > (address - libAddr);
                } else {
                    address = klass->GetFieldOffset(symbol);
                    offset = address;
                }
            } else {
                Address ret;
                return ret;
            }
            Address ret;
            ret.address = address;
            ret.offset = offset;

            return ret;
        }
    } Uclass;
    typedef struct LibManager {
        const char *name;
        LibMain Lib;

        bool open(int version = 0) {
            if (name != nullptr &&
    std::strcmp(name, "libil2cpp.so") != 0) {
                Lib.Lib = dlopen(name, RTLD_LOCAL);
                Lib.libil2cpp = false;
                Lib.name = name;
                if (Lib.size() != 0) {
                    return true;
                }
            } else {
                Lib.Lib = NULL;
                Lib.libil2cpp = true;
                Lib.name = name;
                if (Lib.size() != 0) {
                    return true;
                }
            }
            return false;
        }
        Address dump(const char *symbol) {
            uintptr_t address,
            libAddr,
            offset;
            if (Lib.libil2cpp) {
                Address ret;
                return ret;
            } else {
                address = (uintptr_t)dlsym(Lib.Lib, symbol);
                libAddr = getLib(name);
                offset = reinterpret_cast < uintptr_t > (address - libAddr);
            }
            Address ret;
            ret.address = address;
            ret.offset = offset;
            return ret;
        }
       NepSymbol::Uclass getClass(const char *NMS, const char *nadme) {
           NepSymbol::Uclass ret;
            ret.NS = NMS;
            ret.name = nadme;
            ret.lib = Lib;
            return ret;
        }
        NepSymbol::Uclass getClass(const char *nadme) {
           NepSymbol::Uclass ret;
            ret.NS = "";
            ret.name = nadme;
            ret.lib = Lib;
            return ret;
        }
        LibMain getMain() {
            return Lib;
        }
    } lib;
    
  
};
