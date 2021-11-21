// // Copyright (c) <2017> Intel Corporation

// // Permission is hereby granted, free of charge, to any person obtaining a
// // copy of this software and associated documentation files (the
// //"Software"), to deal in the Software without restriction, including
// // without limitation the rights to use, copy, modify, merge, publish,
// // distribute, sublicense, and/or sell copies of the Software, and to
// // permit persons to whom the Software is furnished to do so, subject to
// // the following conditions:

// // The above copyright notice and this permission notice shall be included
// // in all copies or substantial portions of the Software.

// // THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// // OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// // MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// // IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// // CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// // TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// // SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

// #pragma OPENCL EXTENSION cl_intel_channels : enable

// #define CYCLE 46


// channel float channel_out[16*16];
// channel float channel_blue[16] __attribute__((depth(1)));
// channel float channel_red[16] __attribute__((depth(1)));
// channel float channel_yellow[16][16] __attribute__((depth(1)));
// channel float channel_brown[16][16] __attribute__((depth(1)));

// __attribute__((max_global_work_dim(0))) __kernel void
// reader(__global float *data_A, __global float *data_B, int size,
//        __global float *out) {

//   __local float A_mem[16][16];
//   __local float B_mem[16][16];
//   __local float C_mem[16*16];
//   __local float prefetch_A[16][CYCLE];
//   __local float prefetch_B[CYCLE][16];

//   int i, j;
//   for (i = 0; i < 16; i++) {
//     for (j = 0; j < 16; j++) {
//       A_mem[i][j] = data_A[16 * i + j];
//       B_mem[i][j] = data_B[16 * i + j];
//     }
//   }

//   for (j = 0 ; j < 16; j++) {
// 	for (i = j ; i < j+16; i++) {

// 		prefetch_B[i][j] = B_mem[15+j-i][j] ;
		
// 		}
// 		}


// 	for (i = 0 ; i < 16; i++) {
// 	for (j = i ; j < i+16 ; j++) {
		 
// 		prefetch_A[i][j] = A_mem[i][15+i-j];
		
// 		}
// 		}




// // #pragma unroll
//   for (j = 0; j < CYCLE; j++) {
//     for (i = 0; i < 16; i++) {
//       //    printf("The value of j and i inside blue is %d %d\n",j,i);
//       // printf("in writes:%d %f %f \n",j,prefetch_A[i][j],prefetch_B[j][i]);
//       write_channel_intel(channel_blue[i], prefetch_B[j][i]);
//       write_channel_intel(channel_red[i], prefetch_A[i][j]);
//     }
//   }

//   // while(~flag) {
//   for (i = 0; i < 16*16; i++) {
//     //	if (valid[i] != true) {
//     //	C_mem[i] = read_channel_nb_intel(channel_out[i] , &valid[i]);
//     C_mem[i] = read_channel_intel(channel_out[i]);
//     // 	}

//     // flag = (flag & valid[i]) ;
//   }

//   //}

//   for (i = 0; i < 16*16; i++) {
//     //    printf("\nC_mem[%d] = %f \n",i,C_mem[i]);
//     out[i] = C_mem[i];
//   }

//   /*for( j=0;j<16;j++)
//   {
//   //  mem_fence(CLK_CHANNEL_MEM_FENCE);
//   //	C_mem[j] = read_channel_intel(channel_out);
//     printf("Output[%d]=%f \n",j,C_mem[j]);

//   }*/
//   /*for (i = 0 ; i < 7 ;i ++)
//   {
//           out[i] = C_mem[i] ;
//   }*/
// }

// __attribute__((max_global_work_dim(0))) __attribute__((autorun))
// __attribute__((num_compute_units(16, 16))) __kernel void
// systolic_PE() {

//   //  bool flag[16] ;

//   // while(1)
//   int compute_id_x = get_compute_id(0);
//   int compute_id_y = get_compute_id(1);
//   float input_b = 0;
//   float input_a = 0;
//   float input_c;
//   float count;
//   int index;

//   while (1) {

//     if (count != 0) {
//       if (compute_id_x == 0) {
//         input_b = read_channel_intel(channel_blue[compute_id_y]);
//       } else {
//         input_b =
//             read_channel_intel(channel_brown[compute_id_x - 1][compute_id_y]);
//       }
//            if(compute_id_x== 15 && compute_id_y ==15)
//       {
//         printf("b read happend");
//       }


