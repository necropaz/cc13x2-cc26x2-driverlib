/******************************************************************************
*  Filename:       pka.c
*  Revised:        2017-10-16 10:02:50 +0200 (Mon, 16 Oct 2017)
*  Revision:       49981
*
*  Description:    Driver for the PKA module
*
*  Copyright (c) 2015 - 2017, Texas Instruments Incorporated
*  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions are met:
*
*  1) Redistributions of source code must retain the above copyright notice,
*     this list of conditions and the following disclaimer.
*
*  2) Redistributions in binary form must reproduce the above copyright notice,
*     this list of conditions and the following disclaimer in the documentation
*     and/or other materials provided with the distribution.
*
*  3) Neither the name of the ORGANIZATION nor the names of its contributors may
*     be used to endorse or promote products derived from this software without
*     specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
*  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
*  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
*  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
*  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
*  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
*  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
*  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
*  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
*  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************/

#include "pka.h"

//*****************************************************************************
//
// Handle support for DriverLib in ROM:
// This section will undo prototype renaming made in the header file
//
//*****************************************************************************
#if !defined(DOXYGEN)
    #undef  PKAGetOpsStatus
    #define PKAGetOpsStatus                 NOROM_PKAGetOpsStatus
    #undef  PKABigNumModStart
    #define PKABigNumModStart               NOROM_PKABigNumModStart
    #undef  PKABigNumModGetResult
    #define PKABigNumModGetResult           NOROM_PKABigNumModGetResult
    #undef  PKABigNumCmpStart
    #define PKABigNumCmpStart               NOROM_PKABigNumCmpStart
    #undef  PKABigNumCmpGetResult
    #define PKABigNumCmpGetResult           NOROM_PKABigNumCmpGetResult
    #undef  PKABigNumInvModStart
    #define PKABigNumInvModStart            NOROM_PKABigNumInvModStart
    #undef  PKABigNumInvModGetResult
    #define PKABigNumInvModGetResult        NOROM_PKABigNumInvModGetResult
    #undef  PKABigNumMultiplyStart
    #define PKABigNumMultiplyStart          NOROM_PKABigNumMultiplyStart
    #undef  PKABigNumMultGetResult
    #define PKABigNumMultGetResult          NOROM_PKABigNumMultGetResult
    #undef  PKABigNumAddStart
    #define PKABigNumAddStart               NOROM_PKABigNumAddStart
    #undef  PKABigNumAddGetResult
    #define PKABigNumAddGetResult           NOROM_PKABigNumAddGetResult
    #undef  PKABigNumSubStart
    #define PKABigNumSubStart               NOROM_PKABigNumSubStart
    #undef  PKABigNumSubGetResult
    #define PKABigNumSubGetResult           NOROM_PKABigNumSubGetResult
    #undef  PKAEccMultiplyStart
    #define PKAEccMultiplyStart             NOROM_PKAEccMultiplyStart
    #undef  PKAEccMultiplyGetResult
    #define PKAEccMultiplyGetResult         NOROM_PKAEccMultiplyGetResult
    #undef  PKAEccAddStart
    #define PKAEccAddStart                  NOROM_PKAEccAddStart
    #undef  PKAEccAddGetResult
    #define PKAEccAddGetResult              NOROM_PKAEccAddGetResult
#endif

