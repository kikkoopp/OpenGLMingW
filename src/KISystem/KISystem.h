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
#include <debugapi.h>
#include <sysinfoapi.h>
#include <processthreadsapi.h>
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

            /**
             * 字符串拆分
             * @param strToken 需要拆分的字符串,为NULL则继续拆分上一次的结果
             * @param strDelimit 拆分字符
             * @param pContext 字符串数组
             * @return 每次返回一个token
             */
            FORCEINLINE TCHAR *
            KIStrtok ( TCHAR* strToken, const TCHAR* strDelimit, TCHAR** pContext ) {
#ifdef WINDOWS
                return _tcstok_s( strToken, strDelimit, pContext );
#else
                return NULL;
#endif
            }

            /**
             * 查找字符串中最后一个匹配字符出现的位置
             * @param pString 字符串
             * @param c 匹配的字符
             * @return 最后能匹配的字符位置指针
             */
            FORCEINLINE TCHAR*
            KICsrchr ( const TCHAR* pString, int c ) {
#ifdef WINDOWS
                return _tcsrchr( pString, c );
#else
                return NULL;
#endif
            }

            FORCEINLINE void
            KIOutPutDebugString ( const TCHAR* pString, ... ) {
#ifdef WINDOWS
                char* pArgs;
                // 参考函数入栈结构
                // 由于是一个压栈操作，各个参数的位置都是连续的
                // 这个操作相当于移动指针，指向...的范围
                pArgs = (char*)&pString + sizeof( pString );
                _vstprintf_s( KISystem::ms_sLogBuffer, LOG_BUFFER_SIZE, pString, pArgs );
                OutputDebugString( KISystem::ms_sLogBuffer );
#else
                return;
#endif
            }

            FORCEINLINE void
            KISprintf ( TCHAR * _DstBuf, unsigned int _SizeInBytes,const TCHAR * _Format, ... ) {
#ifdef WINDOWS
                char* pArgs;
                pArgs = (char*)&_Format + sizeof(_Format);
                _vstprintf_s( _DstBuf, _SizeInBytes, _Format, pArgs );
#else
                return;
#endif
            }

            FORCEINLINE void
            KISprintf( TCHAR* _DstBuf, unsigned int _SizeInBytes, const TCHAR* _Format, va_list pArgs ) {
#ifdef WINDOWS
                _vstprintf_s( _DstBuf, _SizeInBytes, _Format, pArgs );
#else
                return;
#endif
            }

            FORCEINLINE void
            KIScanf( TCHAR* Buf, const TCHAR* _Format, va_list pArgs ) {
#ifdef WINDOWS
                _stscanf_s( Buf, _Format, pArgs );
#else
                return;
#endif
            }

            /**
             * 字符串拷贝，将pSource拷贝到pDest中
             * @param pDest 目标字符串
             * @param uiCount 拷贝大小
             * @param pSource 源字符串
             */
            FORCEINLINE void
            KIStrCopy( TCHAR* pDest, unsigned int uiCount, const TCHAR* pSource ) {
#ifdef WINDOWS
                _tcscpy_s( pDest, uiCount, pSource );
#else
                return;
#endif
            }

            /**
             * 判断两个字符串是否相等
             * @param pString1
             * @param pString2
             * @return =0 pString1 == pString2; >0 pString1 > pString2; <0 pString1 < pString2
             */
            FORCEINLINE int
            KIStrCmp( const TCHAR* pString1, const TCHAR* pString2 ) {
#ifdef WINDOWS
                return _tcscmp( pString1, pString2 );
#else
                return 0;
#endif
            }

            /**
             * 将char字符串转换为宽字符串wchar_t
             * @param pDest
             * @param uiSizeInWord
             * @param pSource
             * @param uiSizeInBytes
             */
            FORCEINLINE void
            KIMbsToWcs( wchar_t * pDest, unsigned int uiSizeInWord, const char* pSource, unsigned int uiSizeInBytes ) {
#ifdef WINDOWS
                mbstowcs_s( 0, pDest, uiSizeInWord, pSource, uiSizeInBytes );
#else
                return;
#endif
            }

            /**
             * 将宽字符串转换为多字符编码字符串
             * @param pDest
             * @param uiSizeInBytes
             * @param pSource
             * @param uiSizeInWord
             */
            FORCEINLINE void
            KIWcsToMbs( char* pDest, unsigned int uiSizeInBytes, const wchar_t * pSource, unsigned int uiSizeInWord ) {
#ifdef WINDOWS
                wcstombs_s( 0, pDest,uiSizeInBytes, pSource, uiSizeInWord );
#else
                return;
#endif
            }

            FORCEINLINE unsigned int
            KIGetCpuNum() {
#ifdef WINDOWS
                SYSTEM_INFO systemInfo;
                GetSystemInfo(&systemInfo);
                return systemInfo.dwNumberOfProcessors;
#else
                return 0;
#endif
            }

            FORCEINLINE bool
            KIIsMainThread() {
#ifdef WINDOWS
                return KISystem::ms_dwMainThreadID == GetCurrentThreadId();
#else
                return false;
#endif
            }

            FORCEINLINE void
            KIInitSystem() {
#ifdef WINDOWS
                KISystem::ms_dwMainThreadID = GetCurrentThreadId();
#else
                return;
#endif
            }

#define KIMAC_ASSERT(Expression) \
{                      \
    assert(Expression);\
}
            //求绝对值
            template<typename T>
            FORCEINLINE T ABS(T t)
            {
                // 	if (t < 0)
                // 		return -t;
                return t < 0 ? -t:t;
                /*return t; */
            }

            template<typename T>
            FORCEINLINE T Min(T t0 , T t1)
            {
                return t0 < t1 ? t0 : t1;
            }
            template<typename T>
            FORCEINLINE T Max(T t0 , T t1)
            {
                return t0 > t1 ? t0 : t1;
            }
            template<typename T>
            FORCEINLINE T Clamp(T Value,T Max,T Min)
            {
                if (Value >= Max)
                {
                    return Max;
                }
                if (Value <= Min)
                {
                    return Min;
                }
                return Value;
            }
            template <class T>
            FORCEINLINE void Swap(T &t1, T &t2)
            {
                T temp;
                temp = t1;
                t1 = t2;
                t2 = temp;
            }
#define BIT(i) (1 << i)
        };
    }

#endif //OPENGLEXAMPLE_KISYSTEM_H
