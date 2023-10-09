#include <ext_lib.h>
#include "AudioConvert.h"
#include "AudioTools.h"
#include "AudioGui.h"

typedef enum {
    FORMPARAM_BIN,
    FORMPARAM_WAV,
    FORMPARAM_CCC,
} FormatParam;

char* sToolName = {
    "z64audio" PRNT_GRAY " 2.2.0"
};

char* sToolUsage = {
    EXT_INFO_TITLE("File:")
    EXT_INFO("--i [file]",       16, "Input:  .wav .aiff")
    EXT_INFO("--o [file]",       16, "Output: .wav .aiff .bin .c")
    EXT_INFO("--play",           16, "'--play vadpcm' is also an option")
    PRNT_NL
    EXT_INFO_TITLE("Audio Processing:")
    EXT_INFO("--b",              16, "Target Bit Depth, '32f' for float target")
    EXT_INFO("--m",              16, "Mono")
    EXT_INFO("--n",              16, "Normalize")
    PRNT_NL
    EXT_INFO_TITLE("Arguments for [.bin] input:")
    EXT_INFO("--srate",          16, "Set Samplerate")
    EXT_INFO("--tuning",         16, "Set Tuning")
    EXT_INFO("--split-hi",       16, "Set Low Split")
    EXT_INFO("--split-lo",       16, "Set Hi Split")
    EXT_INFO("--half-precision", 22, "Saves space by halfing framesize")
    PRNT_NL
    EXT_INFO_TITLE("VADPCM:")
    EXT_INFO("--p [file]",       16, "Use excisting predictors")
    EXT_INFO("--I [ 30 ]",       16, "Override TableDesign Refine Iteration")
    EXT_INFO("--F [ 16 ]",       16, "Override TableDesign Frame Size")
    EXT_INFO("--B [  2 ]",       16, "Override TableDesign Bits")
    EXT_INFO("--O [  2 ]",       16, "Override TableDesign Order")
    EXT_INFO("--T [ 10 ]",       16, "Override TableDesign Threshold")
    PRNT_NL
    EXT_INFO_TITLE("Extra:")
    EXT_INFO("--P",              16, "Load separate settings [.cfg]")
    EXT_INFO("--log",            16, "Print Debug Log")
    EXT_INFO("--S",              16, "Silence")
    EXT_INFO("--N",              16, "Print Info of input [file]")
};

bool gVadPrev;
bool gRomForceLoop;
FormatParam sDefaultFormat;
extern s32 gOverrideConfig;

// # # # # # # # # # # # # # # # # # # # #
// # Setup                               #
// # # # # # # # # # # # # # # # # # # # #

#define GenerParam(com1, name, defval, com2) MemFile_Printf( \
        param,                                               \
        com1                                                 \
        "\n%-15s = %-8s # %s\n\n",                           \
        # name,                                              \
        # defval,                                            \
        com2                                                 \
)

void Main_Config_Generate(MemFile* param, char* file) {
    MemFile_Alloc(param, 0x1000);
    
    MemFile_Printf(
        param,
        "### z64audio settings ###\n\n"
    );
    GenerParam(
        "# Specialized for z64rom tool usage, do not set unless this is paired\n# with z64rom!",
        zaudio_z64rom_mode,
        false,
        "[true/false]"
    );
    GenerParam(
        "# nameBook.bin vs name_predictors.bin, more suitable for Jared's\n# zzrtl script.",
        zaudio_zz_naming,
        false,
        "[true/false]"
    );
    GenerParam(
        "# Default format to export when drag'n'dropping files on to z64audio.",
        zaudio_def_dnd_fmt,
        "bin",
        "[bin/wav/aiff/c]"
    );
    
    if (MemFile_SaveFile_String(param, file)) {
        printf_error("Could not create param file [%s]", file);
    }
}

void Main_Config(const char* argv[]) {
    MemFile param = MemFile_Initialize();
    char file[256 * 4];
    u32 argID;
    s32 integer;
    char* list[] = {
        "bin", "wav", "aiff", "c", NULL
    };
    
    strcpy(file, Sys_AppDir());
    strcat(file, "z64audio.cfg");
    
    Log("Get: %s", file);
    if ((argID = ArgStr(argv, "P"))) {
        MemFile_LoadFile_String(&param, argv[argID]);
    } else if (MemFile_LoadFile_String(&param, file)) {
        printf_info("Generating settings [%s]", file);
        Main_Config_Generate(&param, file);
    }
    
    gBinNameIndex = Config_GetBool(&param, "zaudio_zz_naming");
    integer = Config_GetOption(&param, "zaudio_def_dnd_fmt", list);
    
    if (integer != 404040404) {
        sDefaultFormat = integer;
    }
    
    MemFile_Free(&param);
    
    if ((argID = ArgStr(argv, "GenCfg"))) {
        exit(65);
    }
}