//*****************************************************************************
//
// Handle support for DriverLib in ROM:
// This section will undo prototype renaming made in the header file
//
//*****************************************************************************
#if !defined(DOXYGEN)
    #undef  PKAGetOpsStatus
    #define PKAGetOpsStatus                 NOROM_PKAGetOpsStatus
    #undef  PKABigNumModStart
    #define PKABigNumModStart               NOROM_PKABigNumModStart
    #undef  PKABigNumModGetResult
    #define PKABigNumModGetResult           NOROM_PKABigNumModGetResult
    #undef  PKABigNumCmpStart
    #define PKABigNumCmpStart               NOROM_PKABigNumCmpStart
    #undef  PKABigNumCmpGetResult
    #define PKABigNumCmpGetResult           NOROM_PKABigNumCmpGetResult
    #undef  PKABigNumInvModStart
    #define PKABigNumInvModStart            NOROM_PKABigNumInvModStart
    #undef  PKABigNumInvModGetResult
    #define PKABigNumInvModGetResult        NOROM_PKABigNumInvModGetResult
    #undef  PKABigNumMultiplyStart
    #define PKABigNumMultiplyStart          NOROM_PKABigNumMultiplyStart
    #undef  PKABigNumMultGetResult
    #define PKABigNumMultGetResult          NOROM_PKABigNumMultGetResult
    #undef  PKABigNumAddStart
    #define PKABigNumAddStart               NOROM_PKABigNumAddStart
    #undef  PKABigNumAddGetResult
    #define PKABigNumAddGetResult           NOROM_PKABigNumAddGetResult
    #undef  PKABigNumSubStart
    #define PKABigNumSubStart               NOROM_PKABigNumSubStart
    #undef  PKABigNumSubGetResult
    #define PKABigNumSubGetResult           NOROM_PKABigNumSubGetResult
    #undef  PKAEccMultiplyStart
    #define PKAEccMultiplyStart             NOROM_PKAEccMultiplyStart
    #undef  PKAEccMultiplyGetResult
    #define PKAEccMultiplyGetResult         NOROM_PKAEccMultiplyGetResult
    #undef  PKAEccAddStart
    #define PKAEccAddStart                  NOROM_PKAEccAddStart
    #undef  PKAEccAddGetResult
    #define PKAEccAddGetResult              NOROM_PKAEccAddGetResult
#endif



//*****************************************************************************
//
// Define for the maximum curve size supported by the PKA module in 32 bit
// word.
// \note PKA hardware module can support up to 384 bit curve size due to the
//       2K of PKA RAM.
//
//*****************************************************************************
#define PKA_MAX_CURVE_SIZE_32_BIT_WORD  12

//*****************************************************************************
//
// Define for the maximum length of the big number supported by the PKA module
// in 32 bit word.
//
//*****************************************************************************
#define PKA_MAX_LEN_IN_32_BIT_WORD  PKA_MAX_CURVE_SIZE_32_BIT_WORD

//*****************************************************************************
//
// Used in PKAWritePkaParam() and PKAWritePkaParamExtraOffset() to specify that
// the base address of the parameter should not be written to a NPTR register.
//
//*****************************************************************************
#define PKA_NO_POINTER_REG 0xFF

//*****************************************************************************
//
// Length in bytes of NISTP256 parameters.
//
//*****************************************************************************
#define NISTP256_PARAM_SIZE_BYTES 32


//*****************************************************************************
//
// NIST P256 constants in little endian format. byte[0] is the least
// significant byte and byte[NISTP256_PARAM_SIZE_BYTES - 1] is the most
// significant.
//
//*****************************************************************************
const PKA_EccPoint NISTP256_generator = {
    .x = {.byte = {0x96, 0xc2, 0x98, 0xd8, 0x45, 0x39, 0xa1, 0xf4,
                   0xa0, 0x33, 0xeb, 0x2d, 0x81, 0x7d, 0x03, 0x77,
                   0xf2, 0x40, 0xa4, 0x63, 0xe5, 0xe6, 0xbc, 0xf8,
                   0x47, 0x42, 0x2c, 0xe1, 0xf2, 0xd1, 0x17, 0x6b}},
    .y = {.byte = {0xf5, 0x51, 0xbf, 0x37, 0x68, 0x40, 0xb6, 0xcb,
                   0xce, 0x5e, 0x31, 0x6b, 0x57, 0x33, 0xce, 0x2b,
                   0x16, 0x9e, 0x0f, 0x7c, 0x4a, 0xeb, 0xe7, 0x8e,
                   0x9b, 0x7f, 0x1a, 0xfe, 0xe2, 0x42, 0xe3, 0x4f}},
};

const PKA_EccParam256 NISTP256_prime       = {.byte = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                                    0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
                                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    0x01, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff}};

const PKA_EccParam256 NISTP256_a           = {.byte = {0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                                    0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
                                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    0x01, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff}};

const PKA_EccParam256 NISTP256_b           = {.byte = {0x4b, 0x60, 0xd2, 0x27, 0x3e, 0x3c, 0xce, 0x3b,
                                                    0xf6, 0xb0, 0x53, 0xcc, 0xb0, 0x06, 0x1d, 0x65,
                                                    0xbc, 0x86, 0x98, 0x76, 0x55, 0xbd, 0xeb, 0xb3,
                                                    0xe7, 0x93, 0x3a, 0xaa, 0xd8, 0x35, 0xc6, 0x5a}};

