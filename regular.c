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
    t = clock();
    for(i=0;i<m;i++){
        for(j=0;j<n;j++){
            for(l=0;l<k;l++){
                *(c+i*n+j)=+*(a+i*k+l)&*(b+l*n+j);
            }
        }
    }
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds 
  
    printf("fun() took %f seconds to execute \n", time_taken); 	
    return 0;
}
