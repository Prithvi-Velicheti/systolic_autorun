Systolic Array Implementation using Autorun Kernels <br/>
Requirements:
Install INTEL FPGA SDK FOR OPENCL


 
 Requirements:     <br/>
 Install INTEL FPGA SDK FOR OPENCL   <br/>
 
 To Emulate and Run on CPU   <br/>
 $ ./aoc_emulate.sh    <br/>
 $  make  <br/>
 $  CL_CONTEXT_EMULATOR_DEVICE_INTELFPGA=1 bin/host     \
 
 
 To Run on Hardware  \
 $./aoc_compile.sh   { compilation takes 2 hours approx }   \
 $ aocl program acl0 autorun_kernel.aocx    \
 $ bin/host   <br/>
 



![sys](https://user-images.githubusercontent.com/25413124/142777953-d67640df-16e4-408d-a067-f61ac57c77b3.png)