//       if (compute_id_y == 0) {
//         input_a = read_channel_intel(channel_red[compute_id_x]);
//       }

//       else {
//         input_a =
//             read_channel_intel(channel_yellow[compute_id_x][compute_id_y - 1]);
//       }
//                if(compute_id_x== 15 && compute_id_y ==15)
//       {
//         printf("a read happend");
//       }

//       if(compute_id_x== 15 && compute_id_y ==15)
//       {
//         printf("a: %f , b: %f , count: %f \n",input_a,input_b,count);
//       }

//       input_c = input_c + input_a * input_b;
//       if (compute_id_x < 15) {
//         write_channel_intel(channel_brown[compute_id_x][compute_id_y], input_b);
//         // mem_fence(CLK_CHANNEL_MEM_FENCE);
//       }

//       if (compute_id_y < 15) {

//         write_channel_intel(channel_yellow[compute_id_x][compute_id_y],
//                             input_a);
//         // mem_fence(CLK_CHANNEL_MEM_FENCE);
//       }

//       /* if(count == CYCLE)
//         {
//           printf("inputs for PE[%d][%d]: %f %f
//         \n",input_a,input_b,compute_id_x,compute_id_y); printf("output for
//         PE[%d][%d]: %f \n",compute_id_x,compute_id_y,input_c);
//         }	*/

//       /*  if ((compute_id_x == 3) && (compute_id_y == 3 )) {
//          printf("\n in count =%f, sum = %f ,input_b data %f and input_a data %f
//       for PE[%d][%d] \n",count,input_c,
//       input_b,input_a,compute_id_x,compute_id_y);
//       }*/

//       if (count == CYCLE) {
//         // printf("output : %f \n", input_c);
//         // printf("Count: in op %f , x : %d ,y: %d \n",count,compute_id_x,compute_id_y);
//         // printf("PE[%d][%d], output: %f
//         // \n",compute_id_x,compute_id_y,input_c);
//         index = 16 * compute_id_x + compute_id_y;
//         write_channel_intel(channel_out[index], input_c);
//       }
//       count = count + 1;

//     } else {

//       //   printf("%f ",count);
//       if (compute_id_x < 15) {
//         write_channel_intel(channel_brown[compute_id_x][compute_id_y], 0);
//       }

//       if (compute_id_y < 15) {
//         write_channel_intel(channel_yellow[compute_id_x][compute_id_y], 0);
//       }
//       count = count + 1;
//     }
//   }
// }



// Copyright (c) <2017> Intel Corporation

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the
//"Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#pragma OPENCL EXTENSION cl_intel_channels : enable

#define CYCLE 10
#define PEC 4


channel float channel_out[PEC*PEC];
channel float channel_blue[PEC] __attribute__((depth(1)));
channel float channel_red[PEC] __attribute__((depth(1)));
channel float channel_yellow[PEC][PEC] __attribute__((depth(1)));
channel float channel_brown[PEC][PEC] __attribute__((depth(1)));

