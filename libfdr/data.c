#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jrb.h"
#include "fields.h"
#include "dllist.h"
#define max 100           //so luong dinh toi da co ket noi voi mot dinh
#define max1 30          //so luong duong di lon nhat giua hai dinh

typedef JRB Graph;

typedef struct{
   int A[max1];
   int num;
}mang1;

typedef struct{
   mang1 B[max];   //tinh tu B[1] (ko tinh B[0])
   int total;   
}mang2;

//********************************************************************************************
int themdinh(Graph g,int v);
int themcanh(Graph g,int v1,int v2);//them mot canh tu v1 den v2
int ketnoi(Graph g,int v1,int v2);//kiem tra xem co ket noi tu v1 den v2 ko
int in(Graph g,int v,int output[]);//tat ca cac dinh di den v
int out(Graph g,int v,int output[]);//tat ca cac dinh ma dinh v di den
int DAG(Graph g);//kiem tra xem do thi g co chu trinh hay ko dung DFS
void hienthi(Graph g);
void xoatatca(Graph g);
int xoaketnoi(Graph g,int v1,int v2);//tra ve 1 neu ket noi da xoa tra ve 0 neu ko co ket noi
void xoanutrong(Graph g);
void xoamotdinh(Graph g,int v);
int topology(Graph g,int ra[]);
int BFS(Graph g,int start,int stpop,int ra[]);//ham duyet theo chieu rong dung queue,gia tri tra ve la so luong node co trong duong di
int DFS(Graph g,int start,int stop,int ra[]);//duyet theo chieu sau dung stack,gia tri tra ve la so luong node co trong duong di
int BFS1(Graph g,int start,int stop,int ra[]);//duong di ngan nhat giua 2 dinh,gia tri tra ve la so luong node co trong duong di
void docfile(Graph g,FILE *f);
mang2 *duongdi(Graph g,int start,int stop);
//*************************************************************************************************

main(){
	Graph g;
	JRB node,node2,tree;
	FILE *f;
	int c,v1,v2,n,i,j;
	int *out;
	mang2 *b;
	char d;
	g=make_jrb();
	char filename[30];
	printf("nhap ten file:");
	fgets(filename,30,stdin);
    filename[strlen(filename)-1]='\0';
    f=fopen(filename, "r");
    if(f==NULL){
        printf("Khong mo duoc file %s\n",filename);
        exit(1);
    }
    docfile(g,f); 
    do{
    	c=menu();
    	switch(c){
    		case 1:hienthi(g);
    		break;
    		case 2:printf("nhap dinh dau:");scanf("%d",&v1);
    			   printf("nhap dinh cuoi:");scanf("%d",&v2);
    			   if(v1==v2){
    			   		printf("nhap du lieu sai.\n");
    			   		break;
    			   }
    			   node=jrb_find_int(g,v1);
    			   if(node==NULL){
    			   		printf("dinh da nhap khong co trong do thi.\n");
    			   		break;
    			   	}
    			   if(v2!=-1){
    			   		node2=jrb_find_int(g,v2);
    			   		if(node2==NULL){
    			   			printf("dinh da nhap khong co trong do thi.\n");
    			   			break;
    			   		}
    			   }
    			   
    			   out=(int*)malloc(sizeof(int)*50);
    			   n=DFS(g,v1,v2,out);
    			   if(v2!=-1){
    			   		if(n==1){
    			   			printf("khong co duong di tu %d den %d.\n",v1,v2);
    			   			break;
    			   		}
    			   }
    			   
    			   printf("Do thi duyet theo chieu sau la:\n");
    			   for(i=0;i<n;i++) printf("%d\t",out[i]);
    			   	printf("\n");
    			   free(out);
    		break;
    		case 3:printf("nhap dinh dau:");scanf("%d",&v1);
    			   printf("nhap dinh cuoi:");scanf("%d",&v2);
    			   if(v1==v2){
    			   		printf("nhap du lieu sai.\n");
    			   		break;
    			   }
    			  	node=jrb_find_int(g,v1);
    			   	if(node==NULL){
    			   		printf("dinh da nhap khong co trong do thi.\n");
    			   		break;
    			   	}
    			   if(v2!=-1){
    			   		node2=jrb_find_int(g,v2);
    			   		if(node2==NULL){
    			   			printf("dinh da nhap khong co trong do thi.\n");
    			   			break;
    			   		}
    			   }
    			   out=(int*)malloc(sizeof(int)*50);
    			   n=BFS(g,v1,v2,out);
    			   if(v2!=-1){
    			   		if(n==1){
    			   			printf("khong co duong di tu %d den %d.\n",v1,v2);
    			   			break;
    			   		}
    			   }
    			   printf("Do thi duyet theo chieu rong la:\n");
    			   for(i=0;i<n;i++) printf("%d\t",out[i]);
    			   	printf("\n");
    			   free(out);
    		break;
    		case 4:printf("nhap dinh dau:");scanf("%d",&v1);
    			   printf("nhap dinh cuoi:");scanf("%d",&v2);
    			   if(v1==v2){
    			   		printf("nhap du lieu sai.\n");
    			   		break;
    			   }
    			   node=jrb_find_int(g,v1);
    			   node2=jrb_find_int(g,v2);
    			   if(node==NULL || node2==NULL){
    			   		printf("dinh da nhap khong co trong do thi.\n");
    			   		break;
    			   }
    			   out=(int*)malloc(sizeof(int)*50);
    			   n=BFS1(g,v1,v2,out);
    			   if(n==0){
    			   		printf("khong co duong di tu %d den %d.\n",v1,v2);
    			   		break;
    			   }
    			   printf("duong di ngan nhat tu dinh %d den dinh %d la:\n",v1,v2);
    			   for(i=0;i<n;i++) printf("%d\t",out[i]);
    			   	printf("\n");
    			   free(out);
    		break;
    		case 5:printf("nhap dinh dau:");scanf("%d",&v1);
    			   printf("nhap dinh cuoi:");scanf("%d",&v2);
    			   if(v1==v2){
    			   		printf("nhap du lieu sai.\n");
    			   		break;
    			   }
    			   node=jrb_find_int(g,v1);
    			   node2=jrb_find_int(g,v2);
    			   if(node==NULL || node2==NULL){
    			   		printf("dinh da nhap khong co trong do thi.\n");
    			   		break;
    			   }
    			   b=(mang2*)malloc(sizeof(mang2));
    			   b=duongdi(g,v1,v2);
    			   if(b->total==0) printf("ko co duong di tu %d den %d.\n",v1,v2);
                	else{
                    	printf("Tong so duong di tu dinh %d den dinh %d la:%d.\n",v1,v2,b->total);
                    	printf("cac duong di la:\n");
                    	for(i=1;i<=b->total;i++){
                        	for(j=0;j<=b->B[i].num;j++) printf("%d\t",b->B[i].A[j]);
                        	printf("\n");
                    	}
                	} 
                	free(b);
    		break;
    		case 11:printf("Ban co muon thoat khong(y/n):");scanf("%s",&d);
                        d=tolower(d);
                        if(d=='y') c=12;
                break;
    		break;
    		
    		
    	}

    }while(c!=12);
}




