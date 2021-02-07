//
// Created by TongHua on 2021/2/7.
//

#ifndef OPENGLEXAMPLE_KISYNCHRONIZE_H
#define OPENGLEXAMPLE_KISYNCHRONIZE_H

#include "KISystem.h"
#include "synchapi.h"

namespace KIEngine {
    class KISYSTEM_API KISynchronize {
    public:
        KISynchronize() = default;
        virtual ~KISynchronize() = default;
        virtual void* GetHandle() = 0;
        enum
        {
            WF_OBJECT0 = 0,
            WF_TIMEOUT = 256,
            WF_FAILED = 0xFFFFFFFF
        };
        static unsigned int WaitAll ( KISynchronize** pSynchronize, unsigned int uiNum, bool bWaitAll, DWORD dwMilliseseconds = (DWORD)-1 );
        static void KISafeOutPutDebugString ( const TCHAR* pString, ... );
    };

    /**
     * 互斥量，用户态
     */
    class KISYSTEM_API KICriticalSection {
        /**
         * windows 互斥量
         */
        CRITICAL_SECTION  CriticalSection;

    public:
        /**
         * 创建互斥量
         */
        FORCEINLINE KICriticalSection() {
            InitializeCriticalSection(&CriticalSection);
            // 旋转次数，当线程需要获取互斥量且有其他线程占据，在多核cpu下会循环指定次数，然后决定是否需要进入休眠状态
            SetCriticalSectionSpinCount(&CriticalSection,4000);
        }

        /**
         * 清理互斥量
         */
        FORCEINLINE ~KICriticalSection() {
            DeleteCriticalSection(&CriticalSection);
        }

        /**
         * 锁定互斥量
         */
        FORCEINLINE void
        Lock() {
            // 每次进入前都旋转
            EnterCriticalSection(&CriticalSection);
        }

        /**
         * 解锁互斥量
         */
        FORCEINLINE void
        UnLock() {
            LeaveCriticalSection(&CriticalSection);
        }
    };

}



#endif //OPENGLEXAMPLE_KISYNCHRONIZE_H
