#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jrb.h"
#include "fields.h"
#include "dllist.h"
#define MAX 100           //so luong dinh toi da co ket noi voi mot dinh
#define MAX1 1000          //so luong duong di lon nhat giua hai dinh

typedef JRB Graph;

typedef struct{
   int A[MAX];
   int num;
   int wei;
}mang1;

typedef struct{
   mang1 B[MAX1];   //tinh tu B[1] (ko tinh B[0])
   int total;   
}mang2;

//********************************************************************************************
int themdinh(Graph g,int v);
int themcanh(Graph g,int v1,int v2,int wei);//them mot canh tu v1 den v2
int ketnoi(Graph g,int v1,int v2);//kiem tra xem co ket noi tu v1 den v2 ko
int trong_so(Graph g,int v1,int v2);//tra trong so ket noi tu v1 den v2
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
void duongdi2(Graph g,mang2 *a,int v1,int v2,int stop);//tat ca duong di xuat phat tu dinh v1
mang2 *duongdi(Graph g,int start,int stop);//tat ca duong di giua hai dinh(neu dinh stop la -1 thi la tat ca duong di xuat phat
    //tu dinh start)
//*************************************************************************************************

main(){
	Graph g;
	JRB node,node2,tree;
	FILE *f;
	int c,v1,v2,n,i,j,wei,v,k,count,min,max;
	int *output,visited[MAX];
	mang2 *b,*b1;
	char d;
	g=make_jrb();
	char filename[]="data.txt";
    
    do{
    	c=menu();
    	switch(c){
    		case 1://hienthi(g);
                f=fopen(filename, "r");
                if(f==NULL){
                    printf("Khong mo duoc file %s\n",filename);
                    exit(1);
                }
                docfile(g,f);
                printf("Ma tran mang luoi dien cua toa nha la:\n");
                jrb_traverse(node,g){
                    v1=jval_i(node->key);
                    jrb_traverse(node2,g){
                        v2=jval_i(node2->key);
                        if(v1==v2) printf("0 ");
                        else{
                            wei=trong_so(g,v1,v2);
                            printf("%d ",wei);
                        }
                    }
                    printf("\n");
                } 
    		break;
    		case 2:
                i=0;
                jrb_traverse(node,g){
                    i++;
                }
                //hienthidinh(g);
                //hienthi(g);
                printf("Tong so dinh co trong do thi la: %d\n",i);
                i=0;
                jrb_traverse(node,g){
                    v=jval_i(node->key);
                    output=(int*)malloc(sizeof(int)*MAX);
                    n=out(g,v,output);
                    i=i+n;
                    free(output);
                }
                printf("Tong so cung trong do thi la: %d\n",i/2);
    		break;
    		case 3:printf("Nhap so phong:");scanf("%d",&v);
    			node=jrb_find_int(g,v);
    			if(node==NULL){
    			   	printf("dinh da nhap khong co trong do thi.\n");
    			   	break;
    			}
    			output=(int*)malloc(sizeof(int)*50);
    			n=out(g,v,output);
                if(n==0) printf("Khong co phong nao co dan dan noi den phong %d.\n",v);
                else{
                    printf("Cac phong co dan dan noi den phong %d la:\n",v);
                    for(i=0;i<n;i++){
                        if(i==n-1) printf("Phong %d.\n",output[i]);
                        else printf("Phong %d,",output[i]);
                    }
                }
    			
    			free(output);
    		break;
    		case 4:
                printf("nhap phong dau:");scanf("%d",&v1);
    			printf("nhap phong cuoi:");scanf("%d",&v2);
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
    			output=(int*)malloc(sizeof(int)*MAX);
                n=DFS(g,v1,v2,output);
                if(n==1 || v2!=output[n-1]){
                    printf("Khong co duong di day dien tu Phong %d den Phong %d.\n",v1,v2);
                    break;
                }
                else printf("Co duong di day dien tu Phong %d den Phong %d.\n",v1,v2);
    			free(output);
    		break;
    		case 5:
                v1=0;//so luong dinh trong do thi
                j=0;
                jrb_traverse(node,g){
                    v1++;
                }
                jrb_traverse(node,g){
                    output=(int*)malloc(sizeof(int)*MAX);
                    n=DFS(g,jval_i(node->key),-1,output);
                    if(n<v1){
                        printf("Do thi khong lien thong\n");
                        free(output);
                        j=1;
                        break;
                    }
                    free(output);
                }
                if(j==0) printf("Tat ca cac phong co lien thong voi nhau.\n");
                else{
                    count=1;
                    for(j=0;j<MAX;j++) visited[j]=0;
                    for(j=0;j<n;j++) visited[output[j]]=1;
                    jrb_traverse(node,g){
                        k=jval_i(node->key);
                        if(visited[k]==0){
                            output=(int*)malloc(sizeof(int)*MAX);
                            n=DFS(g,k,-1,output);
                            for(i=0;i<n;i++){
                                //printf("%d\t",output[i]);
                                visited[output[i]]=1;
                                
                            }
                            count++;
                            //printf("\n");
                            //free(output);
                        }
                    }
                    printf("So luong cac thanh phan lien thong cua do thi la: %d.\n",count);   

                }
    		break;
            case 6:
                v1=0;//so luong dinh co trong do thi
                j=0;
                jrb_traverse(node,g){
                    v1++;
                }
                jrb_traverse(node,g){
                    output=(int*)malloc(sizeof(int)*MAX);
                    n=DFS(g,jval_i(node->key),-1,output);
                    //printf("n=%d\n",n);
                    if(n<v1){
                        printf("Do thi khong lien thong\n");
                        free(output);
                        j=1;
                        break;
                    }
                    free(output);
                }
                if(j==0){
                    b=(mang2*)malloc(sizeof(mang2));
                    b->total=0;
                    jrb_traverse(node,g){
                        v=jval_i(node->key);
                        b1=(mang2*)malloc(sizeof(mang2));
                        b1=duongdi(g,v,-1);
                        min=-1;
                        //k=0;
                        for(i=0;i<b1->total;i++){
                            if(b1->B[i].num==v1){//duong di qua tat ca cac dinh
                                if(min==-1){
                                    min=b1->B[i].wei;
                                    k=i;
                                }
                                else if(b1->B[i].wei<min){
                                        min=b1->B[i].wei;
                                        k=i;
                                }
                            }
                            for(j=0;j<b1->B[i].num;j++){
                                //printf("%d ",b1->B[i].A[j]);
                            }
                            //printf("\n");
                            //printf("\t(wei=%d,num=%d,min=%d)\n",b1->B[i].wei,b1->B[i].num,min);
                        }
                        if(min!=-1){
                            //printf("n=%d\n",b1->total);
                            //printf("Duong di co trong so nho nhat:\n"); 
                            for(j=0;j<b1->B[k].num;j++){
                                //printf("%d ",b1->B[k].A[j]);
                                b->B[b->total].A[j]=b1->B[k].A[j];

                            }
                            //printf("\n");
                            //getchar();
                            b->B[b->total].num=b1->B[k].num;
                            b->B[b->total].wei=b1->B[k].wei;
                            b->total++;
                            //printf("\n");
                        }
                        free(b1);
                    }
                    //printf("Duong di trong mang b.\n");
                    k=0;
                    min=b->B[0].wei;
                    for(i=0;i<b->total;i++){
                        if(b->B[i].wei<min){
                            min=b1->B[i].wei;
                            k=i;
                        }
                        for(j=0;j<b->B[i].num;j++){
                            //printf("%d ",b->B[i].A[j]);
                        }
                        //printf("\n");
                        //printf("\t(wei=%d)\n",b->B[i].wei);
                    }
                    //printf("n=%d\n",b->total);
                    //printf("Duong di co trong so nho nhat:\n"); 
                    printf("Phuong an lap dat day dan sao cho do dai day dan la nho nhat la:\n");
                    for(j=0;j<b->B[k].num;j++){
                        printf("Phong %d ",b->B[k].A[j]);

                    }
                    printf("\n");
                    free(b);

                }
                
            break;
    		case 7:printf("Ban co muon thoat khong(y/n):");scanf("%s",&d);
                        d=tolower(d);
                        if(d=='y') c=12;
                break;
    		break;
    		
    		
    	}

    }while(c!=12);
}