const PKA_EccParam256 NISTP256_order       = {.byte = {0x51, 0x25, 0x63, 0xfc, 0xc2, 0xca, 0xb9, 0xf3,
                                                    0x84, 0x9e, 0x17, 0xa7, 0xad, 0xfa, 0xe6, 0xbc,
                                                    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                                    0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff}};

//*****************************************************************************
//
// Write a PKA parameter to the PKA module, set required registers, and add an offset.
//
//*****************************************************************************
static uint32_t PKAWritePkaParam(const uint8_t *param, uint32_t paramLength, uint32_t paramOffset, uint32_t ptrRegOffset)
{
    uint32_t i;
    uint32_t *paramWordAlias = (uint32_t *)param;
    // Take the floor of paramLength in 32-bit words
    uint32_t paramLengthInWords = paramLength / sizeof(uint32_t);

    // Load the number in PKA RAM
    for (i = 0; i < paramLengthInWords; i++) {
        HWREG(PKA_RAM_BASE + paramOffset + sizeof(uint32_t) * i) = paramWordAlias[i];
    }

    // If the length is not a word-multiple, fill up a temporary word and copy that in
    // to avoid a bus error. The extra zeros at the end should not matter, as the large
    // number is little-endian and thus has no effect.
    // We could have correctly calculated ceiling(paramLength / sizeof(uint32_t)) above.
    // However, we would not have been able to zero-out the extra few most significant
    // bytes of the most significant word. That would have resulted in doing maths operations
    // on whatever follows param in RAM. Not a good situation to be in.
    if (paramLength % sizeof(uint32_t)) {
        uint32_t temp = 0;
        memcpy(&temp, &paramWordAlias[i], paramLength % sizeof(uint32_t));

        HWREG(PKA_RAM_BASE + paramOffset + sizeof(uint32_t) * i) = temp;

        // Increment paramLengthInWords since we take the ceiling of length / sizeof(uint32_t)
        paramLengthInWords++;
    }

    // Update the A, B, C, or D pointer with the offset address of the PKA RAM location
    // where the number will be stored.
    switch (ptrRegOffset) {
        case PKA_O_APTR:
            HWREG(PKA_BASE + PKA_O_APTR) = paramOffset >> 2;
            HWREG(PKA_BASE + PKA_O_ALENGTH) = paramLengthInWords;
            break;
        case PKA_O_BPTR:
            HWREG(PKA_BASE + PKA_O_BPTR) = paramOffset >> 2;
            HWREG(PKA_BASE + PKA_O_BLENGTH) = paramLengthInWords;
            break;
        case PKA_O_CPTR:
            HWREG(PKA_BASE + PKA_O_CPTR) = paramOffset >> 2;
            break;
        case PKA_O_DPTR:
            HWREG(PKA_BASE + PKA_O_DPTR) = paramOffset >> 2;
            break;
    }

    // Ensure 8-byte alignment of next parameter.
    // Returns the offset for the next parameter.
    return paramOffset + sizeof(uint32_t) * (paramLengthInWords + (paramLengthInWords % 2));
}

//*****************************************************************************
//
// Write a PKA parameter to the PKA module but return a larger offset.
//
//*****************************************************************************
static uint32_t PKAWritePkaParamExtraOffset(const uint8_t *param, uint32_t paramLength, uint32_t paramOffset, uint32_t ptrRegOffset)
{
    // Ensure 16-byte alignment.
    return  (sizeof(uint32_t) * 2) + PKAWritePkaParam(param, paramLength, paramOffset, ptrRegOffset);
}

