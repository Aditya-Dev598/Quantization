#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main()
{   
    clock_t t;
    int m,n,k,i,j,l;
    printf("Enter m\n");
    scanf("%d",&m);
    printf("Enter n\n");
    scanf("%d",&n);
    printf("Enter k\n");
    scanf("%d",&k);
    printf("Success");
    int *a,*b,*c;
    a = (int *)malloc(m*k*sizeof(int));
    b = (int *)malloc(n*k*sizeof(int));
    c = (int *)malloc(m*n*sizeof(int));
    /*for(i=0;i<m;i++){
        for(j=0;j<k;j++){	
            a[i][j]=0b00000000000000000000000000000000;
	    a[i][j]=a[i][j]|(0b00000000000000000000000000000011&)
        }
    }
    for(i=0;i<k;i++){
        for(j=0;j<n;j++){
            b[i][j]
        }
    }*/
    t = clock();
    for(i=0;i<m;i++){
        for(j=0;j<n;j++){
            for(l=0;l<k;l++){
		int p=0,q=0,r=0;
		//unpacking
		p = *(a+i*k+l)&0b00000000000000001111111111111111;
		q = *(b+l*n+j)&0b00000000000000001111111111111111;
                r = r|(p&q);
		//2
		p = *(a+i*k+l)&0b11111111111111110000000000000000;
		q = *(b+l*n+j)&0b11111111111111110000000000000000;
                r = r|(p&q);
		//packing
                *(c+i*n+j)=+r;
            }
        }
    }
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds 
  
    printf("fun() took %f seconds to execute \n", time_taken); 	
    return 0;
}
