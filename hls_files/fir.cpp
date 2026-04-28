#include "fir.h"
#include "cordic.h"


void fpga417_fir(int* input_real, int* input_img, int* kernel_real, int* kernel_img, float* output_img, 
    float* output_real, int input_length) 
{
#pragma HLS INTERFACE mode=s_axilite port=return bundle=BUS_A
#pragma HLS INTERFACE mode=m_axi port=input_real  offset=slave bundle=gmem0
#pragma HLS INTERFACE mode=m_axi port=input_img   offset=slave bundle=gmem1
#pragma HLS INTERFACE mode=m_axi port=kernel_real offset=slave bundle=gmem2
#pragma HLS INTERFACE mode=m_axi port=kernel_img  offset=slave bundle=gmem3
#pragma HLS INTERFACE mode=m_axi port=output_img  offset=slave bundle=gmem4
#pragma HLS INTERFACE mode=m_axi port=output_real offset=slave bundle=gmem5
#pragma HLS INTERFACE mode=s_axilite port=input_length

#pragma HLS DATAFLOW

    int kernel_img_local[KERNEL_SIZE];
    int kernel_real_local[KERNEL_SIZE];

    // code to initialize kernel
    for (int i = 0; i < KERNEL_SIZE; i++) {
        kernel_img_local[i] = kernel_img[i];
        kernel_real_local[i] = kernel_real[i];
    }

    hls::stream<int> img_stream;
    hls::stream<int> real_stream;

    top_fir(input_real, input_img, kernel_real_local, kernel_img_local, img_stream, real_stream, input_length);
    top_cordic_rotater(img_stream, real_stream, output_real, output_img, input_length);
}


void top_fir(int* input_real, int* input_img, int kernel_real[KERNEL_SIZE], int kernel_img[KERNEL_SIZE],
     hls::stream<int>&output_img, hls::stream<int>&output_real, int length) 
{
#pragma HLS ARRAY_PARTITION variable=kernel_real type=complete
#pragma HLS ARRAY_PARTITION variable=kernel_img  type=complete
    
    LOOP_FIR_MAIN: for (int i = 0; i < length; i++) {
#pragma HLS PIPELINE II=1

        int temp_result_real;
        int temp_result_img;

        fir(input_real[i], input_img[i], kernel_real, kernel_img,
            &temp_result_real, &temp_result_img);

        output_img.write(temp_result_img);
        output_real.write(temp_result_real);
    }
}


void fir(int input_real, int input_img, int kernel_real[KERNEL_SIZE], int kernel_img[KERNEL_SIZE], 
    int* output_real, int* output_img) 
{
    static int shift_reg_real[KERNEL_SIZE] = {};
    static int shift_reg_img[KERNEL_SIZE] = {};

#pragma HLS ARRAY_PARTITION variable=shift_reg_real type=complete
#pragma HLS ARRAY_PARTITION variable=shift_reg_img  type=complete

    int acc_real = 0;
    int acc_img = 0;

    SHIFT_LOOP: for (int i = KERNEL_SIZE - 1; i > 0; i--) {
#pragma HLS UNROLL
        shift_reg_real[i] = shift_reg_real[i - 1];
        shift_reg_img[i] = shift_reg_img[i - 1];
    }

    shift_reg_real[0] = input_real;
    shift_reg_img[0] = input_img;

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

void top_cordic_rotater(hls::stream<int>& input_img, hls::stream<int>& input_real, float* output_real, 
    float* output_img, int length)
{
    LOOP_CORDIC_MAIN: for (int i = 0; i < length; i++) {
        int temp_result_real;
        int temp_result_img;

        input_img.read();
        input_real;

        // convert current readed img and real to FIXED_POINT TYPE

        cordic_rotator(FIXED_POINT theta, FIXED_POINT *sin, FIXED_POINT *cos)

        //convert phase and magnitude from FIXED_POINT back to float;

        //write back to top interface float* phase float* magnitude
        output_img.write(temp_result_img);
        output_real.write(temp_result_real);
    }
}