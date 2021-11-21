//Copyright (c) <2017> Intel Corporation

//Permission is hereby granted, free of charge, to any person obtaining a
//copy of this software and associated documentation files (the
//"Software"), to deal in the Software without restriction, including
//without limitation the rights to use, copy, modify, merge, publish,
//distribute, sublicense, and/or sell copies of the Software, and to
//permit persons to whom the Software is furnished to do so, subject to
//the following conditions:

//The above copyright notice and this permission notice shall be included
//in all copies or substantial portions of the Software.

//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include <stdio.h>
#include <stdlib.h>
#include "CL/opencl.h"
#include <string.h>
#include "AOCLUtils/aocl_utils.h"

const char* kernel_name = "autorun_kernel";
//using namespace std;
using namespace aocl_utils;

cl_platform_id platform = NULL;
cl_device_id device = NULL;
cl_context context = NULL;
//cl_command_queue fpga_to_host_queue = NULL;
cl_command_queue host_to_fpga_queue = NULL;
cl_kernel in_kernel = NULL;
//cl_kernel out_kernel = NULL;
cl_kernel kernel = NULL;

cl_program program = NULL;

cl_int status ;
cl_mem input_buf_a;
cl_mem input_buf_b;
cl_mem output_buf;
bool use_emulator = 0 ;

bool init() 
{

	printf("Initializing OpenCL\n");

	if(!setCwdToExeDir()) {
		printf("init error\n");
		return false;
	}

	cl_uint num_platforms;
	// Get the OpenCL platform.
	if (use_emulator) {
		platform = findPlatform("Intel(R) FPGA Emulation Platform for OpenCL(TM)");
	} else {
		platform = findPlatform("Intel(R) FPGA SDK for OpenCL(TM)");
	}


	/*status = clGetPlatformIDs(1, &platform, &num_platforms);
	  if(platform == NULL) {
	  printf("ERROR: Unable to find Intel(R) FPGA OpenCL platform.\n");
	  return false;
	  }*/

	// Query the available OpenCL devices.
	scoped_array<cl_device_id> devices ;
	cl_uint num_devices;

	devices.reset(getDevices(platform, CL_DEVICE_TYPE_ALL, &num_devices));
	device = devices[0];   //We'll use only one device.
	printf("Platform: %s\n", getPlatformName(platform).c_str());

	/*status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 1, &device, &num_devices);
	  if(status != CL_SUCCESS) {

	  printf("Failed clGetDeviceIDs.\n");
	  return false;
	  }*/

	// Create the context.
	//context = clCreateContext(NULL, 1, &device, NULL, NULL, &status);
	//if(status != CL_SUCCESS) printf("Failed to create context");
	context = clCreateContext(NULL, 1,&device, &oclContextCallback, NULL, &status);
	checkError(status, "Failed to create context");

	// Create the program for all device. Use the first device as the
	// representative device (assuming all device are of the same type).
	// std::string binary_file = getBoardBinaryFile("matrix_mult", device[0]);
	std::string binary_file = getBoardBinaryFile("autorun_kernel", device);
	printf("Using AOCX: %s\n", binary_file.c_str());
	program = createProgramFromBinary(context, binary_file.c_str(),&device,1);

	// Build the program that was just created.
	status = clBuildProgram(program, 0, NULL, "", NULL, NULL);
	checkError(status, "Failed to build program");




	// Create the command queue.
	host_to_fpga_queue = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &status);
	if(status != CL_SUCCESS) printf("Failed to create command queue 1\n");

