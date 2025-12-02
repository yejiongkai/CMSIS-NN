/* ----------------------------------------------------------------------
 * Project:      CMSIS NN Library
 * Title:        arm_elementwise_mean_s8
 * Description:  Elementwise mean
 *
 * $Date:        27 Nov 2025
 * $Revision:    V.2.2.0
 *
 * Target Processor:  Cortex-M CPUs
 *
 * -------------------------------------------------------------------- */

#include "arm_nnfunctions.h"
#include "arm_nnsupportfunctions.h"
#include <stdio.h>


arm_cmsis_nn_status arm_elementwise_mean_s8(const cmsis_nn_context *ctx,
                                            const cmsis_nn_dims *input_dims,
                                            const int8_t *input_data,
                                            int8_t *output,
                                            const int32_t out_mult,
                                            const int32_t out_shift,
                                            const int32_t input_offset,
                                            const int32_t output_offset,
                                            const int32_t out_activation_min,
                                            const int32_t out_activation_max)
{
    if (ctx->buf == NULL) {
        return ARM_CMSIS_NN_ARG_ERROR;
    }
    int16_t *buffer_a = (int16_t *)ctx->buf;
    int32_t batch_cnt = input_dims->n;
    const int32_t input_x = input_dims->w;
    const int32_t input_y = input_dims->h;
    const int32_t input_ch = input_dims->c;
    const int32_t count = input_x * input_y;

    while (batch_cnt)
    {   
        if (input_offset)
        {
            for (int i_ch = 0; i_ch < input_ch; i_ch++) 
            {
                buffer_a[i_ch] = (int16_t)input_offset * count;
            }
        }
        else
        {
            memset(buffer_a, 0, sizeof(int16_t) * input_ch);
        }
        for (int i_input_y = 0; i_input_y < input_y; i_input_y++)
        {
            for (int i_input_x = 0; i_input_x < input_x; i_input_x++)
            {
                int loop = 0;
                for (; loop <= input_ch - 4; loop += 4)
                {
                    buffer_a[loop] += input_data[0];
                    buffer_a[loop + 1] += input_data[1];
                    buffer_a[loop + 2] += input_data[2];
                    buffer_a[loop + 3] += input_data[3];
                    input_data += 4;
                }
                for (; loop < input_ch; loop++)
                {
                    buffer_a[loop] += *(input_data++);
                }
            }
        }
        if (count == 0)
        {
            return ARM_CMSIS_NN_ARG_ERROR;
        }
        for (int i_out_c = 0; i_out_c < input_ch; i_out_c++) {
            buffer_a[i_out_c] = buffer_a[i_out_c] > 0 ? (buffer_a[i_out_c] + count / 2) / count : (buffer_a[i_out_c] - count / 2) / count;
            buffer_a[i_out_c] = (int16_t)arm_nn_requantize((int32_t)buffer_a[i_out_c], out_mult, out_shift);
            buffer_a[i_out_c] += output_offset;
            *(output++) = buffer_a[i_out_c];
        }
        batch_cnt--;
    }
    return (ARM_CMSIS_NN_SUCCESS);
}