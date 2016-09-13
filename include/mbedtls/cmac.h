/**
 * \file cmac.h
 *
 * \brief The CMAC Mode for Authentication
 *
 *  Copyright (C) 2006-2016, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */
#ifndef MBEDTLS_CMAC_H
#define MBEDTLS_CMAC_H

#include "cipher.h"

#define MBEDTLS_ERR_CMAC_BAD_INPUT      -0x0011 /**< Bad input parameters to function. */
#define MBEDTLS_ERR_CMAC_VERIFY_FAILED  -0x0013 /**< Verification failed. */
#define MBEDTLS_ERR_CMAC_ALLOC_FAILED   -0x0015 /**< Failed to allocate memory */


#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief          CMAC context structure
 */
typedef struct {
    mbedtls_cipher_context_t cipher_ctx;    /*!< cipher context used */
    unsigned char* K1; /*!< CMAC Subkey 1 */
    unsigned char* K2; /*!< CMAC Subkey 2 */
}
mbedtls_cmac_context;

/**
 * \brief           Initialize CMAC context (just makes references valid)
 *                  Makes the context ready for mbedtls_cmac_setkey() or
 *                  mbedtls_cmac_free().
 *
 * \param ctx       CMAC context to initialize
 */
void mbedtls_cmac_init( mbedtls_cmac_context *ctx );

/**
 * \brief           Initialize the CMAC context
 *
 * \param ctx       CMAC context to be initialized
 * \param cipher    cipher to use.  
                    Cipher block size must be 8 bytes or 16 bytes.
 * \param key       encryption key
 * \param keybits   encryption key size in bits (must be acceptable by the cipher)
 *
 * \return          0 if successful, or a cipher specific error code
 */
int mbedtls_cmac_setkey( mbedtls_cmac_context *ctx,
                         mbedtls_cipher_id_t cipher,
                         const unsigned char *key,
                         unsigned int keybits );

/**
 * \brief           Free a CMAC context and underlying cipher sub-context
 *                  Securely wipes sub keys and other sensitive data.
 *
 * \param ctx       CMAC context to free
 */
void mbedtls_cmac_free( mbedtls_cmac_context *ctx );

/**
 * \brief           Generate a CMAC tag.
 *
 * \param ctx       CMAC context
 * \param input     buffer holding the input data
 * \param in_len    length of the input data in bytes
 * \param tag       buffer for holding the generated tag
 * \param tag_len   length of the tag to generate in bytes
 *                  Must be 2, 4, 6, 8 if cipher block size is 8
 *                  Must be 2, 4, 6, 8, 10, 12, 14 or 16 if cipher block size is 16
 *
 * \return          0 if successful
 */
int mbedtls_cmac_generate( mbedtls_cmac_context *ctx,
                           const unsigned char *input, size_t in_len,
                           unsigned char *tag, size_t tag_len );

/**
 * \brief           Verify a CMAC tag.
 *
 * \param ctx       CMAC context
 * \param input     buffer holding the input data
 * \param in_len    length of the input data in bytes
 * \param tag       buffer holding the tag to verify
 * \param tag_len   length of the tag to verify in bytes
 *                  Must be 2, 4, 6, 8 if cipher block size is 8
 *                  Must be 2, 4, 6, 8, 10, 12, 14 or 16 if cipher block size is 16
 * \return          0 if successful and authenticated
 *                  MBEDTLS_ERR_CMAC_VERIFY_FAILED if tag does not match
 */
int mbedtls_cmac_verify( mbedtls_cmac_context *ctx,
                         const unsigned char *input, size_t in_len,
                         const unsigned char *tag, size_t tag_len );

#ifdef MBEDTLS_AES_C
/**
 * \brief           AES-CMAC-128-PRF
 *                  See RFC 4615 for details
 *
 * \param key       PRF key
 * \param key_len   PRF key length
 * \param input     buffer holding the input data
 * \param in_len    length of the input data in bytes
 * \param tag       buffer holding the generated pseudorandom output
 *
 * \return          0 if successful
 */
int mbedtls_aes_cmac_prf_128( const unsigned char *key, size_t key_len,
                              const unsigned char *input, size_t in_len,
                              unsigned char tag[16] );
#endif /* MBEDTLS_AES_C */

#if defined(MBEDTLS_SELF_TEST) && ( defined(MBEDTLS_AES_C) || defined(MBEDTLS_DES_C) )
/**
 * \brief          Checkup routine
 *
 * \return         0 if successful, or 1 if the test failed
 */
int mbedtls_cmac_self_test( int verbose );
#endif /* MBEDTLS_SELF_TEST && ( MBEDTLS_AES_C || MBEDTLS_DES_C ) */

#ifdef __cplusplus
}
#endif

#endif /* MBEDTLS_CMAC_H */