__attribute__((max_global_work_dim(0))) __kernel void
reader(__global float *data_A, __global float *data_B, int size,
       __global float *out) {

  __local float A_mem[PEC][PEC];
  __local float B_mem[PEC][PEC];
  __local float C_mem[PEC*PEC];
  __local float prefetch_A[PEC][CYCLE];
  __local float prefetch_B[CYCLE][PEC];

  int i, j;
  for (i = 0; i < PEC; i++) {
    for (j = 0; j < PEC; j++) {
      A_mem[i][j] = data_A[PEC * i + j];
      B_mem[i][j] = data_B[PEC * i + j];
    }
  }

  for (j = 0 ; j < PEC; j++) {
	for (i = j ; i < j+PEC; i++) {

		prefetch_B[i][j] = B_mem[(PEC-1)+j-i][j] ;
		
		}
		}


	for (i = 0 ; i < PEC; i++) {
	for (j = i ; j < i+PEC ; j++) {
		 
		prefetch_A[i][j] = A_mem[i][(PEC-1)+i-j];
		
		}
		}




#pragma unroll
  for (j = 0; j < CYCLE; j++) {
    for (i = 0; i < PEC; i++) {
      //    printf("The value of j and i inside blue is %d %d\n",j,i);
      // printf("in writes:%d %f %f \n",j,prefetch_A[i][j],prefetch_B[j][i]);
      write_channel_intel(channel_blue[i], prefetch_B[j][i]);
      write_channel_intel(channel_red[i], prefetch_A[i][j]);
    }
  }

  // while(~flag) {
  for (i = 0; i < PEC*PEC; i++) {
    //	if (valid[i] != true) {
    //	C_mem[i] = read_channel_nb_intel(channel_out[i] , &valid[i]);
    C_mem[i] = read_channel_intel(channel_out[i]);
    // 	}

    // flag = (flag & valid[i]) ;
  }

  //}

  for (i = 0; i < PEC*PEC; i++) {
    //    printf("\nC_mem[%d] = %f \n",i,C_mem[i]);
    out[i] = C_mem[i];
  }

  /*for( j=0;j<PEC;j++)
  {
  //  mem_fence(CLK_CHANNEL_MEM_FENCE);
  //	C_mem[j] = read_channel_intel(channel_out);
    printf("Output[%d]=%f \n",j,C_mem[j]);

  }*/
  /*for (i = 0 ; i < 7 ;i ++)
  {
          out[i] = C_mem[i] ;
  }*/
}

__attribute__((max_global_work_dim(0))) __attribute__((autorun))
__attribute__((num_compute_units(PEC, PEC))) __kernel void
systolic_PE() {

  //  bool flag[PEC] ;

  // while(1)
  int compute_id_x = get_compute_id(0);
  int compute_id_y = get_compute_id(1);
  float input_b = 0;
  float input_a = 0;
  float input_c;
  float count;
  int index;

  while (1) {
    // printf(" %d %d %f \n",compute_id_x,compute_id_y,count);
    if (count != 0) {
      if (compute_id_x == 0) {
        input_b = read_channel_intel(channel_blue[compute_id_y]);
      } else {
        input_b =
            read_channel_intel(channel_brown[compute_id_x - 1][compute_id_y]);
      }

      if (compute_id_y == 0) {
        input_a = read_channel_intel(channel_red[compute_id_x]);
      }

      else {
        input_a =
            read_channel_intel(channel_yellow[compute_id_x][compute_id_y - 1]);
      }

      input_c = input_c + input_a * input_b;
      if (compute_id_x < PEC-1) {
        write_channel_intel(channel_brown[compute_id_x][compute_id_y], input_b);
        // mem_fence(CLK_CHANNEL_MEM_FENCE);
      }

      if (compute_id_y < PEC-1) {

        write_channel_intel(channel_yellow[compute_id_x][compute_id_y],
                            input_a);
        // mem_fence(CLK_CHANNEL_MEM_FENCE);
      }

      /* if(count == CYCLE)
        {
          printf("inputs for PE[%d][%d]: %f %f
        \n",input_a,input_b,compute_id_x,compute_id_y); printf("output for
        PE[%d][%d]: %f \n",compute_id_x,compute_id_y,input_c);
        }	*/

      /*  if ((compute_id_x == 3) && (compute_id_y == 3 )) {
         printf("\n in count =%f, sum = %f ,input_b data %f and input_a data %f
      for PE[%d][%d] \n",count,input_c,
      input_b,input_a,compute_id_x,compute_id_y);
      }*/

      if (count == CYCLE) {
        // printf("output : %f \n", input_c);
        //printf("Count: in op %f , x : %d ,y: %d \n",count,compute_id_x,compute_id_y);
        // printf("PE[%d][%d], output: %f
        // \n",compute_id_x,compute_id_y,input_c);
        index = PEC * compute_id_x + compute_id_y;
        write_channel_intel(channel_out[index], input_c);
      }
      count = count + 1;

    } else {

      //   printf("%f ",count);
      if (compute_id_x < PEC-1) {
        write_channel_intel(channel_brown[compute_id_x][compute_id_y], 0);
      }

      if (compute_id_y < PEC-1) {
        write_channel_intel(channel_yellow[compute_id_x][compute_id_y], 0);
      }
      count = count + 1;
    }
  }
}
