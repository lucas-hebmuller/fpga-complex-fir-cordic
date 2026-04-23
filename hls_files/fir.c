#include "fir.h"

void fpga417_fir(int* input_real, int* input_img, int* kernel_real, int* kernel_img, float* output_img, 
    float* output_real, int input_length) {
    // any pragmas to define interface

#pragma HLS DATAFLOW

    int kernel_img[KERNEL_SIZE];
    int kernel_real[KERNEL_SIZE];

    // code to initialize kernel

    hls::stream<int> img_stream;
    hls::stream<int> real_stream;

    top_fir(input_real, input_img, kernel_real, kernel_img, img_stream, real_stream, input_length);
    top_cordic_rotater(img_stream, real_stream, output_real, output_img, input_length);


}

void top_fir(int* input_real, int* input_img, int kernel_real[KERNEL_SIZE], int kernel_img[KERNEL_SIZE],
     hls::stream<int>&output_img, hls::stream<int>&output_real, int length) {

}

void fir(int input_real, input_img, int kernel_real[KERNEL_SIZE], int kernel_img[KERNEL_SIZE], 
    int* output_real, int* output_img) {

}