#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define FIXED_BIT 5

//Conversion of QUantized Floating Number to Fixed Point 
int8_t float2fix(float n)
{
    int8_t int_part = 0, frac_part = 0;
    int i;
    float t;
    
    if(n==-1.0){
        int_part = 1<<FIXED_BIT;
        return int_part + frac_part;
    }
    else{
        if (n > 0) {
    int_part = ((int)floor(n)) << FIXED_BIT;
    }
    else {
    //int_part = ((int)ceil(n)) << FIXED_BIT;
    int_part = 1 << FIXED_BIT;
    }
   
    n = fabs(n) - floor(fabs(n));
     
    t = 0.5;
    for (i = 0; i < FIXED_BIT; i++) {
        if ((n - t) >= 0) {
            n -= t;
            frac_part += (1 << (FIXED_BIT - 1 - i));
        }
        t = t /2;
    }

    return int_part + frac_part;
        
    }
    
}

//Conversion of Fixed Point To Floating Number   
float fix2float(int8_t n){
    int8_t int_part=0, frac_part=0;
    int m=0;
    float ans=0;
    int_part = n&0b00100000;
    frac_part = n&0b00011111;
    if(int_part==0b00100000&&frac_part==0b0000000){
        ans=-1;
        return ans;
    }
    else{
        for(int i=FIXED_BIT-1;i>=0;i--){
        if (frac_part>0){
            m = frac_part&0b00000001;
            if(m==1){
                ans+=(1.0/(float)(pow(2,i+1)));
            }
            frac_part=frac_part>>1;
        }
        
    }
    if(int_part==0b00100000){
        ans=ans*(-1);
    }
    return ans;
        
    }
     
}