//	fpga_to_host_queue = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &status);
//	if(status != CL_SUCCESS) printf("Failed to create command queue for crc\n");

	/*	std::string binary_file = getBoardBinaryFile("autorun_kernel", device);
		static const char* autorun_kernel_name = "autorun_kernel.aocx";
		printf("\n Using AOCX: %s\n\n", autorun_kernel_name);
		program = createProgramFromBinary(context, autorun_kernel_name, &device, 1);
		if(status != CL_SUCCESS) {
		printf("Failed clCreateProgramWithBinary.\n");
		return false;
		}
	// Build the program that was just created.
	status = clBuildProgram(program, 0, NULL, "", NULL, NULL);
	if(status != CL_SUCCESS) printf("Failed to build program\n"); */ 

	const char *kernel_name1 = "reader";  // Kernel name, as defined in the CL file
	in_kernel = clCreateKernel(program, kernel_name1, &status);
	if(status != CL_SUCCESS) printf("Failed to create kernel 1\n");
//	const char *kernel_name2 = "writer";  
//	out_kernel = clCreateKernel(program, kernel_name2, &status);
//	if(status != CL_SUCCESS) printf("Failed to create kernel 2\n");
	return true;
}

// Free the resources allocated during initialization
void cleanup() 
{	
	//free kernel/queue/program/context
	if(kernel)
		clReleaseKernel(kernel);
	if(in_kernel)
		clReleaseKernel(in_kernel);
//	if(out_kernel)
//		clReleaseKernel(out_kernel);
	if(host_to_fpga_queue)
		clReleaseCommandQueue(host_to_fpga_queue);
//	if(fpga_to_host_queue)
//		clReleaseCommandQueue(fpga_to_host_queue);	
	if(program)
		clReleaseProgram(program);
	if(context)
		clReleaseContext(context);	

	//free in/out buffers
	if(input_buf_a)
		clReleaseMemObject(input_buf_a);

	if(input_buf_b)
		clReleaseMemObject(input_buf_b);

	if(output_buf)
		clReleaseMemObject(output_buf);
}

