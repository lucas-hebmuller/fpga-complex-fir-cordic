#include <stdio.h>
#include "fir.h"

const int LENGTH = 25;

int main() {
    int coef_real[KERNEL_SIZE] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25};
    int coef_img[KERNEL_SIZE] = {25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1};

    int input_real[KERNEL_SIZE] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
    //int input_real[KERNEL_SIZE] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

    //int input_img[KERNEL_SIZE] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
    int input_img[KERNEL_SIZE] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

    float output_magnitude[LENGTH]; // output_real
    float output_phase[LENGTH];     // output_img

    fpga417_fir(input_real, input_img, coef_real, coef_img, output_magnitude, output_phase, LENGTH);

    for (int i = 0; i < LENGTH; i++) {
        printf("OUTPUT %d: mag = %.6f      phase = %.6f \n", i, output_magnitude[i], output_phase[i]);
    }

    return 0;
}
