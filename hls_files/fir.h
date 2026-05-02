#ifndef FIR_H
#define FIR_H

#include <ap_fixed.h>
#include <hls_stream.h>

const int KERNEL_SIZE = 25;

typedef ap_fixed<32,17> FIXED_POINT; // 32 - NUM_ITERATIONS = 17

void fpga417_fir(int* input_real, int* input_img, int* coef_real, int* coef_img, float* output_magnitude, 
    float* output_phase, int input_length);

void top_fir(int* input_real, int* input_img, int kernel_real[KERNEL_SIZE], int kernel_img[KERNEL_SIZE], 
    hls::stream<int>& output_real, hls::stream<int>& output_img, int length);

void fir(int input_real, int input_img, int kernel_real[KERNEL_SIZE], int kernel_img[KERNEL_SIZE], 
    int shift_reg_real[KERNEL_SIZE], int shift_reg_img[KERNEL_SIZE], int* output_real, int* output_img);

void top_cordic_rotater(hls::stream<int>& input_real, hls::stream<int>& input_img, float* output_magnitude, 
    float* output_phase, int length);

#endif