//*****************************************************************************
//
// Writes the result of a large number arithmetic operation to a provided buffer.
//
//*****************************************************************************
static uint32_t PKAGetBigNumResult(uint8_t *resultBuf, uint32_t *resultLength, uint32_t resultPKAMemAddr)
{
    uint32_t regMSWOffset;
    uint32_t lswOffset;
    uint32_t lengthInWords;
    uint32_t i;
    uint32_t *resultWordAlias = (uint32_t *)resultBuf;

    // Check the arguments.
    ASSERT(resultBuf);
    ASSERT((resultPKAMemAddr > PKA_RAM_BASE) &&
           (resultPKAMemAddr < (PKA_RAM_BASE + PKA_RAM_TOT_BYTE_SIZE)));

    // Verify that the operation is complete.
    if (HWREG(PKA_BASE + PKA_O_FUNCTION) & PKA_FUNCTION_RUN) {
        return PKA_STATUS_OPERATION_BUSY;
    }

    // Get the MSW register value.
    regMSWOffset = HWREG(PKA_BASE + PKA_O_MSW);

    // Check to make sure that the result vector is not all zeros.
    if (regMSWOffset & PKA_MSW_RESULT_IS_ZERO_M) {
        return PKA_STATUS_RESULT_0;
    }

    // Get the length of the result
    regMSWOffset = ((regMSWOffset & PKA_MSW_MSW_ADDRESS_M) + 1);
    lswOffset = ((resultPKAMemAddr - PKA_RAM_BASE) >> 2);
    lengthInWords = regMSWOffset - lswOffset;

    // Check if the provided buffer length is adequate to store the result data.
    if (*resultLength < lengthInWords * sizeof(uint32_t)) {
        return PKA_STATUS_BUF_UNDERFLOW;
    }

    // Copy the result into the resultBuf.
    for (i = 0; i < lengthInWords; i++) {
        resultWordAlias[i]= HWREG(resultPKAMemAddr + sizeof(uint32_t) * i);
    }

    // Zero-out the remainder of the result buffer
    memset(resultWordAlias + i, 0x00, *resultLength - lengthInWords * sizeof(uint32_t));

    // Copy the resultant length.
    *resultLength = lengthInWords * sizeof(uint32_t);


    return PKA_STATUS_SUCCESS;
}

static uint32_t PKAGetBigNumResultRemainder(uint8_t *resultBuf, uint32_t *resultLength, uint32_t resultPKAMemAddr)
{
    uint32_t regMSWVal;
    uint32_t lengthInWords;
    uint32_t i;
    uint32_t *resultWordAlias = (uint32_t *)resultBuf;

    // Check the arguments.
    ASSERT(resultBuf);
    ASSERT((resultPKAMemAddr > PKA_RAM_BASE) &&
           (resultPKAMemAddr < (PKA_RAM_BASE + PKA_RAM_TOT_BYTE_SIZE)));

    // Verify that the operation is complete.
    if (HWREG(PKA_BASE + PKA_O_FUNCTION) & PKA_FUNCTION_RUN) {
        return PKA_STATUS_OPERATION_BUSY;
    }

    // Get the MSW register value.
    regMSWVal = HWREG(PKA_BASE + PKA_O_DIVMSW);

    // Check to make sure that the result vector is not all zeros.
    if (regMSWVal & PKA_DIVMSW_RESULT_IS_ZERO_M) {
        return PKA_STATUS_RESULT_0;
    }

    // Get the length of the result
    lengthInWords = ((regMSWVal & PKA_DIVMSW_MSW_ADDRESS_M) + 1) - ((resultPKAMemAddr - PKA_RAM_BASE) >> 2);

    // Check if the provided buffer length is adequate to store the result data.
    if (*resultLength < lengthInWords * sizeof(uint32_t) ) {
        return PKA_STATUS_BUF_UNDERFLOW;
    }

    // Copy the result into the resultBuf.
    for (i = 0; i < lengthInWords; i++) {
        resultWordAlias[i] = HWREG(resultPKAMemAddr + sizeof(uint32_t) * i);
    }

    // Zero-out the remainder of the result buffer
    memset(resultWordAlias + i, 0x00, *resultLength - lengthInWords * sizeof(uint32_t));

    // Copy the resultant length.
    *resultLength = lengthInWords * sizeof(uint32_t);


    return PKA_STATUS_SUCCESS;
}

