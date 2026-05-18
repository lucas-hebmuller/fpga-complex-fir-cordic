# FPGA Complex FIR + CORDIC Signal Processor

Hardware-accelerated signal processing pipeline for Xilinx Zynq-7020 FPGA that performs complex FIR filtering followed by CORDIC-based Cartesian-to-Polar conversion.

## 📋 Overview

This project implements a two-stage streaming signal processing accelerator:
1. **25-tap Complex FIR Filter** - Filters complex-valued input signals (I/Q data)
2. **CORDIC Rotator** - Converts filtered output from Cartesian (real, imaginary) to Polar (magnitude, phase) coordinates

The design uses High-Level Synthesis (HLS) to generate optimized RTL and achieves real-time processing through dataflow pipelining.

## 🎯 Key Features

- **Streaming Architecture**: `hls::stream` FIFOs enable concurrent FIR and CORDIC execution
- **Optimized Resource Usage**: Designed to stay within Zynq-7020 DSP constraints (220 DSP slices)
- **High Throughput**: Pipelined processing with II=1 for maximum streaming performance
- **Fixed-Point Precision**: `ap_fixed<32,17>` CORDIC provides efficient computation with minimal accuracy loss
- **Timing Closure**: Designed to meet 100 MHz operation (10 ns period)

## 🚀 Prerequisites

- Xilinx Vitis HLS 2025.1 or compatible
- Xilinx Vivado 2025.1 (for bitstream generation)
- ZedBoard or Zynq-7020 development board (for hardware testing)

## 📚 References

1. **Xilinx/AMD Documentation**
   - AMD Xilinx. (2025). *Vitis High-Level Synthesis User Guide (UG1399)*. Retrieved from https://docs.amd.com/r/en-US/ug1399-vitis-hls/Introduction

2. **CORDIC Algorithm**
   - Meher, P. K., Valls, J., Juang, T.-B., Sridharan, K., & Maharatna, K. (2009). 50 Years of CORDIC: Algorithms, Architectures, and Applications. *IEEE Transactions on Circuits and Systems I: Regular Papers, 56*(9), 1893-1907. https://doi.org/10.1109/TCSI.2009.2025803

3. **Coursework**
   - CMPEN/EE 417: Digital Design Using Field Programmable Devices (3 Credits). The Pennsylvania State University, Department of Computer Science and Engineering. Course covers FPGA architectures, VHDL-based design, behavioral synthesis, and system-level optimization techniques for reconfigurable computing.

## 📝 License

This project was developed as coursework for CMPEN/EE 417 at The Pennsylvania State University.

