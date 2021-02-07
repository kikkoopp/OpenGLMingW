//
// Created by TongHua on 2021/2/7.
//

#include "KISynchronize.h"
using namespace KIEngine;
KICriticalSection g_Safes;

void KISynchronize::KISafeOutPutDebugString(const TCHAR *pString, ...) {
    g_Safes.Lock();
    char* pArgs;
    pArgs = (char*)&pString + sizeof(pString);
    _vstprintf_s( KISystem::ms_sLogBuffer, LOG_BUFFER_SIZE, pString, pArgs );
    OutputDebugString( KISystem::ms_sLogBuffer );
    g_Safes.UnLock();
}

unsigned int
KISynchronize::WaitAll(KISynchronize **pSynchronize, unsigned int uiNum, bool bWaitAll, DWORD dwMilliseseconds) {
    KIMAC_ASSERT( uiNum >= 1 && uiNum <= MAXIMUM_WAIT_OBJECTS );
    static HANDLE handle[MAXIMUM_WAIT_OBJECTS];
    for ( unsigned int i = 0; i < uiNum; ++i ) {
        handle[i] = (HANDLE)pSynchronize[i]->GetHandle();
    }
    DWORD dw = WaitForMultipleObjects(uiNum, handle, bWaitAll, dwMilliseseconds);
    switch(dw)
    {
        case WAIT_FAILED:
            return WF_FAILED;
        case WAIT_TIMEOUT:
            return WF_TIMEOUT;
        case WAIT_OBJECT_0:
            return WF_OBJECT0;

    }
    return WF_FAILED;
}

KIEvent::KIEvent() {
    Event = NULL;
}

KIEvent::~KIEvent() {
    if ( Event != NULL ) {
        CloseHandle( Event );
    }
}

void KIEvent::Lock() {
    WaitForSingleObject( Event, INFINITE );
}

void KIEvent::UnLock() {
    PulseEvent( Event );
}

bool KIEvent::Create(bool bIsManualReset, const TCHAR *InName) {
    Event = CreateEvent( NULL, bIsManualReset, 0, InName );
    return Event != NULL;
}

void KIEvent::Trigger() {
    SetEvent( Event );
}

void KIEvent::Reset() {
    ResetEvent( Event );
}

void KIEvent::Pulse() {
    PulseEvent( Event );
}

bool KIEvent::Wait(DWORD WaitTime) {
    return WaitForSingleObject( Event, WaitTime ) == WAIT_OBJECT_0;
}

bool KIEvent::IsTrigger() {
    return Wait(0);
}

KIMutex::KIMutex() {
    m_mutex = CreateMutex( NULL, FALSE, NULL );
    KIMAC_ASSERT( m_mutex );
}

KIMutex::~KIMutex() noexcept {
    BOOL closed = CloseHandle( (HANDLE)m_mutex );
    m_mutex = NULL;
    KIMAC_ASSERT( closed );
}

void KIMutex::Enter() {
    DWORD result = WaitForSingleObject( (HANDLE)m_mutex, INFINITE );
    KIMAC_ASSERT( result != WAIT_FAILED );
}

void KIMutex::Leave() {
    BOOL released = ReleaseMutex( (HANDLE)m_mutex );
    KIMAC_ASSERT( released );
}

KISemaphore::KISemaphore(unsigned int uiCount, unsigned int uiMaxCount) {
    KIMAC_ASSERT( uiCount <= uiMaxCount );
    Semaphore = CreateSemaphore( NULL, uiCount, uiMaxCount, NULL );
    m_uiMaxCount = m_uiMaxCount;
    KIMAC_ASSERT( Semaphore );
}

KISemaphore::~KISemaphore() noexcept {
    BOOL closed = CloseHandle( (HANDLE)Semaphore );
    Semaphore = NULL;
    KIMAC_ASSERT( closed );
}

void KISemaphore::Enter() {
    DWORD result = WaitForSingleObject( (HANDLE)Semaphore, INFINITE );
    KIMAC_ASSERT( result );
}

void KISemaphore::Leave(unsigned int uiReleaseCount) {
    BOOL released = ReleaseSemaphore( (HANDLE)Semaphore, uiReleaseCount, NULL );
    KIMAC_ASSERT( released );
}





