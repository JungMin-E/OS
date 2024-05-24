#include "RWLock.h"
#include <sched.h> 

#define LF_WRITE_MASK 0x7FF00000
#define LF_WRITE_FLAG 0x00100000
#define LF_READ_MASK  0x000FFFFF 

void RWLock_Init(RWLock* lock) {
    lock->mLockFlag = 0;
}

void RWLock_Destroy(RWLock* lock) {
    // Clean up resources if necessary
}

void RWLock_EnterWriteLock(RWLock* lock) {
    while (1) {
        while (lock->mLockFlag & LF_WRITE_MASK) {
            sched_yield(); // Yield processor to other threads
        }
        
        if (__sync_add_and_fetch(&lock->mLockFlag, LF_WRITE_FLAG) & LF_WRITE_MASK) {
            while (lock->mLockFlag & LF_READ_MASK) {
                sched_yield(); // Wait for all read locks to be released
            }
            return;
        }
        __sync_add_and_fetch(&lock->mLockFlag, -LF_WRITE_FLAG);
    }
}

void RWLock_LeaveWriteLock(RWLock* lock) {
    __sync_add_and_fetch(&lock->mLockFlag, -LF_WRITE_FLAG);
}

void RWLock_EnterReadLock(RWLock* lock) {
    while (1) {
        while (lock->mLockFlag & LF_WRITE_MASK) {
            sched_yield(); // Wait for write lock to be released
        }

        if ((__sync_add_and_fetch(&lock->mLockFlag, 1) & LF_WRITE_MASK) == 0) {
            return;
        }
        __sync_add_and_fetch(&lock->mLockFlag, -1);
    }
}

void RWLock_LeaveReadLock(RWLock* lock) {
    __sync_add_and_fetch(&lock->mLockFlag, -1);
}

