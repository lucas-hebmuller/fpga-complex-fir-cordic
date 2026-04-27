#include <ap_fixed.h>
#include <ap_int.h>
#include <hls_math.h>
#include <hls_stream.h>

const int KERNEL_SIZE = 25;

// define any additional data types such as ap_fixed over here
typedef ap_fixed<32,20> FIXED_POINT;


void fpga417_fir(int* input_real, int* input_img, int* kernel_real, int* kernel_img, float* output_img, 
    float* output_real, int input_length);

void top_fir(int* input_real, int* input_img, int kernel_real[KERNEL_SIZE], int kernel_img[KERNEL_SIZE], 
    hls::stream<int>& output_img, hls::stream<int>& output_real, int length);

void fir(int input_real, int input_img, int kernel_real[KERNEL_SIZE], int kernel_img[KERNEL_SIZE], 
    int* output_real, int* output_img);

void top_cordic_rotater(hls::stream<int>& input_img, hls::stream<int>& input_real, float* output_real, 
    float* output_img, int length);