int menu(){
    int c,k,i;
    char s[100];
    printf("\n**********************menu**********************\n");
    printf("1.Doc ma va hien thi ma tran tu do thi.\n2.Tong so dinh va cung cua do thi.\n");
    printf("3.Phong co dan dan den mot phong.\n4.Kiem tra duong di giua hai phong.\n5.Tinh lien thong.\n6.Lap dat day dan.\n");
    printf("7.Thoat.\n");
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
   }while(c<1||c>7);
   return c;
}

//*********************************************************************************

void docfile(Graph g,FILE *f){
  char line[100];
  int v1,v2,i,j,wei;
  i=0;
  while(1){
    fgets(line,300, f);
    //printf("i=%d\n",i);
    if(line[strlen(line) - 1]=='\n') line[strlen(line) - 1] = '\0';
    //printf("line=%s\n",line);

    char *ptr;
    ptr = strtok(line, " ");
    j=0;
    while(ptr){
        wei=atoi(ptr);
        //printf("wei=%d\t",wei);
        if(wei!=0){
            themdinh(g,i);
            themdinh(g,j);
            themcanh(g,i,j,wei);
            themcanh(g,j,i,wei);
        }
        j++;
        ptr = strtok(NULL, " ");
    }
    //printf("\n");
    if(feof(f)==1) break;
    i++;
  } 
  fclose(f);
}


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

