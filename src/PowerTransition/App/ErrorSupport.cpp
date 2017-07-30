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


#include <iostream>
#include "ErrorSupport.h"


typedef struct _sgx_errlist_t {
    sgx_status_t err;
    const char * msg;
} sgx_errlist_t;

static sgx_errlist_t sgx_errlist[] = {
    {SGX_ERROR_UNEXPECTED,               "Unexpected error occurred."},
    {SGX_ERROR_INVALID_PARAMETER,        "Invalid parameter."},
    {SGX_ERROR_OUT_OF_MEMORY,            "Out of memory."},
    {SGX_ERROR_ENCLAVE_LOST,             "Power transition occurred."},
    {SGX_ERROR_INVALID_ENCLAVE,          "Invalid enclave image."},
    {SGX_ERROR_INVALID_ENCLAVE_ID,       "Invalid enclave identification."},
    {SGX_ERROR_INVALID_SIGNATURE,        "Invalid enclave signature."},
    {SGX_ERROR_OUT_OF_EPC,               "Out of EPC memory."},
    {SGX_ERROR_NO_DEVICE,                "Invalid SGX device."},
    {SGX_ERROR_MEMORY_MAP_CONFLICT,      "Memory map conflicted."},
    {SGX_ERROR_INVALID_METADATA,         "Invalid encalve metadata."},
    {SGX_ERROR_DEVICE_BUSY,              "SGX device is busy."},
    {SGX_ERROR_INVALID_VERSION,          "Enclave metadata version is invalid."},
    {SGX_ERROR_ENCLAVE_FILE_ACCESS,      "Can't open enclave file."},

    {SGX_ERROR_INVALID_FUNCTION,         "Invalid function name."},
    {SGX_ERROR_OUT_OF_TCS,               "Out of TCS."},
    {SGX_ERROR_ENCLAVE_CRASHED,          "The enclave is crashed."},

    {SGX_ERROR_MAC_MISMATCH,             "Report varification error occurred."},
    {SGX_ERROR_INVALID_ATTRIBUTE,        "The enclave is not authorized."},
    {SGX_ERROR_INVALID_CPUSVN,           "Invalid CPUSVN."},
    {SGX_ERROR_INVALID_ISVSVN,           "Invalid ISVSVN."},
    {SGX_ERROR_INVALID_KEYNAME,          "The requested key name is invalid."},

    {SGX_ERROR_SERVICE_UNAVAILABLE,          "AESM service is not responsive."},
    {SGX_ERROR_SERVICE_TIMEOUT,              "Request to AESM is time out."},
    {SGX_ERROR_SERVICE_INVALID_PRIVILEGE,    "Error occurred while getting launch token."},
};


void ret_error_support(sgx_status_t ret)
{
    size_t idx = 0;
    size_t ttl = sizeof sgx_errlist/sizeof sgx_errlist[0];

    for (idx = 0; idx < ttl; idx++) {
        if(ret == sgx_errlist[idx].err) {
            std::cout << "Error: "<< sgx_errlist[idx].msg << std::endl;
            break;
        }
    }    
    if (idx == ttl)
        std::cout << "Error: Unexpected error occurred." <<std::endl;
    return;
}
