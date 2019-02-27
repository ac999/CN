#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>

#ifndef PI
#define PI 3.1415926535897932384626433832795028841971693993751058209749 
#endif

#ifndef c1
#define c1 0.16666666666666666666666666666667
#endif

#ifndef c2
#define c2 0.00833333333333333333333333333333
#endif

#ifndef c3
#define c3 1.98412698412698412698412698412799
#endif

#ifndef c4
#define c4 2.75573192239858906525573192239869
#endif

#ifndef c5
#define c5 2.50521083854417187750521083854429
#endif

#ifndef c6
#define c6 1.60590438368216145993923771701559
#endif

void pause(){
	printf("Press ENTER to continue..\n");
	getchar();
}

long double precision(){
	long double u=1;
	char power=0;
	while (1+u/10!=1){
		u/=10;
		power--;
	}
	return u;
}

bool associative(long double precision){
	return (((1.0+precision)+precision)==(1.0+(precision+precision)));
}

long double pow3(long double x){
	return pow(x,3);
}

long double pow2(long double x){
	return pow(x,2);
}

long double P1(long double x,long double xpow2,long double xpow3){
	return x-xpow3*(c1-c2*xpow2);
}

long double P2(long double x,long double xpow2,long double xpow3){
	return x-xpow3*(c1-xpow2*(c2-c3*xpow2));
}

long double P3(long double x,long double xpow2,long double xpow3){
	return x-xpow3*(c1-xpow2*(c2-xpow2*(c3-c4*xpow2)));
}

long double P4(long double x,long double xpow2, long double xpow3){
	return x-xpow3*(c1-xpow2*(c2-xpow2*(c3-xpow2*(c4-c5*xpow2))));
}

long double P5(long double x,long double xpow2, long double xpow3){
	return x-xpow3*(c1-xpow2*(c2-xpow2*(c3-xpow2*(c4-xpow2*(c5-c6*xpow2)))));
}

int main(){
	//Ex. 1:
	printf("Ex. 1:\n");
	long double prec=precision();
	printf("Precision=%0.63Lf\n",prec);
	printf("1+u=%0.63Lf\n",1+prec);
	printf("1+u/10=%0.63Lf\n",1+prec/10);
	pause();
	//Ex. 2:
	printf("\nEx. 2:\n");
	printf("(x+y)+z==x+(y+z)\t%s\n\n",associative(prec)?"True":"False");
	srand(time(NULL));
	while(1){
		long double x,y,z;
		x=rand()*prec;
		y=rand()*prec;
		z=rand()*prec;
		bool result=((x*y)*z==x*(y*z));
		if (!result){
			printf("(x*y)*z!=x*(y*z) for:\nx=%0.63Lf\ny=%0.63Lf\nz=%0.63Lf\n",x,y,z);
			break;
		}
	}
	pause();
	
	printf("\nEx. 3:\n");

	long double x=0;
	long double actual_sin=0;
	long double P_err[5];

	for (int i=0;i<5;i++){
		P_err[i]=0;
	}

	long double xpow2=0;
	long double xpow3=0;
	unsigned int no=100000;
	clock_t t1,t2;
	float p1_t,p2_t,p3_t,p4_t,p5_t;
	p1_t=p2_t=p3_t=p4_t=p5_t=0.0000;

	for (int i=0;i<no;++i){
		long double y=rand();
		x=fmod(y,PI)-PI/2;
		//x=fmod(rand(),PI)-PI/2;
		
		actual_sin=sin(x);
		xpow2=pow2(x);
		xpow3=pow3(x);
		t1=clock();
		P_err[0]+=abs(actual_sin-P1(x,xpow2,xpow3));
		t2=clock();
		p1_t+=((float)(t2-t1)/1000000.0F) * 1000;
		t1=clock();
		P_err[1]+=abs(actual_sin-P2(x,xpow2,xpow3));
		t2=clock();
		p2_t+=((float)(t2-t1)/1000000.0F) * 1000;
		t1=clock();
		P_err[2]+=abs(actual_sin-P3(x,xpow2,xpow3));
		t2=clock();
		p3_t+=((float)(t2-t1)/1000000.0F) * 1000;
		t1=clock();
		P_err[3]+=abs(actual_sin-P4(x,xpow2,xpow3));
		t2=clock();
		p4_t+=((float)(t2-t1)/1000000.0F) * 1000;
		t1=clock();
		P_err[4]+=abs(actual_sin-P5(x,xpow2,xpow3));
		t2=clock();
		p5_t+=((float)(t2-t1)/1000000.0F) * 1000;
			}
	for (int i=0;i<5;++i){
		P_err[i]/=no;
	}

	long double min[3];
	min[0]=min[1]=min[2]=1000000;
	for (int i=0;i<5;++i){
		if (P_err[i]<min[0]){
			min[0]=P_err[i];
		}
	}
	for (int i=0;i<5;++i){
		if(P_err[i]<min[1]){
			if (min[0]!=P_err[i]){
				min[1]=P_err[i];
			}
		}
	}
	for (int i=0;i<5;++i){
		if (P_err[i]<min[2]){
			if (min[0]!=P_err[i] && min[1]!=P_err[i]){
				min[2]=P_err[i];
			}
		}
	}

	for (int j=0;j<3;++j){
		for (int i=0;i<5;i++){
			if (min[j]==P_err[i]){
				printf("P%d are cea mai mica eroare: %0.32Lf\n",i+1,min[j]);
	}
		}
	}
	printf("For %d random numbers:\nP1 time: %0.16f ms\nP2 time: %0.16f ms\nP3 time: %0.16f ms\nP4 time: %0.16f ms\nP5 time: %0.16f ms\n",no,p1_t,p2_t,p3_t,p4_t,p5_t);
	return 0;
}
