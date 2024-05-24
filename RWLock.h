#ifndef RWLOCK_H
#define RWLOCK_H
#include <stdint.h>

typedef struct {
    volatile long mLockFlag;
} RWLock;

void RWLock_Init(RWLock* lock);
void RWLock_Destroy(RWLock* lock);

void RWLock_EnterWriteLock(RWLock* lock);
void RWLock_LeaveWriteLock(RWLock* lock);

void RWLock_EnterReadLock(RWLock* lock);
void RWLock_LeaveReadLock(RWLock* lock);

#endif // RWLOCK_H

