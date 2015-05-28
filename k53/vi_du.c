#include <stdio.h>

main(){
	int A[100];
	int B[100];
	int n,m,i,j,k;
	char d;
	do{
		k=0;
	printf("nhap so luong mang A:");scanf("%d",&n);
	printf("nhap mang A:\n");
	for(i=0;i<n;i++){
		printf("phan tu thu %d: ",i+1);scanf("%d",&A[i]);
	}
	printf("nhap so luong mang B:");scanf("%d",&m);
	printf("nhap mang A:\n");
	for(i=0;i<m;i++){
		printf("phan tu thu %d: ",i+1);scanf("%d",&B[i]);
	}
	printf("check=%d\n",check_mang_nguoc(A,n,B,m));
	printf("Ban co muon tiep tuc ko(y/n): ");scanf("%s",&d);
	d=tolower(d);
	if(d=='n') k=1;
}while(k!=1);
}

int check_mang_nguoc(int A[],int n,int B[],int m){
    int i;
    if(n==m){
        for(i=0;i<n;i++){
            if(A[i]!=B[n-i-1]) return 0;
        }
        return 1;
    }
    else return 0;
}