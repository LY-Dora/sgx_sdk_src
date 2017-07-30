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


#include "service_provider.h"
#include "sample_libcrypto.h"
#include "ecp.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>
#include <string.h>
#ifdef _MSC_VER
    #include "stdafx.h"
#pragma warning(push)
#pragma warning ( disable:4127 )
#endif
#include "ias_ra.h"

// This whole file is used as simulation of the interfaces to be
// delivered by the IAS.  This sample does not contact the real
// IAS.  The IAS Sevice Provider developer needs to follow the 
// IAS onboarding process to gain access to the  real IAS.
#define UNUSED(expr) do { (void)(expr); } while (0)

#if !defined(SWAP_ENDIAN_DW)
    #define SWAP_ENDIAN_DW(dw)	((((dw) & 0x000000ff) << 24)                \
    | (((dw) & 0x0000ff00) << 8)                                            \
    | (((dw) & 0x00ff0000) >> 8)                                            \
    | (((dw) & 0xff000000) >> 24))
#endif
#if !defined(SWAP_ENDIAN_32B)
    #define SWAP_ENDIAN_32B(ptr)                                            \
{\
    unsigned int temp = 0;                                                  \
    temp = SWAP_ENDIAN_DW(((unsigned int*)(ptr))[0]);                       \
    ((unsigned int*)(ptr))[0] = SWAP_ENDIAN_DW(((unsigned int*)(ptr))[7]);  \
    ((unsigned int*)(ptr))[7] = temp;                                       \
    temp = SWAP_ENDIAN_DW(((unsigned int*)(ptr))[1]);                       \
    ((unsigned int*)(ptr))[1] = SWAP_ENDIAN_DW(((unsigned int*)(ptr))[6]);  \
    ((unsigned int*)(ptr))[6] = temp;                                       \
    temp = SWAP_ENDIAN_DW(((unsigned int*)(ptr))[2]);                       \
    ((unsigned int*)(ptr))[2] = SWAP_ENDIAN_DW(((unsigned int*)(ptr))[5]);  \
    ((unsigned int*)(ptr))[5] = temp;                                       \
    temp = SWAP_ENDIAN_DW(((unsigned int*)(ptr))[3]);                       \
    ((unsigned int*)(ptr))[3] = SWAP_ENDIAN_DW(((unsigned int*)(ptr))[4]);  \
    ((unsigned int*)(ptr))[4] = temp;                                       \
}
#endif

// This is the ECDSA NIST P-256 private key used to sign the attestation 
// report.  The Report Key here is just from demontration.  One of the 
// outputs of the onboarding proccess is the real Report Key. 
static const sample_ec256_private_t g_rk_priv_key =
{{
    0x63,0x2c,0xd4,0x02,0x7a,0xdc,0x56,0xa5,
    0x59,0x6c,0x44,0x3e,0x43,0xca,0x4e,0x0b,
    0x58,0xcd,0x78,0xcb,0x3c,0x7e,0xd5,0xb9,
    0xf2,0x91,0x5b,0x39,0x0d,0xb3,0xb5,0xfb
}};

static sample_spid_t g_sim_spid = {"Service X"};


// Simulates the IAS function for verifying the quote produce by
// the ISV enclave. It doesn't decrypt or verify the quote in
// the simulation.  Just produces the attestaion verification
// report with the platform info blob.
//
// @param p_isv_quote Pointer to the quote generated by the ISV
//                    enclave.
// @param pse_manifest Pointer to the PSE manifest if used.
// @param p_attestation_verification_report Pointer the outputed
//                                          verification report.
//
// @return int

int ias_verify_attestation_evidence(
    sample_quote_t *p_isv_quote,
    uint8_t* pse_manifest,
    ias_att_report_t* p_attestation_verification_report)
{
    int ret = 0;

    //unused parameters
    UNUSED(pse_manifest);

    if((NULL == p_isv_quote) ||
        (NULL == p_attestation_verification_report))
    {
        return -1;
    }
    //Decrypt the Quote signature and verify.

    p_attestation_verification_report->id = 0x12345678;
    p_attestation_verification_report->status = IAS_QUOTE_OK;
    p_attestation_verification_report->revocation_reason =
        IAS_REVOC_REASON_NONE;
    //The PIB is just zeroed out in the sample.
    memset(&p_attestation_verification_report->info_blob, 0, sizeof(ias_platform_info_blob_t));

    p_attestation_verification_report->pse_status = IAS_PSE_OK;

    return(ret);
}


// This sample application does not interface with IAS. Instead it simulates
// the retrieval of the SigRL.
//
// @param gid Group ID for the EPID key.
// @param p_sig_rl_size Pointer to the output value of the full
//                      SIGRL size in bytes. (including the
//                      signature).
// @param p_sig_rl Pointer to the output of the SIGRL.
//
// @return int

int ias_get_sigrl(
    const sample_epid_group_id_t gid,
    uint32_t *p_sig_rl_size,
    uint8_t **p_sig_rl)
{
    int ret = 0;

#ifdef ANDROID
    (void) gid;
#else
    UNREFERENCED_PARAMETER(gid);
#endif

    do {

        if (NULL == p_sig_rl || NULL == p_sig_rl_size) {
            ret = -1;
            break;
        }
        *p_sig_rl_size = 0;
        *p_sig_rl = NULL;
        // we should try to get sig_rl from IAS, but right now we will just
        // skip it until the IAS backend is ready.
        break;
    }while (0);

    return(ret);
}


// Used to simulate the enrollment function of the IAS.  It only
// gives back the SPID right now. In production, the enrollment
// occurs out of context from an attestation attempt and only
// occurs once.
//
//
// @param sp_credentials
// @param p_spid
// @param p_authentication_token
//
// @return int

int ias_enroll(
    int sp_credentials,
    sample_spid_t *p_spid,
    int *p_authentication_token)
{
#ifdef ANDROID
    (void) sp_credentials;
    (void) p_authentication_token;
#else
    UNREFERENCED_PARAMETER(sp_credentials);
    UNREFERENCED_PARAMETER(p_authentication_token);
#endif

    if (NULL != p_spid) {
        memcpy_s(p_spid, sizeof(sample_spid_t), &g_sim_spid,
                 sizeof(sample_spid_t));
    } else {
        return(1);
    }
    return(0);
}

#ifdef _MSC_VER
    #pragma warning(pop)
#endif
