//
// Created by Kriko on 2021/2/6.
//

#ifndef OPENGLEXAMPLE_KISYSTEM_H
#define OPENGLEXAMPLE_KISYSTEM_H

#include <stdio.h>
#include <tchar.h>
#include <memory.h>
#include <assert.h>
#include <sys/stat.h>
#include <minwindef.h>
#include <winnt.h>
#pragma warning(disable:4251) //去除模板导出编译的警告
#pragma warning(disable:4595)
#pragma warning(disable:4091)
#ifdef KISYSTEM_EXPORTS
#define KISYSTEM_API __declspec(dllexport)
#else
#define KISYSTEM_API __declspec(dllimport)
#endif

namespace KIEngine {
#if WIN32
#define SYSTEM_BIT_WIDTH 32
#elif WIN64
#define SYSTEM_BIT_WIDTH 64
#endif
#define THREAD_CALLBACK __stdcall
#if WIN32 || WIN64
#define WINDOWS
#endif

#define LOG_BUFFER_SIZE 65536
        class KISYSTEM_API KISystem {
        public:
            static TCHAR ms_sLogBuffer[LOG_BUFFER_SIZE];
            static DWORD ms_dwMainThreadID;
            FORCEINLINE bool
            KIMemcpy( void* pDest, const void* pSrc, unsigned int uiCountSize, unsigned int uiDestBufferSize = 0 ) {
                if ( !uiDestBufferSize ) {
                    uiDestBufferSize = uiCountSize;
                }
#ifdef WINDOWS
                return (unsigned int) (memcpy_s(pDest, uiDestBufferSize, pSrc, uiCountSize));
#else
                return false;
#endif
            }

            FORCEINLINE unsigned int
            KIStrlen (const TCHAR* pSrc) {
#ifdef WINDOWS
                return (unsigned int)_tcslen( pSrc );
#else
                return 0;
#endif
            }

            /**
             * 原子性增加
             * @param pRefCount
             */
            FORCEINLINE void
            KILockedIncrement ( long* pRefCount ) {
#ifdef WINDOWS
                _InterlockedIncrement( pRefCount );
#else
                return;
#endif
            }

            /**
             * 原子性减少
             * @param pRefCount
             */
            FORCEINLINE void
            KILockedDecrement ( long* pRefCount ) {
#ifdef WINDOWS
                _InterlockedDecrement( pRefCount );
#else
                return;
#endif
            }

            /**
             * 指定内存内容
             * @param pDest
             * @param uiVal
             * @param uiCount
             */
            FORCEINLINE void
            KIMemset ( void* pDest, int  uiVal, unsigned int uiCount ) {
#ifdef WINDOWS
                memset( pDest, uiVal, uiCount );
#else
                return;
#endif
            }

            /**
             * 把 pSource 所指向的字符串追加到 pDest 所指向的字符串的结尾
             * @param pDest
             * @param uiCount
             * @param pSource
             */
            FORCEINLINE void
            KIStrcat ( TCHAR* pDest, unsigned int uiCount, const TCHAR* pSource ) {
#ifdef WINDOWS
                _tcscat_s( pDest, uiCount, pSource );
#else
                return;
#endif
            }


        };
    }

#endif //OPENGLEXAMPLE_KISYSTEM_H
