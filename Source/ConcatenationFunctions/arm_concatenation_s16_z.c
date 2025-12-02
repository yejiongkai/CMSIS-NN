#include "arm_nnfunctions.h"
#include "arm_nnsupportfunctions.h"

void arm_concatenation_nhwc_s16_c(const int16_t *input,
                                 const uint16_t input_n,
                                 const uint16_t input_h,
                                 const uint16_t input_w,
                                 const uint16_t input_c,
                                 int16_t *output,
                                 const uint16_t output_c,
                                 const uint32_t offset_c)
{
    output += offset_c;

    for (uint32_t n = 0; n < input_n; n++)
    {
        for (uint32_t h = 0; h < input_h; h++) 
        {
            for (uint32_t w = 0; w < input_w; w++)
            {
                arm_memcpy_q15(output, input, input_c * 2);
                output += output_c;
                input += input_c;
            }
        }
    }
}
