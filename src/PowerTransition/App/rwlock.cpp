/**
*   Copyright(C) 2011-2016 Intel Corporation All Rights Reserved.
*
*   The source code, information  and  material ("Material") contained herein is
*   owned  by Intel Corporation or its suppliers or licensors, and title to such
*   Material remains  with Intel Corporation  or its suppliers or licensors. The
*   Material  contains proprietary information  of  Intel or  its  suppliers and
*   licensors. The  Material is protected by worldwide copyright laws and treaty
*   provisions. No  part  of  the  Material  may  be  used,  copied, reproduced,
*   modified, published, uploaded, posted, transmitted, distributed or disclosed
*   in any way  without Intel's  prior  express written  permission. No  license
*   under  any patent, copyright  or  other intellectual property rights  in the
*   Material  is  granted  to  or  conferred  upon  you,  either  expressly,  by
*   implication, inducement,  estoppel or  otherwise.  Any  license  under  such
*   intellectual  property  rights must  be express  and  approved  by  Intel in
*   writing.
*
*   *Third Party trademarks are the property of their respective owners.
*
*   Unless otherwise  agreed  by Intel  in writing, you may not remove  or alter
*   this  notice or  any other notice embedded  in Materials by Intel or Intel's
*   suppliers or licensors in any way.
*/


// rwlock.cpp: wrappers of Slim Reader/Writer (SRW) Locks

#include "rwlock.h"

#if defined(_MSC_VER)
void wtlock(prwlock_t lock)
{
    AcquireSRWLockExclusive(lock);
}

void wtunlock(prwlock_t lock)
{
    ReleaseSRWLockExclusive(lock);
}

void rdlock(prwlock_t lock)
{
    AcquireSRWLockShared(lock);
}

void rdunlock(prwlock_t lock)
{
    ReleaseSRWLockShared(lock);
}

void init_rwlock(prwlock_t lock)
{
    InitializeSRWLock(lock);
}

void fini_rwlock(prwlock_t lock)
{
	(void)lock;
    return;
}

#elif defined(__GNUC__)

#include <stdlib.h>
void wtlock(prwlock_t lock)
{
    int ret = pthread_rwlock_wrlock(lock);
    if(0 != ret)
        abort();
}

void wtunlock(prwlock_t lock)
{
    int ret = pthread_rwlock_unlock(lock);
    if(0 != ret)
        abort();
}


void rdlock(prwlock_t lock)
{
    int ret = pthread_rwlock_rdlock(lock);
    if(0 != ret)
        abort();
}

void rdunlock(prwlock_t lock)
{
    int ret = pthread_rwlock_unlock(lock);
    if(0 != ret)
        abort();
}

void init_rwlock(prwlock_t lock)
{
    //use the default attribute.
    int ret = pthread_rwlock_init(lock, NULL);
    if(0 != ret)
        abort();
}

void fini_rwlock(prwlock_t lock)
{
    int ret = pthread_rwlock_destroy(lock);
    if(0 != ret)
        abort();
}

#endif

