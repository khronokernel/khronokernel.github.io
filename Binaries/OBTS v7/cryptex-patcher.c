
/*
    Cryptex Patcher
    ---------------
    Demonstration of Apple's patching system for Rapid Security Response (RSR) updates.
    Presented at Objective by the Sea v7.0 (2024) by Mykola Grymalyuk.

    Credit to DhinakG and ASentientBot for their research on RawImagePatch and the RIDIFF10 format.

    ----------------
    Compile: clang -fmodules cryptex-patcher.c -o cryptex-patcher

    ----------------
    Usage: cryptex-patcher <inputFile> <outputFile> <patchFile>

    Example: cryptex-patcher OS.dmg OS-Patched.dmg rsr
*/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <dlfcn.h>


// MARK: - RawImagePatchArgs

typedef struct
{
    void (*progressFunc)(void *key, float progress); // Optional
    void *progressFuncKey;                           // Optional
    const char *inputFile;                           // Optional. If missing, `*full replacement*`.
    const char *outputFile;
    const char *patchFile;
    bool isNotCryptexCache;
    uint32_t threadCount;
    uint32_t verboseLevel;
} RawImagePatchArgs;


// MARK: - RawImagePatch

int RawImagePatch(RawImagePatchArgs *args) {

    void *lib = dlopen("/usr/lib/libParallelCompression.dylib", RTLD_LAZY);
    if (!lib) {
        fprintf(stderr, "Failed to load the library\n");
        return 1;
    }

    int (*RawImagePatch)(RawImagePatchArgs*) = dlsym(lib, "RawImagePatch");
    if (!RawImagePatch) {
        fprintf(stderr, "Failed to get the function\n");
        return 1;
    }

    int result = RawImagePatch(args);
    if (result != 0) {
        fprintf(stderr, "The function returned %d\n", result);
        return 1;
    }

    dlclose(lib);

    return 0;
}

// MARK: - Main

int main(int argc, char *argv[])
{
    RawImagePatchArgs args = {0};

    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s <inputFile> <outputFile> <patchFile>\n", argv[0]);
        return 1;
    }

    // Set the parameters
    args.inputFile  = argv[1];
    args.outputFile = argv[2];
    args.patchFile  = argv[3];

    // Fill the rest
    args.isNotCryptexCache = false;
    args.threadCount = 2;
    args.verboseLevel = 2;

    return RawImagePatch(&args);
}