//*********************************************************************************
int themdinh(Graph g,int v){
	JRB res,tree;
	int f;
	res=jrb_find_gte_int(g, v, &f);
	if(f==0){
		tree=make_jrb();
    	jrb_insert_int(g,v,new_jval_v(tree));
    	return 1;
	}
	return 0;
}

int themcanh(Graph g,int v1,int v2){
    int f,f2;
    JRB res,res2,tree;
    if(v1==v2) return 0;
  	res=jrb_find_gte_int(g, v1, &f);
  if(f==1){
     tree=(JRB)jval_v(res->val);
     res2=jrb_find_gte_int(tree, v2, &f2);
     if(f2==0){
        jrb_insert_int(tree,v2,new_jval_i(1));
        return 1;
     } 
     else{ return 0;}
  }else{
    tree=make_jrb();
    jrb_insert_int(g,v1,new_jval_v(tree));
    jrb_insert_int(tree,v2,new_jval_i(1));
    return 1;
  }
   
}

int ketnoi(Graph g,int v1,int v2){
  JRB res,node,b;
  int f,f1,k;
  k=0;
  res=jrb_find_gte_int(g,v1,&f);
  if(f==1){
      b=(JRB)jval_v(res->val);
      node=jrb_find_gte_int(b,v2,&f1);
      if(f1==1) k=1;
   }
  return k;
 
}

int in(Graph g,int v,int output[]){
  JRB b,res;
  int f,f1,n,x;
  n=0;
  jrb_traverse(b,g){
    x=ketnoi(g,jval_i( b->key),v);
    if(x==1){
      	output[n]=jval_i(b->key); 
    	n=n+1;
    }
  }
  return n;

}

int out(Graph g,int v,int output[]){
   JRB res,b,c;
   int f,f1,n=0;
   res=jrb_find_gte_int(g,v,&f);
   b=(JRB)jval_v(res->val);
   jrb_traverse(c,b){
      output[n]=jval_i(c->key);
      n=n+1;
     }
   return n;
}

