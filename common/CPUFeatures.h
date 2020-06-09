#ifndef CPUFEATURES_H
#define CPUFEATURES_H


#include "Global.h"

#if SSR_USE_X86_ASM

class CPUFeatures {

private:
    static bool s_mmx;
    static bool s_sse, s_sse2, s_sse3, s_ssse3, s_sse41, s_sse42;
    static bool s_avx, s_avx2;
    static bool s_bmi1, s_bmi2;

public:
    static void Detect();

    inline static bool HasMMX() { return s_mmx; }
    inline static bool HasSSE() { return s_sse; }
    inline static bool HasSSE2() { return s_sse2; }
    inline static bool HasSSE3() { return s_sse3; }
    inline static bool HasSSSE3() { return s_ssse3; }
    inline static bool HasSSE41() { return s_sse41; }
    inline static bool HasSSE42() { return s_sse42; }
    inline static bool HasAVX() { return s_avx; }
    inline static bool HasAVX2() { return s_avx2; }
    inline static bool HasBMI1() { return s_bmi1; }
    inline static bool HasBMI2() { return s_bmi2; }

};

#endif // SSR_USE_X86_ASM
#endif // CPUFEATURES_H