int trong_so(Graph  g,int v1,int v2){
    JRB node,tree,node2;
    if(ketnoi(g,v1,v2)==0) return 0;
    else{
        node=jrb_find_int(g,v1);
        tree=(JRB)jval_v(node->val);
        node2=jrb_find_int(tree,v2);
        return jval_i(node2->val);
    }
}

int themcanh(Graph g,int v1,int v2,int wei){
    int f,f2;
    JRB res,res2,tree;
    if(v1==v2) return 0;
  	res=jrb_find_gte_int(g, v1, &f);
  if(f==1){
     tree=(JRB)jval_v(res->val);
     res2=jrb_find_gte_int(tree, v2, &f2);
     if(f2==0){
        jrb_insert_int(tree,v2,new_jval_i(wei));
        return 1;
     } 
     else{ return 0;}
  }else{
    tree=make_jrb();
    jrb_insert_int(g,v1,new_jval_v(tree));
    jrb_insert_int(tree,v2,new_jval_i(wei));
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
  int visited[MAX];
  int output[MAX];
  int pre[MAX];
  int u,v,n,i,k;
  Dllist node;

  //JRB node;
  Dllist q=new_dllist();
  for(i=0;i<MAX;i++) visited[i]=0;
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
  int visited[MAX];
  int output[MAX];
  int pre[MAX];
  int a[MAX];
  int k,s;
  int u,v,n,i;
  Dllist node;
  //JRB node;
  Dllist q=new_dllist();
  for(i=0;i<MAX;i++) visited[i]=0;
  for(i=0;i<MAX;i++) pre[i]=1000;
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
  int visited[MAX];
  int output[MAX];
  int u,v,n,i,m,k;
  Dllist q,node;
  q=new_dllist();
  for(i=0;i<MAX;i++) visited[i]=0;
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

void duongdi2(Graph g,mang2 *a,int v1,int v2,int stop){
    //Neu v1 la phan tu cuoi cung cua duong di(num-1) va v2 khong co trong duong di cuoi cung thi them v2 vao cuoi duong di va de qui 
    //voi v2 va cac dinh co ket noi den v2.
    //Neu v1 la phan tu gan cuoi cung cua duong di(num-2) va v2 khong co trong duong di thi tang so luong duong di len 1 va cac node
    //trong duong di lay tu num-2 node cua duong di dang truoc va v2
    int i,j,k,check,check1,n;
    int output[MAX];
    if((*a).B[(*a).total-1].A[(*a).B[(*a).total-1].num-1]==v1){//phan tu cuoi cung cua mang la v1
        check=0;
        for(i=0;i<(*a).B[(*a).total-1].num;i++){
            if((*a).B[(*a).total-1].A[i]==v2){
                check=1;//v2 co trong mang
                break;
            }
        }
        //them node vao duong di
        if(check==0){
            (*a).B[(*a).total-1].A[(*a).B[(*a).total-1].num]=v2;
            (*a).B[(*a).total-1].num++;
            if(stop==-1){
                n=out(g,v2,output);
                for(i=0;i<n;i++) duongdi2(g,a,v2,output[i],stop);
            }
            else{
                if(v2!=stop){
                    n=out(g,v2,output);
                    for(i=0;i<n;i++) duongdi2(g,a,v2,output[i],stop);
                }
            }            
        }
    }
    else{
        check=0;
        for(i=0;i<(*a).B[(*a).total-1].num;i++){
            if((*a).B[(*a).total-1].A[i]==v1){
                check=1;
                break;
            }
        }
        //them duong di moi
        if(check==1){
            check1=0;
            for(j=0;j<i;j++){
                if((*a).B[(*a).total-1].A[j]==v2){
                    check1=1;
                    break;
                }
            }
            if(check1==0){
                (*a).total++;
                for(j=0;j<=i;j++){
                    (*a).B[(*a).total-1].A[j]=(*a).B[(*a).total-2].A[j];
                    (*a).B[(*a).total-1].num=j+1;
                }
                (*a).B[(*a).total-1].A[(*a).B[(*a).total-1].num]=v2;
                (*a).B[(*a).total-1].num++;
                if(stop==-1){
                n=out(g,v2,output);
                for(i=0;i<n;i++) duongdi2(g,a,v2,output[i],stop);
                }
                else{
                    if(v2!=stop){
                        n=out(g,v2,output);
                        for(i=0;i<n;i++) duongdi2(g,a,v2,output[i],stop);
                    }
                }   
            }
        }
    }
}

mang2 *duongdi(Graph g,int start,int stop){
    mang2 *a;
    mang2 *b;
    int *output;
    int n,i,j,k;
    a=(mang2*)malloc(sizeof(mang2));
    a->total=1;
    a->B[0].num=1;
    a->B[0].A[0]=start;
    output=(int*)malloc(sizeof(int)*MAX);
    n=out(g,start,output);
    for(i=0;i<n;i++){
        duongdi2(g,a,start,output[i],stop);
    }
    if(a->total==1 &&  a->B[0].num==1){
            a->total=0;
            return a;
        }   
    if(stop==-1){
        for(i=0;i<a->total;i++){
            a->B[i].wei=0;
            for(j=0;j<a->B[i].num-1;j++){
                a->B[i].wei+=trong_so(g,a->B[i].A[j],a->B[i].A[j+1]);
            }
        }
        return a;
    }
    else{
        k=0;
        b=(mang2*)malloc(sizeof(mang2));
        for(i=0;i<a->total;i++){
            if(a->B[i].A[a->B[i].num-1]==stop){
                for(j=0;j<a->B[i].num;j++){
                    b->B[k].A[j]=a->B[i].A[j];
                }
                b->B[k].num=a->B[i].num;
                k++;
                b->total=k;
            }            
        }
        for(i=0;i<b->total;i++){
            b->B[i].wei=0;
            for(j=0;j<b->B[i].num-1;j++){
                b->B[i].wei+=trong_so(g,b->B[i].A[j],b->B[i].A[j+1]);
            }
        }
        return b;
    }

}