int DAG(Graph g){
  int visited[50];
  JRB b;
  int output[50];
  int op[50];
  int start;
  int u,v,n,i,k,m;
  Dllist q,node;
  q=new_dllist();
  //k=0;
  jrb_traverse(b,g){
    for(i=0;i<50;i++) visited[i]=0;
    start=jval_i(b->key);
    m=out(g,start,op);
    for(i=0;i<m;i++) dll_append(q,new_jval_i(op[i]));
    while(!dll_empty(q)){
       node=dll_first(q);
       u=jval_i(node->val);
       dll_delete_node(node);
       if(u==start){
         return 1;
      }
     if(visited[u]==0){
       visited[u]=1; 
       n=out(g,u,output);
       for(i=0;i<n;i++){
       if(visited[output[i]]==0) dll_append(q,new_jval_i(output[i]));
        }
     }
   }  
 }
 return 0;    
}
void hienthi(Graph g){
   JRB b,d,c;
   jrb_traverse(b,g){
      //printf("tu dinh %s co the di den cac dinh:",laytendinh(g,jval_i(b->key)));
      c=(JRB)jval_v(b->val);
      jrb_traverse(d,c){
         printf("%d => %d\n",jval_i(b->key),jval_i(d->key));}
      //printf("\n");
   }
}

void xoatatca(Graph g){
  JRB b,c;
  jrb_traverse(b,g){
     jrb_delete_node(b);     
  }
}

void xoanutrong(Graph g){
  JRB b,res;
   int n=0,v;
   int output[50];
   jrb_traverse(b,g){
      v=jval_i(b->key);
      n=out(g,v,output);
      //res=jrb_find_int(g.dinh,v);
      if(n==0) {jrb_delete_node(b);}
   }
}

int xoaketnoi(Graph g,int v1,int v2){
  JRB res,res1;
  JRB node;
  int i=0;
  res=jrb_find_int(g,v1);
  if(res!=NULL){
     node=(JRB)jval_v(res->val);
     res1=jrb_find_int(node,v2);
     if(res1==NULL) return i;
     else{
        jrb_delete_node(res1);
        i=1;
        }
  }
  return i;
}

int topology(Graph g,int ra[]){
   JRB b;
   JRB a,res;
   int output[50];
   int op[50];
   int start;
   int u,v,n,i,m,j,k;
   Dllist q,node;
   a=make_jrb();
   q=new_dllist();
  // for(i=0;i<50;i++) in[i]=0;
   //luu cay gom dinh va so luong cac dinh den dinh do 
   jrb_traverse(b,g){
      j=jval_i(b->key);
      m=in(g,j,output);
      jrb_insert_int(a,j,new_jval_i(m));
   }
   //cho nhung dinh co so luong =0 vao hang doi(dinh khong co dinh nao den no)
   jrb_traverse(res,a){
      j=jval_i(res->key);
      m=jval_i(res->val);
      if(m==0){
         dll_append(q,new_jval_i(j));
         jrb_delete_node(res);
       }
   }
   //printf("\n");
   k=0;
   while(!dll_empty(q)){
       node=dll_first(q);
       u=jval_i(node->val);
       dll_delete_node(node);
       ra[k]=u;
       k=k+1;
       //printf("%d\t",u);
       n=out(g,u,op);
       for(i=0;i<n;i++){
           res=jrb_find_int(a,op[i]);
           if(res==NULL) continue;
           m=jval_i(res->val);
           m=m-1;
           if(m==0){
              jrb_delete_node(res);
               dll_append(q,new_jval_i(op[i]));
           }
           else{
              jrb_delete_node(res);
              jrb_insert_int(a,op[i],new_jval_i(m));

           }    
       }
  }
  return k;  
} 

int BFS(Graph g,int start,int stop,int ra[]){
  int visited[max];
  int output[max];
  int pre[max];
  int u,v,n,i,k;
  Dllist node;

  //JRB node;
  Dllist q=new_dllist();
  for(i=0;i<max;i++) visited[i]=0;
  dll_append(q,new_jval_i(start));
  k=0;
  while(!dll_empty(q)){
     node=dll_first(q);
     u=jval_i(node->val);  
     dll_delete_node(node);
     if(u==stop){
        ra[k]=u;
        return k+1;
     }
     if(visited[u]==0){
         ra[k]=u;
         k=k+1;
         visited[u]=1;
         n=out(g,u,output);
         for(i=0;i<n;i++){
      		if(visited[output[i]]==0) dll_append(q,new_jval_i(output[i]));
     
         }
         
     }
  }
  return k;
      
}