int main() 
{
	int insize = 16;
	int outsize = 16;
	 float *input_A = ( float *)malloc(insize*sizeof(float)); 
	 float *input_B = ( float *)malloc(insize*sizeof(float)); 
	 float *output = ( float *)malloc(outsize*sizeof(float)); 

	if (!input_A) printf("error in allocating inputA\n");
	if (!input_B) printf("error in allocating inputB\n");

	if (!output) printf("error in allocating output\n");


	printf(" Input data: \n");
	for (int i =0; i < insize; ++i)
	{
		input_A[i] = i;
		input_B[i] = i;
		printf("%f, ", input_A[i]);
		printf("%f, ", input_B[i]);
	}
	printf("\n kernel function: Each Input_data will be added 1 \n");

	if (!init()) return false;

	input_buf_a = clCreateBuffer(context, CL_MEM_READ_ONLY, insize * sizeof(float), NULL, &status);
	if(status != CL_SUCCESS) printf( "Failed to create input buffer\n");	


	input_buf_b = clCreateBuffer(context, CL_MEM_READ_ONLY, insize * sizeof(float), NULL, &status);
	if(status != CL_SUCCESS) printf( "Failed to create input buffer\n");	



	output_buf = clCreateBuffer(context, CL_MEM_WRITE_ONLY, outsize * sizeof(float), NULL, &status);
	if(status != CL_SUCCESS) printf( "Failed to create output_buf\n");

	cl_event write_event[2];
	cl_event finish_event;
	status = clEnqueueWriteBuffer(host_to_fpga_queue, input_buf_a, CL_FALSE, 0, insize * sizeof(float), input_A, 0, NULL, &write_event[0]);
	// status = clEnqueueWriteBuffer(host_to_fpga_queue, input_buf_a, CL_FALSE, 0, insize * sizeof(float), input_A, 0, NULL, NULL);
	if(status!=CL_SUCCESS) printf("Failed to transfer input A\n");

	status = clEnqueueWriteBuffer(host_to_fpga_queue, input_buf_b, CL_FALSE, 0, insize * sizeof(float), input_B, 0, NULL, &write_event[1]);

	// status = clEnqueueWriteBuffer(host_to_fpga_queue, input_buf_b, CL_FALSE, 0, insize * sizeof(float), input_B, 0, NULL, NULL);
	if(status!=CL_SUCCESS) printf("Failed to transfer input B\n");



	clWaitForEvents(0, &finish_event);
	printf("\n\n write event status = %d \n ",status );

	unsigned argi = 0;
	cl_event in_kernel_event;
//	cl_event out_kernel_event;

	status = clSetKernelArg(in_kernel, argi++, sizeof(cl_mem), &input_buf_a);
	if(status!=CL_SUCCESS) printf("Failed to set argument %d on kernel 1\n", argi - 1);

	status = clSetKernelArg(in_kernel, argi++, sizeof(cl_mem), &input_buf_b);
	if(status!=CL_SUCCESS) printf("Failed to set argument %d on kernel 1\n", argi - 1);

	status = clSetKernelArg(in_kernel, argi++, sizeof(cl_int), (void *) &insize);
	if(status!=CL_SUCCESS) printf("Failed to set argument %d on kernel 1\n", argi - 1);	

	status = clSetKernelArg(in_kernel, argi++, sizeof(cl_mem), &output_buf);
	if(status!=CL_SUCCESS) printf("Failed to set argument %d on kernel 1\n", argi - 1);	

	printf("\n after arg set for in_kernel \n");
	argi = 0;


/*	status = clSetKernelArg(out_kernel, argi++, sizeof(cl_mem), &output_buf);
	if(status!=CL_SUCCESS) printf("Failed to set argument %d on kernel 1\n", argi - 1);	
	status = clSetKernelArg(out_kernel, argi++, sizeof(cl_int), (void *) &outsize);
	if(status!=CL_SUCCESS) printf("Failed to set argument %d on kernel 1\n", argi - 1);	
	printf("\n after arg set for out_kernel \n");*/


	status = clEnqueueTask(host_to_fpga_queue, in_kernel, 2,write_event, &in_kernel_event);
	if(status!=CL_SUCCESS) printf("Failed to launch kernel\n");

//	status = clEnqueueTask(fpga_to_host_queue, out_kernel, 0, NULL, &out_kernel_event);
	
//	clWaitForEvents(1, &out_kernel_event);
	
//	if(status!=CL_SUCCESS) printf("Failed to launch kernel\n");
//	printf("\n after kernels queued! \n\n");
	status = clEnqueueReadBuffer(host_to_fpga_queue, output_buf, CL_FALSE, 0, outsize*sizeof(float), output, 0,NULL, &finish_event);
//	status = clEnqueueReadBuffer(fpga_to_host_queue, output_buf, CL_FALSE, 0, outsize*sizeof(float), output, 1, &out_kernel_event, &finish_event);



	printf("\n\n Read event status status = %d \n ",status );

//	if (status != CL_SUCCESS) printf("read error\n");
	//printf("\n status %d: CL_SUCCESS \n", status);

	clWaitForEvents(1, &finish_event);
	clWaitForEvents(1, &in_kernel_event);
//	printf("\n after wait \n");
	printf("Output data: \n");
	for (int i = 0; i< outsize; ++i)
		printf("%f, ", output[i]);

	// Self test to check kernel autorun is launch correctly 
/*	unsigned test_result = 0;
	for (int i = 0; i< outsize; ++i)
	{	
		if((input[i]+1) != output[i])
			test_result = 1;	 // as output incorrect, mark as wrong !
	}
	if (test_result == 0) 
		printf(" \n\n autorun kernel is launched successfully ! \n\n");
	else
		printf(" \n\n  autorun  kernel is launched failed ! \n\n");		
*/

//	clReleaseEvent(read_event);
	clReleaseEvent(in_kernel_event);
//	clReleaseEvent(out_kernel_event);
//	clReleaseEvent(read_event);
	clReleaseEvent(finish_event);
	cleanup();
	return 0;
}


