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


#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "sgx_trts.h"
#include "sgx_thread.h"
#include "sgx_tseal.h"

#include "Enclave_t.h"

uint32_t g_secret;
sgx_thread_mutex_t g_mutex = SGX_THREAD_MUTEX_INITIALIZER;

static inline void free_allocated_memory(void *pointer)
{
    if(pointer != NULL)
    {
        free(pointer);
        pointer = NULL;
    }
}


int initialize_enclave(struct sealed_buf_t *sealed_buf)
{
    // sealed_buf == NULL indicates it is the first time to initialize the enclave
    if(sealed_buf == NULL)
    {
        sgx_thread_mutex_lock(&g_mutex);
        g_secret = 0;
        sgx_thread_mutex_unlock(&g_mutex);
        return 0;
    }

    // It is not the first time to initialize the enclave
    // Reinitialize the enclave to recover the secret data from the input backup
    // sealed data.

    uint32_t len = sizeof(sgx_sealed_data_t) + sizeof(uint32_t);
    //Check the sealed_buf length and check the outside pointers deeply
    if(sealed_buf->sealed_buf_ptr[MOD2(sealed_buf->index)] == NULL ||
        sealed_buf->sealed_buf_ptr[MOD2(sealed_buf->index + 1)] == NULL ||
        !sgx_is_outside_enclave(sealed_buf->sealed_buf_ptr[MOD2(sealed_buf->index)], len) ||
        !sgx_is_outside_enclave(sealed_buf->sealed_buf_ptr[MOD2(sealed_buf->index + 1)], len))
    {
        print("Incorrect input parameter(s).\n");
        return -1;
    }

    // Retrieve the secret from current backup sealed data
    uint32_t unsealed_data = 0;
    uint32_t unsealed_data_length = sizeof(g_secret);
    uint8_t *plain_text = NULL;
    uint32_t plain_text_length = 0;
    uint8_t *temp_sealed_buf = (uint8_t *)malloc(len);
    if(temp_sealed_buf == NULL)
    {
        print("Out of memory.\n");
        return -1;
    }

    sgx_thread_mutex_lock(&g_mutex);
    memcpy(temp_sealed_buf, sealed_buf->sealed_buf_ptr[MOD2(sealed_buf->index)], len);

    // Unseal current sealed buf
    sgx_status_t ret = sgx_unseal_data((sgx_sealed_data_t *)temp_sealed_buf, 
        plain_text, &plain_text_length, (uint8_t *)&unsealed_data, &unsealed_data_length);
    if(ret == SGX_SUCCESS)
    {
        g_secret = unsealed_data;
        sgx_thread_mutex_unlock(&g_mutex);
        free_allocated_memory(temp_sealed_buf);
        return 0;
    }
    else
    {
        sgx_thread_mutex_unlock(&g_mutex);
        print("Failed to reinitialize the enclave.\n");
        free_allocated_memory(temp_sealed_buf);
        return -1;
    }
}

int increase_and_seal_data(size_t tid, struct sealed_buf_t* sealed_buf)
{
    uint32_t sealed_len = sizeof(sgx_sealed_data_t) + sizeof(g_secret);
    // Check the sealed_buf length and check the outside pointers deeply
    if(sealed_buf->sealed_buf_ptr[MOD2(sealed_buf->index)] == NULL ||
        sealed_buf->sealed_buf_ptr[MOD2(sealed_buf->index + 1)] == NULL ||
        !sgx_is_outside_enclave(sealed_buf->sealed_buf_ptr[MOD2(sealed_buf->index)], sealed_len) ||
        !sgx_is_outside_enclave(sealed_buf->sealed_buf_ptr[MOD2(sealed_buf->index + 1)], sealed_len))
    {
        print("Incorrect input parameter(s).\n");
        return -1;
    }

    char string_buf[BUFSIZ] = {'\0'};
    uint32_t temp_secret = 0;
    uint8_t *plain_text = NULL;
    uint32_t plain_text_length = 0;
    uint8_t *temp_sealed_buf = (uint8_t *)malloc(sealed_len);
    if(temp_sealed_buf == NULL)
    {
        print("Out of memory.\n");
        return -1;
    }
    memset(temp_sealed_buf, 0, sealed_len);

    sgx_thread_mutex_lock(&g_mutex);

    // Increase and seal the secret data
    temp_secret = ++g_secret;
    sgx_status_t ret = sgx_seal_data(plain_text_length, plain_text, 
        sizeof(g_secret), (uint8_t *)&g_secret, sealed_len, 
        (sgx_sealed_data_t *)temp_sealed_buf);
    if(ret != SGX_SUCCESS)
    {
        sgx_thread_mutex_unlock(&g_mutex);
        print("Failed to seal data\n");
        free_allocated_memory(temp_sealed_buf);
        return -1;
    }
    // Backup the sealed data to outside buffer
    memcpy(sealed_buf->sealed_buf_ptr[MOD2(sealed_buf->index + 1)],
        temp_sealed_buf, sealed_len);
    sealed_buf->index++;

    sgx_thread_mutex_unlock(&g_mutex);
    free_allocated_memory(temp_sealed_buf);

    // Ocall to print the unsealed secret data outside.
    // In theory, the secret data(s) SHOULD NOT be transferred outside the 
    // enclave as clear text(s).
    // So please DO NOT print any secret outside. Here printing the secret data
    // to outside is only for demo.
    snprintf(string_buf, BUFSIZ, "Thread %#x>: %d\n", (unsigned int)tid, 
        temp_secret);
    print(string_buf);
    return 0;
}