void Main_LoadSampleConf(const char* conf) {
    MemFile mem = MemFile_Initialize();
    char* param;
    
    if (conf == NULL || !Sys_Stat(conf))
        return;
    
    MemFile_LoadFile_String(&mem, conf);
    
    if ((param = Config_GetVariable(mem.str, "book_iteration"))) gTableDesignIteration = param;
    if ((param = Config_GetVariable(mem.str, "book_frame_size"))) gTableDesignFrameSize = param;
    if ((param = Config_GetVariable(mem.str, "book_bits"))) gTableDesignBits = param;
    if ((param = Config_GetVariable(mem.str, "book_order"))) gTableDesignOrder = param;
    if ((param = Config_GetVariable(mem.str, "book_threshold"))) gTableDesignThreshold = param;
    
    if ((param = Config_GetVariable(mem.str, "sample_rate"))) gBinSampleRate = Value_Int(param);
    if ((param = Config_GetVariable(mem.str, "sample_tuning"))) gTuning = Value_Float(param);
    
    MemFile_Free(&mem);
}

// # # # # # # # # # # # # # # # # # # # #
// # MAIN                                #
// # # # # # # # # # # # # # # # # # # # #

s32 Main(s32 argc, const char* argv[]) {
    AudioSample sample;
    char* input = NULL;
    char* output = NULL;
    u32 argID;
    u32 callSignal = 0;
    
#if 0
    if (argc == 1) {
        WindowContext* winCtx;
        
        Calloc(winCtx, sizeof(WindowContext));
        winCtx->vg = Interface_Init("z64audio", &winCtx->app, &winCtx->input, winCtx, (void*)Window_Update, (void*)Window_Draw, Window_DropCallback, 980, 480, 0);
        
        winCtx->geoGrid.passArg = winCtx;
        winCtx->geoGrid.taskTable = gTaskTable;
        
        Theme_Init(0);
        GeoGrid_Init(&winCtx->geoGrid, &winCtx->app.winDim, &winCtx->input, winCtx->vg);
        Cursor_Init(&winCtx->cursor, &winCtx->app);
        Cursor_CreateCursor(CURSOR_ARROW_U, gCursor_ArrowUData, 24, 12, 12);
        Cursor_CreateCursor(CURSOR_ARROW_D, gCursor_ArrowDData, 24, 12, 12);
        Cursor_CreateCursor(CURSOR_ARROW_L, gCursor_ArrowLData, 24, 12, 12);
        Cursor_CreateCursor(CURSOR_ARROW_R, gCursor_ArrowRData, 24, 12, 12);
        Cursor_CreateCursor(CURSOR_ARROW_H, gCursor_ArrowHData, 32, 16, 16);
        Cursor_CreateCursor(CURSOR_ARROW_V, gCursor_ArrowVData, 32, 16, 16);
        Cursor_CreateCursor(CURSOR_CROSSHAIR, gCursor_CrosshairData, 40, 19, 20);
        Cursor_CreateCursor(CURSOR_EMPTY, gCursor_EmptyData, 16, 0, 0);
        
        Rectf32 size = {
            winCtx->geoGrid.workRect.x,
            winCtx->geoGrid.workRect.y,
            winCtx->geoGrid.workRect.w,
            winCtx->geoGrid.workRect.h
        };
        
        GeoGrid_AddSplit(&winCtx->geoGrid, "SampleView", &size)->id = 1;
        
        ThreadLock_Init();
        Interface_Main(&winCtx->app);
        
        ThreadLock_Free();
        glfwTerminate();
        
        return 0;
    }
#endif
    
    Main_Config(argv);
    if ((argID = ArgStr(argv, "log"))) callSignal = true;
    if ((argID = ArgStr(argv, "S"))) printf_SetSuppressLevel(PSL_NO_WARNING);
    if ((argID = ArgStr(argv, "i"))) input = String_GetSpacedArg(argv, argID);
    if ((argID = ArgStr(argv, "o"))) output = String_GetSpacedArg(argv, argID);
    
    if (argc == 2 /* DragNDrop */) {
        static char outbuf[256 * 2];
        
        printf_SetSuppressLevel(PSL_NO_WARNING);
        
        if (StrEndCase(argv[1], ".wav") || StrEndCase(argv[1], ".aiff") || StrEndCase(argv[1], ".mp3")) {
            char* format[] = {
                ".bin",
                ".wav",
                ".aiff",
                ".c"
            };
            
            String_SwapExtension(outbuf, argv[1], format[sDefaultFormat]);
            Log("Input: %s", argv[1]);
            Log("Output: %s", argv[1]);
            
            input = qFree(StrDup(argv[1]));
            output = outbuf;
        }
    }
    
    if (input == NULL) {
        printf_toolinfo(
            sToolName,
            sToolUsage
        );
        
        return 1;
    }
    
    printf_toolinfo(sToolName, "\n");
    Audio_InitSample(&sample, input, output);
    
    if (ArgStr(argv, "config-override")) gOverrideConfig = true;
    
    if ((argID = ArgStr(argv, "design"))) {
        Main_LoadSampleConf(argv[argID]);
    } else if ((argID = ArgStr(argv, "book")) && sample.useExistingPred == 0) {
        AudioTools_LoadCodeBook(&sample, argv[argID]);
        sample.useExistingPred = true;
    } else {
        if ((argID = ArgStr(argv, "table-iter"))) gTableDesignIteration = qFree(StrDup(argv[argID]));
        if ((argID = ArgStr(argv, "table-frame"))) gTableDesignFrameSize = qFree(StrDup(argv[argID]));
        if ((argID = ArgStr(argv, "table-bits"))) gTableDesignBits = qFree(StrDup(argv[argID]));
        if ((argID = ArgStr(argv, "table-order"))) gTableDesignOrder = qFree(StrDup(argv[argID]));
        if ((argID = ArgStr(argv, "table-threshold"))) gTableDesignThreshold = qFree(StrDup(argv[argID]));
    }
    
    if ((argID = ArgStr(argv, "srate"))) gBinSampleRate = Value_Int(argv[argID]);
    if ((argID = ArgStr(argv, "tuning"))) gTuning = Value_Float(argv[argID]);
    
    if ((argID = ArgStr(argv, "raw-channel"))) gRaw.channelNum = Value_Int(argv[argID]);
    if ((argID = ArgStr(argv, "raw-samplerate"))) gRaw.sampleRate = Value_Int(argv[argID]);
    if ((argID = ArgStr(argv, "raw-bit"))) {
        gRaw.bit = Value_Int(argv[argID]);
        if (StrStrCase(argv[argID], "f"))
            gRaw.dataIsFloat = true;
    }
    
    Audio_LoadSample(&sample);
    
    if ((argID = ArgStr(argv, "basenote"))) sample.instrument.note = Value_Int(argv[argID]);
    if ((argID = ArgStr(argv, "finetune"))) sample.instrument.fineTune = Value_Int(argv[argID]);
    
    if ((argID = ArgStr(argv, "split-hi"))) sample.instrument.highNote = Value_Int(argv[argID]);
    if ((argID = ArgStr(argv, "split-lo"))) sample.instrument.lowNote = Value_Int(argv[argID]);
    if ((argID = ArgStr(argv, "half-precision"))) gPrecisionFlag = 3;
    
    if (output == NULL) printf_error("No output specified!");
    if ((argID = ArgStr(argv, "b"))) {
        if (StrStr(argv[argID], "32"))
            sample.targetBit = 32;
        if (StrStr(argv[argID], "16"))
            sample.targetBit = 16;
        
        sample.targetIsFloat = false;
        if (sample.targetBit) {
            if (StrStr(argv[argID], "f"))
                sample.targetIsFloat = true;
        }
    }
    if ((argID = ArgStr(argv, "N"))) {
        printf_info_align("BitDepth", "%10d", sample.bit);
        printf_info_align("Sample Rate", "%10d", sample.sampleRate);
        printf_info_align("Channels", "%10d", sample.channelNum);
        printf_info_align("Frames", "%10d", sample.samplesNum);
        printf_info_align("Data Size", "%10d", sample.size);
        printf_info_align("File Size", "%10d", sample.memFile.size);
        printf_info_align("Loop Start", "%10d", sample.instrument.loop.start);
        printf_info_align("Loop End", "%10d", sample.instrument.loop.end);
        
        if (output == NULL)
            return 0;
    }
    
    Audio_BitDepth(&sample);
    if ((argID = ArgStr(argv, "m"))) Audio_Mono(&sample);
    if ((argID = ArgStr(argv, "n"))) Audio_Normalize(&sample);
    
    Audio_SaveSample(&sample);
    
    Audio_FreeSample(&sample);
    
    if (callSignal) Log_Print();
    Log_Free();
    
    return 0;
}