//*****************************************************************************
//
// Writes the resultant curve point of an ECC operation to the provided buffer.
//
//*****************************************************************************
static uint32_t PKAGetECCResult(uint8_t *curvePointX, uint8_t *curvePointY, uint32_t resultPKAMemAddr, uint32_t length)
{
    uint32_t i = 0;
    uint32_t *xWordAlias = (uint32_t *)curvePointX;
    uint32_t *yWordAlias = (uint32_t *)curvePointY;

    // Check for the arguments.
    ASSERT(curvePoint);
    ASSERT(curvePointX);
    ASSERT(curvePointY);
    ASSERT((resultPKAMemAddr > PKA_RAM_BASE) &&
           (resultPKAMemAddr < (PKA_RAM_BASE + PKA_RAM_TOT_BYTE_SIZE)));

    // Verify that the operation is completed.
    if (HWREG(PKA_BASE + PKA_O_FUNCTION) & PKA_FUNCTION_RUN) {
        return PKA_STATUS_OPERATION_BUSY;
    }

    if (HWREG(PKA_BASE + PKA_O_SHIFT)) {
         return PKA_STATUS_FAILURE;
    }

    // Check to make sure that the result vector is not all zeros.
    if (HWREG(PKA_BASE + PKA_O_MSW) & PKA_MSW_RESULT_IS_ZERO) {
        return PKA_STATUS_RESULT_0;
    }

    // TODO: x[i] no longer indexes by word but by byte -> switch to byte addressed

    if (curvePointX != NULL) {
        // Copy the x co-ordinate value of the result from vector D into
        // the curvePoint.
        for (i = 0; i < (length / sizeof(uint32_t)); i++) {
            xWordAlias[i] = HWREG(resultPKAMemAddr + sizeof(uint32_t) * i);
        }
    }

    resultPKAMemAddr += sizeof(uint32_t) * (2 + length / sizeof(uint32_t) + ((length / sizeof(uint32_t) % 2)));

    if (curvePointY != NULL) {
        // Copy the y co-ordinate value of the result from vector D into
        // the curvePoint.
        for (i = 0; i < (length / sizeof(uint32_t)); i++) {
            yWordAlias[i] = HWREG(resultPKAMemAddr + sizeof(uint32_t) * i);
        }
    }


    return PKA_STATUS_SUCCESS;
}

//*****************************************************************************
//
// Provides the PKA operation status.
//
//*****************************************************************************
uint32_t PKAGetOpsStatus(void)
{
    if (HWREG(PKA_BASE + PKA_O_FUNCTION) & PKA_FUNCTION_RUN_M) {
        return PKA_STATUS_OPERATION_BUSY;
    }
    else {
        return PKA_STATUS_OPERATION_RDY;
    }
}

//*****************************************************************************
//
// Start the big number modulus operation.
//
//*****************************************************************************
uint32_t PKABigNumModStart(const uint8_t *bigNum, uint32_t bigNumLength, const uint8_t *modulus, uint32_t modulusLength, uint32_t *resultPKAMemAddr)
{
    uint32_t offset = 0;

    // Check the arguments.
    ASSERT(bigNum);
    ASSERT(modulus);
    ASSERT(resultPKAMemAddr);

    // Make sure no operation is in progress.
    if (HWREG(PKA_BASE + PKA_O_FUNCTION) & PKA_FUNCTION_RUN) {
        return PKA_STATUS_OPERATION_BUSY;
    }

    offset = PKAWritePkaParam(bigNum, bigNumLength, offset, PKA_O_APTR);

    offset = PKAWritePkaParamExtraOffset(modulus, modulusLength, offset, PKA_O_BPTR);

    // Copy the result vector address location.
    *resultPKAMemAddr = PKA_RAM_BASE + offset;

    // Load C pointer with the result location in PKA RAM
    HWREG(PKA_BASE + PKA_O_CPTR) = offset >> 2;

    // Start the PKCP modulo operation by setting the PKA Function register.
    HWREG(PKA_BASE + PKA_O_FUNCTION) = (PKA_FUNCTION_RUN | PKA_FUNCTION_MODULO);

    return PKA_STATUS_SUCCESS;
}

//*****************************************************************************
//
// Get the result of the big number modulus operation.
//
//*****************************************************************************
uint32_t PKABigNumModGetResult(uint8_t *resultBuf, uint32_t length, uint32_t resultPKAMemAddr)
{
    return PKAGetBigNumResultRemainder(resultBuf, &length, resultPKAMemAddr);
}



//*****************************************************************************
//
// Start the comparison of two big numbers.
//
//*****************************************************************************
uint32_t PKABigNumCmpStart(const uint8_t *bigNum1, const uint8_t *bigNum2, uint32_t length)
{
    uint32_t offset = 0;

    // Check the arguments.
    ASSERT(bigNum1);
    ASSERT(bigNum2);

    // Make sure no operation is in progress.
    if (HWREG(PKA_BASE + PKA_O_FUNCTION) & PKA_FUNCTION_RUN) {
        return PKA_STATUS_OPERATION_BUSY;
    }

    offset = PKAWritePkaParam(bigNum1, length, offset, PKA_O_APTR);

    offset = PKAWritePkaParam(bigNum2, length, offset, PKA_O_BPTR);

    // Set the PKA Function register for the Compare operation
    // and start the operation.
    HWREG(PKA_BASE + PKA_O_FUNCTION) = (PKA_FUNCTION_RUN | PKA_FUNCTION_COMPARE);

    return PKA_STATUS_SUCCESS;
}