int BFS1(Graph g,int start,int stop,int ra[]){
  int visited[max];
  int output[max];
  int pre[max];
  int a[max];
  int k,s;
  int u,v,n,i;
  Dllist node;
  //JRB node;
  Dllist q=new_dllist();
  for(i=0;i<max;i++) visited[i]=0;
  for(i=0;i<max;i++) pre[i]=1000;
  dll_append(q,new_jval_i(start));
  while(!dll_empty(q)){
     node=dll_first(q);
     u=jval_i(node->val);
     dll_delete_node(node);
     if(u==stop){
          if(pre[stop]==1000) return 0;
          else{
             k=0;
             do{
                 a[k]=stop;
                 stop=pre[stop];
                 k=k+1;
             }while(stop!=start);
             a[k]=start;     
             for(i=k;i>=0;i--) ra[k-i]=a[i];
             printf("\n");
             return k+1;
          }
     }
     if(visited[u]==0){
        visited[u]=1;
        n=out(g,u,output);
        for(i=0;i<n;i++){
      		if(visited[output[i]]==0){ 
      			dll_append(q,new_jval_i(output[i]));
         		if(pre[output[i]]==1000) pre[output[i]]=u;
       		}
     
        }
         
     }

  } 
  return 0;     
}

int DFS(Graph g,int start,int stop,int ra[]){
  int visited[max];
  int output[max];
  int u,v,n,i,m,k;
  Dllist q,node;
  q=new_dllist();
  for(i=0;i<max;i++) visited[i]=0;
  dll_append(q,new_jval_i(start));
  k=0;
  while(!dll_empty(q)){
     node=dll_last(q);
     u=jval_i(node->val);
     dll_delete_node(node);
     if(u==stop){
       ra[k]=u;
       return k+1;
      }
     if(visited[u]==0){
       ra[k]=u;
       k=k+1;
       visited[u]=1;
       n=out(g,u,output);
       for(i=0;i<n;i++){
     if(visited[output[i]]==0) dll_append(q,new_jval_i(output[i]));
        }
      }
   }

   return k;
}

void mangnguoc(int a[],int n){
   int i;
   int b[max];
   for(i=0;i<=n;i++) b[n-i]=a[i];
   for(i=0;i<=n;i++) a[i]=b[i];
}

int sosanhmang(int a[],int n,int b[],int m){
  int i,k;
  if(m!=n) return 0;
  k=1;
  for(i=0;i<=n;i++){
     if(a[i]!=b[i]){ k=0;}
  }
  return k; 
}

void doicho(int a[],int i,int j){int t;t=a[i];a[i]=a[j];a[j]=t;}

