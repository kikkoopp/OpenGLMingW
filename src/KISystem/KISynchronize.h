//
// Created by TongHua on 2021/2/7.
//

#ifndef OPENGLEXAMPLE_KISYNCHRONIZE_H
#define OPENGLEXAMPLE_KISYNCHRONIZE_H

#include <cmath>
#include <synchapi.h>
#include <handleapi.h>
#include <eventsys.h>
#include "KISystem.h"

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

    /**
     * 信号量,是一种加减操作
     */
    class KISYSTEM_API KISemaphore{
    public:
        KISemaphore( unsigned int uiCount, unsigned int uiMaxCount );
        virtual ~KISemaphore();

        virtual void Enter();
        virtual void Leave( unsigned int uiReleaseCount );
        virtual void* GetHandle() { return Semaphore; }
    protected:
        void* Semaphore;
        unsigned int m_uiMaxCount;
    };

    /**
     * 互斥锁
     */
    class KISYSTEM_API KIMutex : public KISynchronize {
    public:
        KIMutex();
        virtual ~KIMutex();

        virtual void Enter();
        virtual void Leave();
        virtual void* GetHandle() { return m_mutex; }
    protected:
        void* m_mutex;
    };

    /**
     * windows 事件
     */
    class KISYSTEM_API KIEvent : public KISynchronize {
        HANDLE Event;
    public:
        virtual void* GetHandle() { return Event; }
        /**
         * 构造，将event handle置为空
         */
        KIEvent();
        /**
         * 销毁event handle
         */
        virtual ~KIEvent();
        /**
         * 尝试获取event，获取不到就阻塞
         */
        virtual void Lock();
        /**
         * 发送一个脉冲，然后重置事件设置为无信号
         * 只有足够快的线程才能够接受到这个脉冲
         */
        virtual void UnLock();

        /**
         * 创建一个事件
         * @param bIsManualReset 是否为手动重置事件
         * @param InName 事件名称
         * @return 是否成功创建
         */
        virtual bool Create( bool bIsManualReset = FALSE, const TCHAR* InName = NULL );

        /**
         * 触发事件
         */
        virtual void Trigger( void );

        /**
         * 重置事件
         * 如果事件是手动重置，那么需要调用重置
         * 来将事件设置为无信号状态
         */
        virtual void Reset( void );

        /**
         * 触发事件，发送脉冲，然后重置事件
         * 对于手动重置事件，所有的线程都会被释放
         * 对于自动重置事件，只有一个线程会被释放
         */
        virtual void Pulse( void );

        /**
         * 等待事件被触发
         * @param WaitTime
         * @return
         */
        virtual bool Wait( DWORD WaitTime = (DWORD)-1 );

        virtual bool IsTrigger();

    };


}



#endif //OPENGLEXAMPLE_KISYNCHRONIZE_H