//*****************************************************************************
//
// Get the result of the comparison operation of two big numbers.
//
//*****************************************************************************
uint32_t PKABigNumCmpGetResult(void)
{
    uint32_t  status;

    // verify that the operation is complete.
    if (HWREG(PKA_BASE + PKA_O_FUNCTION) & PKA_FUNCTION_RUN) {
        return PKA_STATUS_OPERATION_BUSY;
    }

    // Check the COMPARE register.
    switch(HWREG(PKA_BASE + PKA_O_COMPARE)) {
        case PKA_COMPARE_A_EQUALS_B:
            status = PKA_STATUS_EQUAL;
            break;

        case PKA_COMPARE_A_GREATER_THAN_B:
            status = PKA_STATUS_A_GR_B;
            break;

        case PKA_COMPARE_A_LESS_THAN_B:
            status = PKA_STATUS_A_LT_B;
            break;

        default:
            status = PKA_STATUS_FAILURE;
            break;
    }

    return status;
}

//*****************************************************************************
//
// Start the big number inverse modulo operation.
//
//*****************************************************************************
uint32_t PKABigNumInvModStart(const uint8_t *bigNum, uint32_t bigNumLength, const uint8_t *modulus, uint32_t modulusLength, uint32_t *resultPKAMemAddr)
{
    uint32_t offset = 0;

    // Check the arguments.
    ASSERT(bigNum);
    ASSERT(modulus);
    ASSERT(resultPKAMemAddr);

    // Make sure no operation is in progress.
    if (HWREG(PKA_BASE + PKA_O_FUNCTION) & PKA_FUNCTION_RUN) {
        return PKA_STATUS_OPERATION_BUSY;
    }

    offset = PKAWritePkaParam(bigNum, bigNumLength, offset, PKA_O_APTR);

    offset = PKAWritePkaParam(modulus, modulusLength, offset, PKA_O_BPTR);

    // Copy the result vector address location.
    *resultPKAMemAddr = PKA_RAM_BASE + offset;

    // Load D pointer with the result location in PKA RAM.
    HWREG(PKA_BASE + PKA_O_DPTR) = offset >> 2;

    // set the PKA function to InvMod operation and the start the operation.
    HWREG(PKA_BASE + PKA_O_FUNCTION) = 0x0000F000;

    return PKA_STATUS_SUCCESS;
}

//*****************************************************************************
//
// Get the result of the big number inverse modulo operation.
//
//*****************************************************************************
uint32_t PKABigNumInvModGetResult(uint8_t *resultBuf, uint32_t length, uint32_t resultPKAMemAddr)
{
    return PKAGetBigNumResult(resultBuf, &length, resultPKAMemAddr);
}

//*****************************************************************************
//
// Start the big number multiplication.
//
//*****************************************************************************
uint32_t PKABigNumMultiplyStart(const uint8_t *multiplicand, uint32_t multiplicandLength, const uint8_t *multiplier, uint32_t multiplierLength, uint32_t *resultPKAMemAddr)
{
    uint32_t offset = 0;

    // Check for the arguments.
    ASSERT(multiplicand);
    ASSERT(multiplier);
    ASSERT(resultPKAMemAddr);

    // Make sure no operation is in progress.
    if (HWREG(PKA_BASE + PKA_O_FUNCTION) & PKA_FUNCTION_RUN) {
        return PKA_STATUS_OPERATION_BUSY;
    }

    offset = PKAWritePkaParam(multiplicand, multiplicandLength, offset, PKA_O_APTR);

    offset = PKAWritePkaParam(multiplier, multiplierLength, offset, PKA_O_BPTR);


    // Copy the result vector address location.
    *resultPKAMemAddr = PKA_RAM_BASE + offset;

    // Load C pointer with the result location in PKA RAM.
    HWREG(PKA_BASE + PKA_O_CPTR) = offset >> 2;

    // Set the PKA function to the multiplication and start it.
    HWREG(PKA_BASE + PKA_O_FUNCTION) = (PKA_FUNCTION_RUN | PKA_FUNCTION_MULTIPLY);

    return PKA_STATUS_SUCCESS;
}

//*****************************************************************************
//
// Get the results of the big number multiplication.
//
//*****************************************************************************
uint32_t PKABigNumMultGetResult(uint8_t *resultBuf, uint32_t *resultLength, uint32_t resultPKAMemAddr)
{
     return PKAGetBigNumResult(resultBuf, resultLength, resultPKAMemAddr);
}

