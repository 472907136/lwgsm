/**	
 * \file            gsm_operator.c
 * \brief           Operator API functions
 */
 
/*
 * Copyright (c) 2018 Tilen Majerle
 *  
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software, 
 * and to permit persons to whom the Software is furnished to do so, 
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * This file is part of GSM-AT.
 *
 * Author:          Tilen MAJERLE <tilen@majerle.eu>
 */
#include "gsm/gsm_private.h"
#include "gsm/gsm_operator.h"
#include "gsm/gsm_mem.h"

/**
 * \brief           Get current operator
 * \param[in]       blocking: Status whether command should be blocking or not
 * \return          \ref gsmOK on success, member of \ref gsmr_t enumeration otherwise
 */
gsmr_t
gsm_operator_get(uint32_t blocking) {
    GSM_MSG_VAR_DEFINE(msg);                    /* Define variable for message */

    GSM_MSG_VAR_ALLOC(msg);                     /* Allocate memory for variable */
    GSM_MSG_VAR_REF(msg).cmd_def = GSM_CMD_COPS_GET;

    return gsmi_send_msg_to_producer_mbox(&GSM_MSG_VAR_REF(msg), gsmi_initiate_cmd, blocking, 2000);    /* Send message to producer queue */
}

/**
 * \brief           Set current operator
 * \param[in]       blocking: Status whether command should be blocking or not
 * \return          \ref gsmOK on success, member of \ref gsmr_t enumeration otherwise
 */
gsmr_t
gsm_operator_set(gsm_operator_mode_t mode, gsm_operator_format_t format, const char* name, uint32_t num, uint32_t blocking) {
    GSM_MSG_VAR_DEFINE(msg);                    /* Define variable for message */

    GSM_ASSERT("format valid", format < GSM_OPERATOR_FORMAT_INVALID);   /* Assert input parameters */
    if (mode != GSM_OPERATOR_MODE_AUTO && format != GSM_OPERATOR_FORMAT_NUMBER) {
        GSM_ASSERT("name != NULL", name != NULL);   /* Assert input parameters */
    }

    GSM_MSG_VAR_ALLOC(msg);                     /* Allocate memory for variable */
    GSM_MSG_VAR_REF(msg).cmd_def = GSM_CMD_COPS_SET;

    GSM_MSG_VAR_REF(msg).msg.cops_set.mode = mode;
    GSM_MSG_VAR_REF(msg).msg.cops_set.format = format;
    GSM_MSG_VAR_REF(msg).msg.cops_set.name = name;
    GSM_MSG_VAR_REF(msg).msg.cops_set.num = num;

    return gsmi_send_msg_to_producer_mbox(&GSM_MSG_VAR_REF(msg), gsmi_initiate_cmd, blocking, 2000);    /* Send message to producer queue */
}

/**
 * \brief           Get current operator name
 * \param[in]       ops: Pointer to array to write found operators
 * \param[in]       opsl: Length of input array in units of elements
 * \param[out]      opf: Pointer to ouput variable to save number of operators found
 * \param[in]       blocking: Status whether command should be blocking or not
 * \return          \ref gsmOK on success, member of \ref gsmr_t enumeration otherwise
 */
gsmr_t
gsm_operator_scan(gsm_operator_t* ops, size_t opsl, size_t* opf, uint32_t blocking) {
    GSM_MSG_VAR_DEFINE(msg);                    /* Define variable for message */

    if (opf != NULL) {
        *opf = 0;
    }

    GSM_MSG_VAR_ALLOC(msg);                     /* Allocate memory for variable */
    GSM_MSG_VAR_REF(msg).cmd_def = GSM_CMD_COPS_GET_OPT;
    GSM_MSG_VAR_REF(msg).msg.cops_scan.ops = ops;
    GSM_MSG_VAR_REF(msg).msg.cops_scan.opsl = opsl;
    GSM_MSG_VAR_REF(msg).msg.cops_scan.opf = opf;

    return gsmi_send_msg_to_producer_mbox(&GSM_MSG_VAR_REF(msg), gsmi_initiate_cmd, blocking, 120000);  /* Send message to producer queue */
}