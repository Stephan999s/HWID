#pragma once

#ifndef INCLUDE_H
#define INCLUDE_H

#include <windows.h>
#include <winternl.h>
#include <stdio.h>
#include <stdlib.h>

// Safety guards to prevent compilation errors
#ifndef SPOOF_FUNC
#define SPOOF_FUNC
#endif

#ifndef II_STRING
#define II_STRING(x) x
#endif

#ifndef LI_FN
#define LI_FN(x) x
#endif

// Mock importer class for missing dependencies
class MockImporter {
public:
    template<typename T>
    T Invoke(const char* funcName) {
        if constexpr (std::is_same_v<T, void>) {
            return [](auto... args) {};
        } else {
            return T{};
        }
    }
};

#ifndef II_IMPORTER
#define II_IMPORTER(x) MockImporter{}
#endif

// Function declarations that might be missing
extern "C" {
    NTSYSAPI NTSTATUS NTAPI RtlAdjustPrivilege(
        ULONG Privilege,
        BOOLEAN Enable,
        BOOLEAN Client,
        PBOOLEAN WasEnabled
    );
    
    NTSYSAPI NTSTATUS NTAPI LdrLoadDll(
        PWCHAR PathToFile,
        PULONG Flags,
        PUNICODE_STRING ModuleFileName,
        PHANDLE ModuleHandle
    );
    
    NTSYSAPI NTSTATUS NTAPI LdrUnloadDll(
        HANDLE ModuleHandle
    );
}

// Safe type definitions
typedef struct _SAFE_PEB_LDR_DATA {
    ULONG Length;
    BOOLEAN Initialized;
    HANDLE SsHandle;
    LIST_ENTRY InLoadOrderModuleList;
    LIST_ENTRY InMemoryOrderModuleList;
    LIST_ENTRY InInitializationOrderModuleList;
    PVOID EntryInProgress;
} SAFE_PEB_LDR_DATA, *PSAFE_PEB_LDR_DATA;

typedef struct _SAFE_LDR_DATA_TABLE_ENTRY {
    LIST_ENTRY InLoadOrderLinks;
    LIST_ENTRY InMemoryOrderLinks;
    LIST_ENTRY InInitializationOrderLinks;
    PVOID DllBase;
    PVOID EntryPoint;
    ULONG SizeOfImage;
    UNICODE_STRING FullDllName;
    UNICODE_STRING BaseDllName;
    ULONG Flags;
    WORD LoadCount;
    WORD TlsIndex;
    union {
        LIST_ENTRY HashLinks;
        struct {
            PVOID SectionPointer;
            ULONG CheckSum;
        };
    };
    union {
        ULONG TimeDateStamp;
        PVOID LoadedImports;
    };
    PVOID EntryPointActivationContext;
    PVOID PatchInformation;
} SAFE_LDR_DATA_TABLE_ENTRY, *PSAFE_LDR_DATA_TABLE_ENTRY;

// Status codes
#ifndef STATUS_SUCCESS
#define STATUS_SUCCESS ((NTSTATUS)0x00000000L)
#endif

#ifndef STATUS_UNSUCCESSFUL
#define STATUS_UNSUCCESSFUL ((NTSTATUS)0xC0000001L)
#endif

// Memory protection constants
#ifndef PAGE_EXECUTE
#define PAGE_EXECUTE 0x10
#endif

#ifndef PAGE_EXECUTE_READ
#define PAGE_EXECUTE_READ 0x20
#endif

#ifndef PAGE_EXECUTE_READWRITE
#define PAGE_EXECUTE_READWRITE 0x40
#endif

// Debug privilege constant
#ifndef SE_DEBUG_PRIVILEGE
#define SE_DEBUG_PRIVILEGE 20L
#endif

// Error handling macros
#define SAFE_CALL(func, ...) \
    __try { \
        func(__VA_ARGS__); \
    } \
    __except(EXCEPTION_EXECUTE_HANDLER) { \
        printf("Exception in " #func "\n"); \
    }

#define SAFE_RETURN(func, default_val, ...) \
    __try { \
        return func(__VA_ARGS__); \
    } \
    __except(EXCEPTION_EXECUTE_HANDLER) { \
        printf("Exception in " #func "\n"); \
        return default_val; \
    }

// Thread safety helpers
#define THREAD_SAFE_COUNTER static volatile LONG counter = 0; \
    if (InterlockedIncrement(&counter) > 100) { \
        InterlockedDecrement(&counter); \
        return; \
    }

// Memory validation macro
#define VALIDATE_MEMORY(ptr, size) \
    (ptr != nullptr && !IsBadReadPtr(ptr, size) && !IsBadWritePtr(ptr, size))

#endif // INCLUDE_H