mang2 *duongdi(Graph g,int start,int stop){
  mang2 *a;
  int *C,*D;
  int i,j,k,l,u,v,n,m,x,s;
  int r,z;
  int y,p,e,w,f,h;
  int visited[max];                //m: so luong dinh can di qua trong duong di
  int output[max];                 //k: so luong duong di
  int pre[max];
  int pr[max1];
  int op[max];
  int outp[max1];
  Dllist q,node;
  a=(mang2*)malloc(sizeof(mang2));
  q=new_dllist();
  k=0;
  n=out(g,start,op);
  for(i=0;i<n;i++){
     x=op[i];
     for(j=0;j<max;j++) visited[j]=0;
     visited[start]=1;
     for(j=0;j<max;j++) pre[j]=0;
     pre[x]=start;
     dll_append(q,new_jval_i(x));
     while(!dll_empty(q)){
       node=dll_last(q);
       u=jval_i(node->val);
       dll_delete_node(node);
       m=0;
       s=stop;
       if(u==stop){
          C=(int*)malloc(sizeof(int)*max1);
          while(s!=start){
              C[m]=s;
              s=pre[s];
              m=m+1;
              };
          C[m]=start;
          mangnguoc(C,m);
           h=1;
           for(e=0;e<m;e++){
              f=ketnoi(g,C[e],C[e+1]);
              if(f==0) h=0;}
           if(h==1){
              r=0;
              for(e=0;e<=a->total;e++){
                  z=sosanhmang(C,m,a->B[e].A,a->B[e].num);
                  if(z==1) r=1;
              }
              if(r==0){
                  k=k+1;  
                  a->B[k].num=m;
                  a->total=k;
                  for(e=0;e<=m;e++) a->B[k].A[e]=C[e];
              } 
           }                    
          if(m>2){
             for(y=1;y<=m;y++){
                for(j=y+1;j<m;j++){
                          doicho(C,y,j);
                          h=1;
                          for(e=0;e<m;e++){
                             f=ketnoi(g,C[e],C[e+1]);
                             if(f==0) h=0;}
                          if(h==1){
                             r=0;
                             for(e=0;e<=a->total;e++){
                                z=sosanhmang(C,m,a->B[e].A,a->B[e].num);
                                if(z==1) r=1;
                             }
                             if(r==0){
                                k=k+1; 
                                a->B[k].num=m;
                                a->total=k;
                                for(e=0;e<=m;e++) a->B[k].A[e]=C[e];      
                             } 
                          }                    
                }
             }
          }
          free(C);   
          pre[u]=pre[pre[u]];
        }else{
          if(visited[u]==0){
             visited[u]=1;
             l=out(g,u,output);
             for(j=0;j<l;j++){ 
          if(visited[output[j]]==0){ 
             pre[output[j]]=u;
             dll_append(q,new_jval_i(output[j]));}}}}
     }
      
  }
  for(i=0;i<n;i++){
     x=op[i];
     for(j=0;j<max;j++) visited[j]=0;
     visited[start]=1;
     for(j=0;j<max;j++) pre[j]=0;
     pre[x]=start;
     dll_append(q,new_jval_i(x));
     while(!dll_empty(q)){
       node=dll_first(q);
       u=jval_i(node->val);
       dll_delete_node(node);
       m=0;
       s=stop;
       if(u==stop){
          C=(int*)malloc(sizeof(int)*max1);
          while(s!=start){
              C[m]=s;
              s=pre[s];
              m=m+1;
              };
          C[m]=start;
          mangnguoc(C,m);
           h=1;
           for(e=0;e<m;e++){
              f=ketnoi(g,C[e],C[e+1]);
              if(f==0) h=0;}
           if(h==1){
              r=0;
              for(e=0;e<=a->total;e++){
                  z=sosanhmang(C,m,a->B[e].A,a->B[e].num);
                  if(z==1) r=1;
              }
              if(r==0){
                  k=k+1;  
                  a->B[k].num=m;
                  a->total=k;
                  for(e=0;e<=m;e++) a->B[k].A[e]=C[e];
              } 
           }                    
          if(m>2){
             for(y=1;y<=m;y++){
                for(j=y+1;j<m;j++){
                          doicho(C,y,j);
                          h=1;
                          for(e=0;e<m;e++){
                             f=ketnoi(g,C[e],C[e+1]);
                             if(f==0) h=0;}
                          if(h==1){
                             r=0;
                             for(e=0;e<=a->total;e++){
                                z=sosanhmang(C,m,a->B[e].A,a->B[e].num);
                                if(z==1) r=1;
                             }
                             if(r==0){
                                k=k+1; 
                                a->B[k].num=m;
                                a->total=k;
                                for(e=0;e<=m;e++) a->B[k].A[e]=C[e];
                             } 
                          }                    
                }
             }
          }
          free(C);   
          pre[u]=pre[pre[u]];
        }else{
          if(visited[u]==0){
             visited[u]=1;
             l=out(g,u,output);
             for(j=0;j<l;j++){ 
          if(visited[output[j]]==0){ 
             pre[output[j]]=u;
             dll_append(q,new_jval_i(output[j]));}}}}
     }
      
  }
  return a;
}

void docfile(Graph g,FILE *f){
	char line[100];
	int v1,v2,i,j;
	i=0;
	while(1){
		fgets(line,300, f);
		//printf("i=%d\n",i);
		if(line[strlen(line) - 1]=='\n') line[strlen(line) - 1] = '\0';
		//printf("line=%s\n",line);

		for(j=0;j<strlen(line);j=j+2){
			if(line[j]=='1'){
				themdinh(g,i);
				themdinh(g,j/2);
				themcanh(g,i,j/2);	
			} 
		}
		if(feof(f)==1) break;
		i++;
 	}	
 	fclose(f);
}

int menu(){
   int c,k,i;
   char s[100];
   printf("\n**********************menu**********************\n");
   printf("1.Hien thi tat ca cac ket noi.\n2.Duyet dinh theo chieu sau(theo DFS).\n3.Duyet dinh theo chieu rong(theo BFS).\n4.Duong di ngan nhat giua 2 dinh.\n5.Tat ca cac duong di giua hai dinh.\n11.Thoat.\n");
   printf("************************************************\n");
   do{
     printf("Moi ban chon:");
     scanf("%s",s);
     for(i=0;i<strlen(s);i++){
     	if(s[i]<'0' || s[i]>'9'){
     		c=0;
     		break;
     	} 
     	else c=atoi(s);
     }
     //printf("c=%d\n",c );
     while(getchar()!='\n');
   }while(c<1||c>11);
   return c;
}
