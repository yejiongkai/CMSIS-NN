#include "arm_nnfunctions.h"
#include "arm_nnsupportfunctions.h"


inline void arm_hardswish_s16(const int16_t *data,  
                             const int32_t input_offset,
                             const int32_t thresh,
                             const int32_t left_shift,
                             int16_t *output,
                             const int32_t out_offset,
                             const int32_t out1_mult,
                             const int32_t out1_shift,
                             const int32_t out2_mult,
                             const int32_t out2_shift,
                             const int32_t out_activation_min,
                             const int32_t out_activation_max,
                             uint32_t size)
{
    int64_t input;
    for (uint32_t i = 0; i < size; i++)
    {
        input = (int64_t)(*data++) + input_offset;
        input <<= left_shift;
        if (input <= -thresh)
        {
            input = out_offset;
        }
        else if (input < thresh)
        {
            input = arm_nn_requantize_s64(input * (input + thresh) >> (2 * left_shift), out1_mult, out1_shift);
            input += out_offset;
        }
        else
        {
            input = arm_nn_requantize((int32_t)(input >> left_shift), out2_mult, out2_shift);
            input += out_offset;
        }
        input = MAX(out_activation_min, input);
        input = MIN(out_activation_max, input);
        *output++ = (int16_t)(input);
    }
}