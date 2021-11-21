#include <stdio.h>
#include <stdlib.h>

int main()
{
	float A[4][4] ;
	float p[10][4] ;
	float pa[4][10] ;
	
	int i , j ;

	for (i = 0 ; i < 4; i++) {
	for (j = 0 ; j < 4; j++) {
		A[i][j] = 4*i + j ;
	}
	}

	for (i = 0 ; i < 10; i++) {
	for (j = 0 ; j < 4; j++) {
			p[i][j] = 0 ;		
		}
		}

	for (i = 0 ; i < 4; i++) {
	for (j = 0 ; j < 10 ; j++) {
			pa[i][j] = 0 ;		
		}
		}

	for (i = 0 ; i < 7; i++) {
	for (j = 0 ; j < 4; j++) {
		
		if(j-i < 1) 
		p[i][j] = A[3-i+j][j] ;
		
		}
		}


	for (i = 0 ; i < 4; i++) {
	for (j = 0 ; j < 7; j++) {
		
		if(i-j < 1) 
		pa[i][j] = A[i][3+i-j];
		
		}
		}
		




	printf("\n Printing B\n");	
	for (i = 0 ; i < 20; i++) {
	for (j = 0 ; j < 4; j++) {
		printf(" %f ",p[i][j]) ;
				}
				printf("\n"); 
				}
				

	printf("\n Printing A\n");	

	for (i = 0 ; i < 4; i++) {
	for (j = 0 ; j < 20; j++) {
		printf(" %f ,",pa[i][j]) ;
				}
				printf("\n"); 
				}
				

}