//*****************************************************************************
//
// Start the addition of two big number.
//
//*****************************************************************************
uint32_t PKABigNumAddStart(const uint8_t *bigNum1, uint32_t bigNum1Length, const uint8_t *bigNum2, uint32_t bigNum2Length, uint32_t *resultPKAMemAddr)
{
    uint32_t offset = 0;

    // Check for arguments.
    ASSERT(bigNum1);
    ASSERT(bigNum2);
    ASSERT(resultPKAMemAddr);


    // Make sure no operation is in progress.
    if (HWREG(PKA_BASE + PKA_O_FUNCTION) & PKA_FUNCTION_RUN) {
        return PKA_STATUS_OPERATION_BUSY;
    }

    offset = PKAWritePkaParam(bigNum1, bigNum1Length, offset, PKA_O_APTR);

    offset = PKAWritePkaParam(bigNum2, bigNum2Length, offset, PKA_O_BPTR);

    // Copy the result vector address location.
    *resultPKAMemAddr = PKA_RAM_BASE + offset;

    // Load C pointer with the result location in PKA RAM.
    HWREG(PKA_BASE + PKA_O_CPTR) = offset >> 2;

    // Set the function for the add operation and start the operation.
    HWREG(PKA_BASE + PKA_O_FUNCTION) = (PKA_FUNCTION_RUN | PKA_FUNCTION_ADD);

    return PKA_STATUS_SUCCESS;
}

//*****************************************************************************
//
// Get the result of the addition operation on two big number.
//
//*****************************************************************************
uint32_t PKABigNumSubGetResult(uint8_t *resultBuf, uint32_t *resultLength, uint32_t resultPKAMemAddr)
{
    return PKAGetBigNumResult(resultBuf, resultLength, resultPKAMemAddr);
}

//*****************************************************************************
//
// Start the addition of two big number.
//
//*****************************************************************************
uint32_t PKABigNumSubStart(const uint8_t *minuend, uint32_t minuendLength, const uint8_t *subtrahend, uint32_t subtrahendLength, uint32_t *resultPKAMemAddr)
{
    uint32_t offset = 0;

    // Check for arguments.
    ASSERT(minuend);
    ASSERT(subtrahend);
    ASSERT(resultPKAMemAddr);


    // Make sure no operation is in progress.
    if (HWREG(PKA_BASE + PKA_O_FUNCTION) & PKA_FUNCTION_RUN) {
        return PKA_STATUS_OPERATION_BUSY;
    }

    offset = PKAWritePkaParam(minuend, minuendLength, offset, PKA_O_APTR);

    offset = PKAWritePkaParam(subtrahend, subtrahendLength, offset, PKA_O_BPTR);

    // Copy the result vector address location.
    *resultPKAMemAddr = PKA_RAM_BASE + offset;

    // Load C pointer with the result location in PKA RAM.
    HWREG(PKA_BASE + PKA_O_CPTR) = offset >> 2;

    // Set the function for the add operation and start the operation.
    HWREG(PKA_BASE + PKA_O_FUNCTION) = (PKA_FUNCTION_RUN | PKA_FUNCTION_SUBTRACT);

    return PKA_STATUS_SUCCESS;
}

//*****************************************************************************
//
// Get the result of the addition operation on two big number.
//
//*****************************************************************************
uint32_t PKABigNumAddGetResult(uint8_t *resultBuf, uint32_t *resultLength, uint32_t resultPKAMemAddr)
{
    return PKAGetBigNumResult(resultBuf, resultLength, resultPKAMemAddr);
}