int main()
{   

    clock_t t,s,r;
    
    srand(time(0));
    
    float input[] = {-1.0  , -0.96875, -0.9375 , -0.875  , -0.84375, -0.8125 ,
       -0.78125, -0.75   , -0.71875, -0.6875 , -0.65625, -0.625  ,
       -0.59375, -0.5625 , -0.53125, -0.5    , -0.46875, -0.4375 ,
       -0.40625, -0.375  , -0.34375, -0.3125 , -0.28125, -0.25   ,
       -0.21875, -0.1875 , -0.15625, -0.125  , -0.09375, -0.0625 ,
       -0.03125,  0.     ,  0.03125,  0.0625 ,  0.09375,  0.125  ,
        0.15625,  0.1875 ,  0.21875,  0.25   ,  0.28125,  0.3125 ,
        0.34375,  0.375  ,  0.40625,  0.4375 ,  0.46875,  0.5    ,
        0.53125,  0.5625 ,  0.59375,  0.625  ,  0.65625,  0.6875 ,
        0.71875,  0.75   ,  0.78125,  0.8125 ,  0.84375,  0.875  ,
        0.9375 ,  0.96875};
    
    
    int m=0,n=0,k=0,M=0,K=0,N=0;    
    printf("Enter the dimensions of the compressed matrices m,n,k....\n");
    scanf("%d%d%d",&m,&n,&k);

    int *A,*B,*C;

    A = (int *)malloc(m*k*sizeof(int));
    B = (int *)malloc(n*k*sizeof(int));
    C = (int *)malloc(m*n*sizeof(int));
    
    printf("Enter the dimensions of the original matrices M,N,K....\n");
    scanf("%d%d%d",&M,&N,&K);

    float *D,*E,*F;

    D = (float *)malloc(M*K*sizeof(float));
    E = (float *)malloc(N*K*sizeof(float));
    F = (float *)malloc(M*N*sizeof(float));
    
    int i,j;
    int num=0;
    float b=0.0,c=0.0;
    //Initializing Matrix A with Compressed Values...
    //Debugging Code.....
   /* printf("Storing weights in compressed format using fixed points in matrix A...\n");
    for(i=0;i<m;i++){
        for(j=0;j<k;j++){
            b = input[rand()%63];
            num = float2fix(b);
            printf("%f ",b);
            *(A+i*k+j)=0b00000000000000000000000000000000;
            *(A+i*k+j)=num;
            b = input[rand()%63];
            num = float2fix(b);
            printf("%f ",b);
            *(A+i*k+j)+=(num<<6);
            b = input[rand()%63];
            num = float2fix(b);
            printf("%f ",b);
            *(A+i*k+j)+=(num<<12);
            b = input[rand()%63];
            num = float2fix(b);
            printf("%f ",b);
            *(A+i*k+j)+=(num<<18);
            b = input[rand()%63];
            num = float2fix(b);
            printf("%f\n",b);
            *(A+i*k+j)+=(num<<24);
        }        
    }
    */

    /*
    printf("\nRetrieving The compressed Values from Matrix A\n"); 
     for(i=0;i<m;i++){
        for(j=0;j<k;j++){
            val = *(A+i*k+j)&(0b00000000000000000000000000111111);
            b = fix2float(val);
            printf("%f ",b);
            val = *(A+i*k+j)&0b00000000000000000000111111000000;
            val = val>>6;
            b = fix2float(val);
            printf("%f ",b);
            val = *(A+i*k+j)&0b00000000000000111111000000000000;
            val = val>>12;
            b = fix2float(val);
            printf("%f ",b);
            val = *(A+i*k+j)&0b00000000111111000000000000000000;
            val = val>>18;
	    b = fix2float(val);
            printf("%f ",b);
            val = *(A+i*k+j)&0b00111111000000000000000000000000;
            val = val>>24;
            b = fix2float(val);
            printf("%f\n",b);
        }     
    }
    */

    printf("Inizialtizing the compressed matrix A...\n");
    for(i=0;i<m;i++){
        for(j=0;j<k;j++){
            *(A+i*k+j)=0;
            num = float2fix(input[rand()%63]);
            *(A+i*k+j)=num;
            num = float2fix(input[rand()%63]);
            *(A+i*k+j)+=(num<<6); 
            num = float2fix(input[rand()%63]);
            *(A+i*k+j)+=(num<<12);
            num = float2fix(input[rand()%63]);
            *(A+i*k+j)+=(num<<18);
            num = float2fix(input[rand()%63]);
            *(A+i*k+j)+=(num<<24);
        }        
    }
    int val=0;
    int count=1,p=0,q=0;
    int a=0;
    printf("\nRetrieving The compressed Values from Matrix A to store in Original size\n");   
    t = clock(); 
    for(i=0;i<M;i++){
	for(j=0;j<K;j++){
          if(count==1){
            val = *(A+p*k+q)&0b00000000000000000000000000111111;
            *(D+i*K+j) = fix2float(val);
            count++; 
	  }
         else if(count==2){
           val = *(A+p*k+q)&0b00000000000000000000111111000000;
           val = val>>6;
           *(D+i*K+j) = fix2float(val);
           count++;
          }
         else if(count==3){
          val = *(A+p*k+q)&0b00000000000000111111000000000000;
          val = val>>12;
          *(D+i*K+j) = fix2float(val);
          count++;
          }
         else if(count==4){
           val = *(A+p*k+q)&0b00000000111111000000000000000000;
           val = val>>18;
	   *(D+i*K+j) = fix2float(val);
           count++;
          }
         else if(count==5){
          val = *(A+p*k+q)&0b00111111000000000000000000000000;
          val = val>>24;
          *(D+i*K+j) = fix2float(val);
          count=1;
          q=q+1;
          }
         if(q==k){
           p=p+1;
           q=0;
          }
         //a++;
         //printf("%d\n",a);
	}	
    } 
    t = clock() - t;
  /* printf("\n");
   for(int i=0;i<M;i++){
      for(int j=0;j<K;j++){
         printf("%f ",*(D+i*K+j));
      }
      printf("\n");
   }
   */
   printf("\nInizialtizing the compressed matrix B...\n");
    for(i=0;i<k;i++){
        for(j=0;j<n;j++){
            *(B+i*n+j)=0;
            num = float2fix(input[rand()%63]);
            *(B+i*n+j)+=num;
            num = float2fix(input[rand()%63]);
            *(B+i*n+j)+=(num<<6);
            num = float2fix(input[rand()%63]);
            *(B+i*n+j)+=(num<<12);
            num = float2fix(input[rand()%63]);
            *(B+i*n+j)+=(num<<18);
            num = float2fix(input[rand()%63]);
            *(B+i*n+j)+=(num<<24);
        }
        
    }
    
    val=0;
    printf("\nRetrieving The compressed Values from Matrix B to store in Original size\n");   
    count=1;
    p=0;
    q=0; 
    s = clock();
    for(i=0;i<K;i++){
	for(j=0;j<N;j++){
          if(count==1){
            val = *(B+p*n+q)&(0b00000000000000000000000000111111);
            *(E+i*N+j) = fix2float(val);
            count++; 
	  }
         else if(count==2){
           val = *(B+p*n+q)&0b00000000000000000000111111000000;
           val = val>>6;
           *(E+i*N+j) = fix2float(val);
           count++;
          }
         else if(count==3){
          val = *(B+p*n+q)&0b00000000000000111111000000000000;
          val = val>>12;
          *(E+i*N+j) = fix2float(val);
          count++;
          }
         else if(count==4){
           val = *(B+p*n+q)&0b00000000111111000000000000000000;
           val = val>>18;
	   *(E+i*N+j) = fix2float(val);
           count++;
          }
         else if(count==5){
          val = *(B+p*n+q)&0b00111111000000000000000000000000;
          val = val>>24;
          *(E+i*N+j) = fix2float(val);
          count=1;
          q=q+1;
          }
         if(q==n){
           p=p+1;
           q=0;
          }
	}	
    }
    s = clock() - s;
    
   double time_taken = ((double)t+s)/CLOCKS_PER_SEC; // in seconds 
  
   printf("\n Time to retrieve data and store in original matrix is %f seconds\n", time_taken); 	
    
   //Multiplication 
   
    r = clock();	
    for(i=0;i<M;i++){
        for(j=0;j<N;j++){
            for(int l=0;l<K;l++){
                *(F+i*N+j)=+(*(D+i*K+l))*(*(E+l*N+j));
            }
        }
    }
    r = clock() - r;
   
   time_taken = ((double)r)/CLOCKS_PER_SEC; // in seconds 
  
   printf("\n Time For Multiplication is %f seconds\n", time_taken); 
   
   double total_time_taken = ((double)t+s+r)/CLOCKS_PER_SEC; // in seconds 
  
   printf("\n Total Time is %f seconds\n", total_time_taken); 	
   	
   
    
   /* printf("\nStoring weights in compressed format using fixed points in matrix B...\n");
    for(i=0;i<k;i++){
        for(j=0;j<n;j++){
            b = input[rand()%63];
            num= float2fix(b);
            printf("%f ",b);
            *(B+i*n+j)=0b00000000000000000000000000000000;
            *(B+i*n+j)+=num;
            b = input[rand()%63];
            num = float2fix(b);
            printf("%f ",b);
            *(B+i*n+j)+=(num<<6);
            b = input[rand()%63];
            num = float2fix(b);
            printf("%f ",b);
            *(B+i*n+j)+=(num<<12);
            b = input[rand()%63];
            num = float2fix(b);
            printf("%f ",b);
            *(B+i*n+j)+=(num<<18);
            b = input[rand()%63];
            num = float2fix(b);
            printf("%f\n",b);
            *(B+i*n+j)+=(num<<24);
        }
        
    }


    printf("\nRetrieving The compressed Values from Matrix B\n"); 
     for(i=0;i<k;i++){
        for(j=0;j<n;j++){
            val = *(B+i*n+j)&0b00000000000000000000000000111111;
            b = fix2float(val);
            printf("%f ",b);
            val = *(B+i*n+j)&0b00000000000000000000111111000000;
            val = val>>6;
            b = fix2float(val);
            printf("%f ",b);
            val = *(B+i*n+j)&0b00000000000000111111000000000000;
            val = val>>12;
            b = fix2float(val);
            printf("%f ",b);
            val = *(B+i*n+j)&0b00000000111111000000000000000000;
            val = val>>18;
	    b = fix2float(val);
            printf("%f ",b);
            val = *(B+i*n+j)&0b00111111000000000000000000000000;
            val = val>>24;
            b = fix2float(val);
            printf("%f\n",b);
        }     
    } 
/*
    /*float a = input[rand()%63];
    //float a = 0.406250;
    printf("%f\n",a);
    int c = float2fix(a);
    printf("%d\n",c);
    clock_t t,s;
    t = clock();
    a = fix2float(c);
    t = clock() - t;
    s+=t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds 
    printf("fun() took %f seconds to execute \n", time_taken); 	
    printf("%f\n",a);*/
    return 0;
}

