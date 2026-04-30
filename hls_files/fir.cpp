#include "fir.h"
#include "cordic.h"

void fpga417_fir(int* input_real, int* input_img, int* kernel_real, int* kernel_img, float* output_magnitude, 
    float* output_phase, int input_length) 
{
#pragma HLS INTERFACE mode=s_axilite port=return bundle=BUS_A
#pragma HLS INTERFACE mode=m_axi port=input_real        offset=slave bundle=gmem0
#pragma HLS INTERFACE mode=m_axi port=input_img         offset=slave bundle=gmem1
#pragma HLS INTERFACE mode=m_axi port=kernel_real       offset=slave bundle=gmem2
#pragma HLS INTERFACE mode=m_axi port=kernel_img        offset=slave bundle=gmem3
#pragma HLS INTERFACE mode=m_axi port=output_magnitude  offset=slave bundle=gmem4
#pragma HLS INTERFACE mode=m_axi port=output_phase      offset=slave bundle=gmem5
#pragma HLS INTERFACE mode=s_axilite port=input_length

#pragma HLS DATAFLOW

    hls::stream<int> real_stream;
    hls::stream<int> img_stream;

    top_fir(input_real, input_img, kernel_real, kernel_img, real_stream, img_stream, input_length);
    top_cordic_rotater(real_stream, img_stream, output_magnitude, output_phase, input_length);
}

void top_fir(int* input_real, int* input_img, int kernel_real[KERNEL_SIZE], int kernel_img[KERNEL_SIZE],
     hls::stream<int>&output_real, hls::stream<int>&output_img, int length) 
{    
    LOOP_FIR_MAIN: for (int i = 0; i < length; i++) {
#pragma HLS PIPELINE II=1
#pragma HLS LOOP_TRIPCOUNT min=25

        int temp_result_real;
        int temp_result_img;

        fir(input_real[i], input_img[i], kernel_real, kernel_img,
            &temp_result_real, &temp_result_img);

        output_real.write(temp_result_real);
        output_img.write(temp_result_img);
    }
}

void fir(int input_real, int input_img, int kernel_real[KERNEL_SIZE], int kernel_img[KERNEL_SIZE], 
    int* output_real, int* output_img) 
{
    static int shift_reg_real[KERNEL_SIZE] = {};
    static int shift_reg_img[KERNEL_SIZE] = {};

#pragma HLS ARRAY_PARTITION variable=shift_reg_real type=complete
#pragma HLS ARRAY_PARTITION variable=shift_reg_img  type=complete
#pragma HLS ARRAY_PARTITION variable=kernel_real    type=complete
#pragma HLS ARRAY_PARTITION variable=kernel_img     type=complete

    SHIFT_LOOP: for (int i = KERNEL_SIZE - 1; i > 0; i--) {
#pragma HLS UNROLL

        shift_reg_real[i] = shift_reg_real[i - 1];
        shift_reg_img[i] = shift_reg_img[i - 1];
    }

    shift_reg_real[0] = input_real;
    shift_reg_img[0] = input_img;

    int acc_real = 0;
    int acc_img = 0;

    // (A+ai) (B+bi) = (AB – ab) + (Ab + aB)i
    // A = shift_reg_real, a = shift_reg_img, B = kernel_real, b = kernel_img
    ACC_LOOP: for (int i = 0; i < KERNEL_SIZE; i++) {
#pragma HLS UNROLL

        acc_real += shift_reg_real[i] * kernel_real[i]
                    - shift_reg_img[i] * kernel_img[i];
                    
        acc_img += shift_reg_real[i] * kernel_img[i]
                    + shift_reg_img[i] * kernel_real[i];
    }

    *output_real = acc_real;
    *output_img = acc_img;
}

void top_cordic_rotater(hls::stream<int>& input_real, hls::stream<int>& input_img, float* output_magnitude, 
    float* output_phase, int length)
{
    LOOP_CORDIC_MAIN: for (int i = 0; i < length; i++) {
#pragma HLS PIPELINE II=1
#pragma HLS LOOP_TRIPCOUNT min=25

        // read from FIFO
        int temp_result_real = input_real.read();
        int temp_result_img = input_img.read();

        // convert current readed img and real to FIXED_POINT TYPE
        FIXED_POINT real_fixed = (FIXED_POINT)temp_result_real;
        FIXED_POINT img_fixed = (FIXED_POINT)temp_result_img;

        // magnitude and degree/phase to be found
        FIXED_POINT magnitude_fixed;
        FIXED_POINT phase_fixed;

        cordic_rotator(real_fixed, img_fixed, &magnitude_fixed, &phase_fixed);

        //convert phase and magnitude from FIXED_POINT back to float
        float magnitude_float = magnitude_fixed.to_float();
        float phase_float = phase_fixed.to_float();

        //write back to top interface float* phase float* magnitude
        output_magnitude[i] = magnitude_float;
        output_phase[i] = phase_float;
    }
}