//*****************************************************************************
//
// Start ECC Multiplication.
//
//*****************************************************************************
uint32_t PKAEccMultiplyStart(const uint8_t *scalar, const uint8_t *curvePointX, const uint8_t *curvePointY, const uint8_t *prime, const uint8_t *a, const uint8_t *b, uint32_t length, uint32_t *resultPKAMemAddr)
{
    uint32_t offset = 0;

    // Check for the arguments.
    ASSERT(scalar);
    ASSERT(curvePoint);
    ASSERT(curvePointX);
    ASSERT(curvePointY);
    ASSERT(curve);
    ASSERT(length <= PKA_MAX_CURVE_SIZE_32_BIT_WORD * sizeof(uint32_t));
    ASSERT(resultPKAMemAddr);

    // Make sure no PKA operation is in progress.
    if (HWREG(PKA_BASE + PKA_O_FUNCTION) & PKA_FUNCTION_RUN) {
        return PKA_STATUS_OPERATION_BUSY;
    }

    offset = PKAWritePkaParam(scalar, length, offset, PKA_O_APTR);

    offset = PKAWritePkaParamExtraOffset(prime, length, offset, PKA_O_BPTR);
    offset = PKAWritePkaParamExtraOffset(a, length, offset, PKA_NO_POINTER_REG);
    offset = PKAWritePkaParamExtraOffset(b, length, offset, PKA_NO_POINTER_REG);

    offset = PKAWritePkaParamExtraOffset(curvePointX, length, offset, PKA_O_CPTR);
    offset = PKAWritePkaParamExtraOffset(curvePointY, length, offset, PKA_NO_POINTER_REG);

    // Update the result location.
    *resultPKAMemAddr =  PKA_RAM_BASE + offset;

    // Load D pointer with the result location in PKA RAM.
    HWREG(PKA_BASE + PKA_O_DPTR) = offset >> 2;

    // Set the PKA function to ECC-MULT and start the operation.
    HWREG(PKA_BASE + PKA_O_FUNCTION) = PKA_FUNCTION_RUN_M | (0x05 << PKA_FUNCTION_SEQUENCER_OPERATIONS_S);

    return PKA_STATUS_SUCCESS;
}

//*****************************************************************************
//
// Get the result of ECC Multiplication
//
//*****************************************************************************
uint32_t PKAEccMultiplyGetResult(uint8_t *curvePointX, uint8_t *curvePointY, uint32_t resultPKAMemAddr, uint32_t length)
{
    return PKAGetECCResult(curvePointX, curvePointY, resultPKAMemAddr, length);
}

//*****************************************************************************
//
// Start the ECC Addition.
//
//*****************************************************************************
uint32_t PKAEccAddStart(const uint8_t *curvePoint1X, const uint8_t *curvePoint1Y, const uint8_t *curvePoint2X, const uint8_t *curvePoint2Y, const uint8_t *prime, const uint8_t *a, uint32_t length, uint32_t *resultPKAMemAddr)
{
    uint32_t offset = 0;

    // Check for the arguments.
    ASSERT(curvePoint1X);
    ASSERT(curvePoint1Y);
    ASSERT(curvePoint2X);
    ASSERT(curvePoint2Y);
    ASSERT(prime);
    ASSERT(a);
    ASSERT(resultPKAMemAddr);

    // Make sure no operation is in progress.
    if (HWREG(PKA_BASE + PKA_O_FUNCTION) & PKA_FUNCTION_RUN) {
        return PKA_STATUS_OPERATION_BUSY;
    }

    offset = PKAWritePkaParamExtraOffset(curvePoint1X, length, offset, PKA_O_APTR);
    offset = PKAWritePkaParamExtraOffset(curvePoint1Y, length, offset, PKA_NO_POINTER_REG);


    offset = PKAWritePkaParamExtraOffset(prime, length, offset, PKA_O_BPTR);
    offset = PKAWritePkaParamExtraOffset(a, length, offset, PKA_NO_POINTER_REG);

    offset = PKAWritePkaParamExtraOffset(curvePoint2X, length, offset, PKA_O_CPTR);
    offset = PKAWritePkaParamExtraOffset(curvePoint2Y, length, offset, PKA_NO_POINTER_REG);

    // Copy the result vector location.
    *resultPKAMemAddr = PKA_RAM_BASE + offset;

    // Load D pointer with the result location in PKA RAM.
    HWREG(PKA_BASE + PKA_O_DPTR) = offset >> 2;

    // Set the PKA Function to ECC-ADD and start the operation.
    HWREG(PKA_BASE + PKA_O_FUNCTION ) = PKA_FUNCTION_RUN_M | (0x03 << PKA_FUNCTION_SEQUENCER_OPERATIONS_S);

    return PKA_STATUS_SUCCESS;
}

//*****************************************************************************
//
// Get the result of the ECC Addition
//
//*****************************************************************************
uint32_t PKAEccAddGetResult(uint8_t *curvePointX, uint8_t *curvePointY, uint32_t resultPKAMemAddr, uint32_t length)
{
    return PKAGetECCResult(curvePointX, curvePointY, resultPKAMemAddr, length);
}
