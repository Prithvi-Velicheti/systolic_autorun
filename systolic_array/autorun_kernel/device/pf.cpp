#include <bits/stdc++.h>
using namespace std;

int main()
{
	float A[4][4] ;
	float pb[8][4] ;
	float pa[4][8] ;
	
	int i , j ;

	for (i = 0 ; i < 4; i++) {
	for (j = 0 ; j < 4; j++) {
		A[i][j] = 4*i + j ;
	}
	}

	for (i = 0 ; i < 8; i++) {
	for (j = 0 ; j < 4; j++) {
			pb[i][j] = 0 ;		
		}
		}

	for (i = 0 ; i < 4; i++) {
	for (j = 0 ; j < 8; j++) {
			pa[i][j] = 0 ;		
		}
		}

	

	for (j = 0 ; j < 4; j++) {
	for (i = j ; i < j+4; i++) {

		pb[i][j] = A[3+j-i][j] ;
		
		}
		}


	for (i = 0 ; i < 4; i++) {
	for (j = i ; j < i+4 ; j++) {
		 
		pa[i][j] = A[i][3+i-j];
		
		}
		}

		// pa[1][5]=0;
		// pa[1][6]=0;
		// pa[2][6]=0;
		




	printf("\n Printing B\n");	
	for (i = 0 ; i < 8; i++) {
	for (j = 0 ; j < 4; j++) {
		printf(" %f ,",pb[i][j]) ;
				}
				printf("\n"); 
				}
				

	printf("\n Printing A\n");	

	for (i = 0 ; i < 4; i++) {
	for (j = 0 ; j < 8; j++) {
		printf(" %f ,",pa[i][j]) ;
				}
				printf("\n"); 
				}
				

}











