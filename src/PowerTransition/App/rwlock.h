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



#ifndef _RWLOCK_H
#define _RWLOCK_H

#if defined(_MSC_VER)
#include <windows.h>

typedef SRWLOCK rwlock_t; 
typedef PSRWLOCK prwlock_t; 

#elif defined(__GNUC__)
#include <pthread.h>
typedef pthread_rwlock_t rwlock_t;
typedef pthread_rwlock_t* prwlock_t;
#endif

#ifdef __cplusplus
extern "C" {
#endif

    void wtlock(prwlock_t lock);
    void rdlock(prwlock_t lock);
    void rdunlock(prwlock_t lock);
    void wtunlock(prwlock_t lock);
    void init_rwlock(prwlock_t lock);
    void fini_rwlock(prwlock_t lock);

#ifdef __cplusplus
}
#endif

#endif
