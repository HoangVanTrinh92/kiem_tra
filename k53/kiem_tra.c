#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jrb.h"
#include "fields.h"
#include "dllist.h"
#define MAX 100           //so luong dinh toi da co trong do thi
#define MAX1 1000         //so luong duong di toi da trong do thi
#define SIZE 10

//typedef JRB Graph;
typedef struct{
  JRB dinh;
  JRB canh;
}Graph;

typedef struct{
   int A[MAX];
   int num;
}mang1;

typedef struct{
   mang1 B[MAX1];   
   int total;   
}mang2;

int size_matrix;
//********************************************************************************************
Graph create();
//tao Graph

int taomadinh(Graph g);
int laymadinh(Graph g,char* name);
char*laytendinh(Graph g,int id);//lay ra ten dinh qua ma so
int themcanh(Graph g,int v1,int v2);//them mot canh tu v1 den v2
void themdinh(Graph g,int id,char* name);//them mot dinh
int ketnoi(Graph g,int v1,int v2);//kiem tra xem co ket noi tu v1 den v2 ko
int in(Graph g,int v,int output[]);//tat ca cac dinh di den v
int out(Graph g,int v,int output[]);//tat ca cac dinh ma dinh v di den
int DAG(Graph g);//kiem tra xem do thi g co chu trinh hay ko dung DFS
void hienthi(Graph g);
void hienthidinh(Graph g);
int BFS1(Graph g,int start,int stop,int ra[]);//duong di ngan nhat giua 2 dinh
void duongdi2(Graph g,mang2 *a,int v1,int v2,int stop);//tat ca duong di xuat phat tu dinh v1
mang2 *duongdi(Graph g,int start,int stop);//tat ca duong di giua hai dinh(neu dinh stop la -1 thi la tat ca duong di xuat phat
    //tu dinh start)
void All_node_visited_a(Graph g,int v,mang1 *a,int stop);
mang1* All_node_visited(Graph g,int v);//tat ca cac node phai den truoc khi tham node v
int check_mang_nguoc(int A[],int n,int B[],int m);//kiem tra hai mang A,B co phai la mang nguoc cua nhau ko    
void docfile(Graph g,FILE *f);   
void All_node_visit_a(Graph g,int v,mang1 *a,int stop);
mang1* All_node_visit(Graph g,int v);//tat ca cac node ma tu node v co the den duoc  
mang2 *phan_cap_node(Graph g,int v);//chia bac(bac 1,2,3,...) tat ca cac node ma node v co the den duoc(coi node v la goc co bac la 0)  

//*************************************************************************************************
main(){
    Graph g;
    JRB node,res,res2,node2;
    int c;
    char d;
    FILE *f;
    char *name;
    char *name2;
    int v,v1,v2,n,i,k,j,check,check1,min,max;
    mang2 *b,*b1;
    mang1 *a;
    int *output;
    int A[MAX][MAX];
    int visited[MAX];
    int cap_ban_be[MAX];
    g=create(); 
    f=fopen("mangxahoi.txt", "r");
    if(f==NULL){
        printf("Khong mo duoc file mangxahoi.txt\n");
        exit(1);
    }
    else docfile(g,f);
    do{
        c=menu();
        switch(c){
     
            case 1 :
                //hienthidinh(g);
                //hienthi(g);
                i=0;
                jrb_traverse(node,g.dinh){
                    i++;
                }
                printf("Tong so nguoi xuat hien trong mang xa hoi la: %d\n",i);
                printf("Danh Sach nhung nguoi do la:\n");
                jrb_traverse(node,g.dinh){
                    v=jval_i(node->key);
                    printf("%s\n",laytendinh(g,v));
                }
            break;
            case 2:
                i=0;
                jrb_traverse(node,g.dinh){
                    v=jval_i(node->key);
                    output=(int*)malloc(sizeof(int)*MAX);
                    n=out(g,v,output);
                    i=i+n;
                    free(output);
                }
                 
                
                printf("Tong so quan he ban be trong mang xa hoi la: %d\n",i/2);
            break;       
            case 3 :
                jrb_traverse(node,g.dinh){
                    v=jval_i(node->key);
                    output=(int*)malloc(sizeof(int)*MAX);
                    n=out(g,v,output);
                    if(n==0) printf("%s khong co ban be.\n",laytendinh(g,v));
                    else{
                        printf("%s co ban be la ",laytendinh(g,v));
                        for(i=0;i<n;i++){
                            if(i==n-1) printf("%s.\n",laytendinh(g,output[i]));
                            else printf("%s,",laytendinh(g,output[i]));
                        }
                    }
                }
            break;  
            case 4 :
                name=(char*)malloc(sizeof(char)*20);
                printf("Nhap ten:");
                fgets(name,20,stdin);
                name[strlen(name)-1]='\0';
                v1=laymadinh(g,name);
                if(v1==-1){
                    printf("%s khong co trong mang xa hoi.\n",name);
                    break;
                }
                a=(mang1*)malloc(sizeof(mang1));
                a=All_node_visited(g,v1);
                printf("Tong so tat ca nguoi ban co moi quan he xa gan voi %s la: %d.\n",name,a->num);
                if(a->num!=0){
                    for(i=0;i<MAX;i++) cap_ban_be[i]=0;
                    output=(int*)malloc(sizeof(int)*MAX);
                    n=out(g,v1,output);
                    for(i=0;i<n;i++) cap_ban_be[output[i]]=1;
                    //printf("Danh sach nhung nguoi ban do la: ");
                    free(output);    
                    while(1){
                        check=1;
                        for(i=0;i<a->num;i++){
                            if(cap_ban_be[a->A[i]]==0){
                                check=0;//co mot nguoi chua co cap ban be
                                break;
                            }
                        }
                        if(check==1) break;//tat ca deu co cap ban be thi dung lai.
                        for(i=0;i<a->num;i++){
                            if(cap_ban_be[a->A[i]]==0){
                                output=(int*)malloc(sizeof(int)*MAX);
                                n=out(g,a->A[i],output);
                                min=100;
                                if(n!=0){
                                    for(j=0;j<n;j++){
                                        if(cap_ban_be[output[j]]!=0){
                                            if(cap_ban_be[output[j]]<min) min=cap_ban_be[output[j]];
                                        }  
                                    }
                                    if(min!=100) cap_ban_be[a->A[i]]=min+1;
                                    
                                }
                                free(output);
                            }
                        } 
                    }
                    //for(i=0;i<a->num;i++) printf("cap ban be cua %s la: %d\n",laytendinh(g,a->A[i]),cap_ban_be[a->A[i]]);
                    b=(mang2*)malloc(sizeof(mang2));
                    b->total=0;
                    for(i=0;i<MAX1;i++) b->B[i].num=0;
                    for(i=0;i<a->num;i++){
                        if(cap_ban_be[a->A[i]] > b->total) b->total=cap_ban_be[a->A[i]];
                        b->B[cap_ban_be[a->A[i]]-1].A[b->B[cap_ban_be[a->A[i]]-1].num]=a->A[i];
                        b->B[cap_ban_be[a->A[i]]-1].num++;
                    }
                    for(i=0;i<b->total;i++){
                        printf("Ban cap %d:",i+1);
                        for(j=0;j<b->B[i].num;j++){
                            if(j==b->B[i].num-1) printf("%s.\n",laytendinh(g,b->B[i].A[j]));
                            else printf("%s,",laytendinh(g,b->B[i].A[j]));
                        }
                    } 
                    free(b);   
                        
                }
                free(a);
            break;
            case 5:
            printf("Ban co muon thuc hien lai cong viec 4 khong(Y/N):");scanf("%s",&d);
            d=tolower(d);
            if(d=='y'){
                name=(char*)malloc(sizeof(char)*20);
                printf("Nhap ten:");
                while(getchar()!='\n');
                fgets(name,20,stdin);
                name[strlen(name)-1]='\0';
                v1=laymadinh(g,name);
                if(v1==-1){
                    printf("%s khong co trong mang xa hoi.\n",name);
                    break;
                }
                a=(mang1*)malloc(sizeof(mang1));
                a=All_node_visited(g,v1);
                printf("Tong so tat ca nguoi ban co moi quan he xa ngan voi %s la: %d.\n",name,a->num);
                if(a->num!=0){
                    for(i=0;i<MAX;i++) cap_ban_be[i]=0;
                    output=(int*)malloc(sizeof(int)*MAX);
                    n=out(g,v1,output);
                    for(i=0;i<n;i++) cap_ban_be[output[i]]=1;
                    //printf("Danh sach nhung nguoi ban do la: ");
                    free(output);    
                    while(1){
                        check=1;
                        for(i=0;i<a->num;i++){
                            if(cap_ban_be[a->A[i]]==0){
                                check=0;//co mot nguoi chua co cap ban be
                                break;
                            }
                        }
                        if(check==1) break;//tat ca deu co cap ban be thi dung lai.
                        for(i=0;i<a->num;i++){
                            if(cap_ban_be[a->A[i]]==0){
                                output=(int*)malloc(sizeof(int)*MAX);
                                n=out(g,a->A[i],output);
                                min=100;
                                if(n!=0){
                                    for(j=0;j<n;j++){
                                        if(cap_ban_be[output[j]]!=0){
                                            if(cap_ban_be[output[j]]<min) min=cap_ban_be[output[j]];
                                        }  
                                    }
                                    if(min!=100) cap_ban_be[a->A[i]]=min+1;
                                    
                                }
                                free(output);
                            }
                        } 
                    }
                    //for(i=0;i<a->num;i++) printf("cap ban be cua %s la: %d\n",laytendinh(g,a->A[i]),cap_ban_be[a->A[i]]);
                    b=(mang2*)malloc(sizeof(mang2));
                    b->total=0;
                    for(i=0;i<MAX1;i++) b->B[i].num=0;
                    for(i=0;i<a->num;i++){
                        if(cap_ban_be[a->A[i]] > b->total) b->total=cap_ban_be[a->A[i]];
                        b->B[cap_ban_be[a->A[i]]-1].A[b->B[cap_ban_be[a->A[i]]-1].num]=a->A[i];
                        b->B[cap_ban_be[a->A[i]]-1].num++;
                    }
                    for(i=0;i<b->total;i++){
                        printf("Ban cap %d:",i+1);
                        for(j=0;j<b->B[i].num;j++){
                            if(j==b->B[i].num-1) printf("%s.\n",laytendinh(g,b->B[i].A[j]));
                            else printf("%s,",laytendinh(g,b->B[i].A[j]));
                        }
                    } 
                    free(b);   
                        
                }
                free(a);
            }
            break;     
            case 6:     
            printf("Ban co muon thoat khoi chuong trinh khong(y/n):");scanf("%c",&d);
            d=tolower(d);
            if(d=='y') c=22;
            break;  
     }
   }while(c!=22);
}


int menu(){
    int c,i;
    char s[100];
    printf("\n**********************MENU*************************************************\n");
    printf("1.Tong so nguoi va ten xuat hien trong mang xa hoi.\n2.Tong so quan he ban be trong mang xa hoi.\n");
    printf("3.Danh sach ca nhan va moi quan he ban be.\n4.Thong tin ve mot nguoi.\n");
    printf("5.Chuc nang 5.\n6.Thoat.\n");
   printf("***************************************************************************\n");
   do{
     printf("Moi ban chon:");scanf("%s",s);
     for(i=0;i<strlen(s);i++){
        if(s[i]<'0' || s[i]>'9'){
            c=0;
            break;
            
        } 
        else c=atoi(s); 
     }
     
     //printf("c=%d\n",c );
     while(getchar()!='\n');
   }while(c<1||c>6);
   return c;
}


//**************************************************************************************************

void docfile(Graph g,FILE *f){
    char line[300];
    char *name,*name2,*s;
    int v1,v2,i;
    while(1){
        fgets(line,300, f);
        if(line[strlen(line) - 1]=='\n') line[strlen(line) - 1] = '\0';
        char *ptr;
        ptr = strtok(line,":");
        name=(char*)malloc(sizeof(char)*20);
        strcpy(name,ptr);
        name2=(char*)malloc(sizeof(char)*20);
        strcpy(name2, ptr+strlen(ptr)+2);
        //printf("name=(%s)\n",name);
        v1=laymadinh(g,name);
        if(v1==-1){
            v1=taomadinh(g);
            themdinh(g,v1,name);
        }
        if(strlen(name2)!=0){
            //printf("name2=(%s)\n",name2);
            v2=laymadinh(g,name2);
            if(v2==-1){
                v2=taomadinh(g);
                themdinh(g,v2,name2);
            }
            themcanh(g,v1,v2);
            themcanh(g,v2,v1);
        }
        if(feof(f)==1) break;
    }

}

Graph create(){
  Graph g;
  g.dinh=make_jrb();
  g.canh=make_jrb();
  return g;
}

int laymadinh(Graph g,char* name){
 JRB b,c;
 int k;
  jrb_traverse(b,g.dinh){
     if(strcmp(name,jval_s(b->val))==0) return jval_i(b->key);    
  } 
  return -1;
}

int taomadinh(Graph g){
   int i,j,u;
   JRB b;
   Dllist node;
   Dllist q=new_dllist();
   jrb_traverse(b,g.dinh){
      j=jval_i(b->key);
      dll_append(q,new_jval_i(j));
   }
   if(dll_empty(q)) return 0;
   else{
     node=dll_last(q);
     u=jval_i(node->val);  
     dll_delete_node(node);
     i=u+1;
   }    
   return i;
}

void themdinh(Graph g,int id,char* name){
  JRB res;
  //res=jrb_find_int(g.dinh,id);
  jrb_insert_int(g.dinh,id,new_jval_s(name));
}

char*laytendinh(Graph g,int id){
  JRB res;
  res=jrb_find_int(g.dinh,id);
  if(res==NULL) return " ";
  //if(res==NULL){printf("ko co dinh %d trong do thi.\n",id);return;}
  //else printf("ten cua dinh %d la:\'%s\'.\n",id,jval_s(res->val));
  return jval_s(res->val);
}


int themcanh(Graph g,int v1,int v2){
    int f,f2;
    JRB res,res2,tree;
  res=jrb_find_gte_int(g.canh, v1, &f);
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
    jrb_insert_int(g.canh,v1,new_jval_v(tree));
    jrb_insert_int(tree,v2,new_jval_i(1));
    return 1;
  }
   
}

int ketnoi(Graph g,int v1,int v2){
  JRB res,node,b;
  int f,f1,k;
  k=0;
  res=jrb_find_gte_int(g.canh,v1,&f);
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
  jrb_traverse(b,g.dinh){
    x=ketnoi(g,jval_i( b->key),v);
    if(x==1){
      output[n]=jval_i(b->key); 
    n=n+1;}
  }
  return n;

}

int out(Graph g,int v,int output[]){
  JRB res,b;
  int f,n,x;
  n=0;
  jrb_traverse(b,g.dinh){
    x=ketnoi(g,v,jval_i(b->key));
    if(x==1){
      output[n]=jval_i(b->key);
      n=n+1;
    }
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
  jrb_traverse(b,g.dinh){
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
   jrb_traverse(b,g.canh){
      //printf("tu dinh \'%s\' co the di den cac dinh:",laytendinh(g,jval_i(b->key)));
      c=(JRB)jval_v(b->val);
      jrb_traverse(d,c){
         printf("\'%s\' => \'%s\'\n",laytendinh(g,jval_i(b->key)),laytendinh(g,jval_i(d->key)));}
      //printf("\n");
   }
   jrb_traverse(b,g.canh){
      //printf("tu dinh \'%s\' co the di den cac dinh:",laytendinh(g,jval_i(b->key)));
      c=(JRB)jval_v(b->val);
      jrb_traverse(d,c){
         printf("%d => %d\n",jval_i(b->key),jval_i(d->key));}
      //printf("\n");
   }
}

void hienthidinh(Graph g){
   JRB b,c;
   printf("%-15s %-10s\n","Ten dinh","Ma dinh");
   jrb_traverse(b,g.dinh){
      printf("%-15s %-10d\n",jval_s(b->val),jval_i(b->key));
   }
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
        return b;
    }

}

void All_node_visited_a(Graph g,int v,mang1 *a,int stop){
    int i,j,n;
    int output[MAX];
    int check;
    n=in(g,v,output);
    if(n!=0){
        //them nuode vao danh sach
        for(i=0;i<n;i++){
            check=0;
            for(j=0;j<(*a).num;j++){
                if(output[i]==(*a).A[j] || output[i]==stop){
                   check=1;
                   break; 
                } 
            }
            if(check==0){
                (*a).A[(*a).num]=output[i];
                (*a).num+=1;
                All_node_visited_a(g,output[i],a,stop);
            }
        }
    }
}

mang1* All_node_visited(Graph g,int v){
    mang1* a;
    int i,j,n;
    int output[MAX];
    a=(mang1*)malloc(sizeof(mang1));
    a->num=0;
    n=in(g,v,output); 
    if(n!=0){
        for(i=0;i<n;i++){
            a->A[i]=output[i];
            a->num+=1;    
        }
        for(i=0;i<n;i++){
            All_node_visited_a(g,output[i],a,v);
        }
        for(i=0;i<a->num-1;i++){
            for(j=i+1;j<a->num;j++){
                if(ketnoi(g,a->A[i],a->A[j])==1){
                    n=a->A[i];
                    a->A[i]=a->A[j];
                    a->A[j]=n;
                }
            }
        }

    }
    
    return a;
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

void All_node_visit_a(Graph g,int v,mang1 *a,int stop){
    int i,j,n;
    int output[MAX];
    int check;
    n=out(g,v,output);
    if(n!=0){
        //them nuode vao danh sach
        for(i=0;i<n;i++){
            check=0;
            for(j=0;j<(*a).num;j++){
                if(output[i]==(*a).A[j] || output[i]==stop){
                   check=1;
                   break; 
                } 
            }
            if(check==0){
                (*a).A[(*a).num]=output[i];
                (*a).num+=1;
                All_node_visit_a(g,output[i],a,stop);
            }
        }
    }
}

mang1* All_node_visit(Graph g,int v){
    mang1* a;
    int i,j,n;
    int output[MAX];
    a=(mang1*)malloc(sizeof(mang1));
    a->num=0;
    n=out(g,v,output); 
    if(n!=0){
        for(i=0;i<n;i++){
            a->A[i]=output[i];
            a->num+=1;    
        }
        for(i=0;i<n;i++){
            All_node_visit_a(g,output[i],a,v);
        }
        for(i=0;i<a->num-1;i++){
            for(j=i+1;j<a->num;j++){
                if(ketnoi(g,a->A[i],a->A[j])==1){
                    n=a->A[i];
                    a->A[i]=a->A[j];
                    a->A[j]=n;
                }
            }
        }

    }
    
    return a;
}

mang2 *phan_cap_node(Graph g,int v){
    mang1 *a;
    mang2 *b;
    int *output;
    int cap_ban_be[MAX];
    int check,check1,i,j,k,n,min,max;
    b=(mang2*)malloc(sizeof(mang2));
    b->total=0;
    a=(mang1*)malloc(sizeof(mang1));
    a=All_node_visit(g,v);
    //printf("Tong so tat ca nguoi ban co moi quan he xa ngan voi %s la: %d.\n",name,a->num);
    if(a->num!=0){
        for(i=0;i<MAX;i++) cap_ban_be[i]=0;
        output=(int*)malloc(sizeof(int)*MAX);
        n=out(g,v,output);
        for(i=0;i<n;i++) cap_ban_be[output[i]]=1; 
        while(1){
            check=1;
            for(i=0;i<a->num;i++){
                if(cap_ban_be[a->A[i]]==0){
                    check=0;//co mot nguoi chua co cap ban be
                    break;
                }
            }
            if(check==1) break;//tat ca deu co cap ban be thi dung lai.
            for(i=0;i<a->num;i++){
                if(cap_ban_be[a->A[i]]==0){
                    output=(int*)malloc(sizeof(int)*MAX);
                    n=out(g,a->A[i],output);
                    min=100;
                    if(n!=0){
                        for(j=0;j<n;j++){
                            if(cap_ban_be[output[j]]!=0){
                                if(cap_ban_be[output[j]]<min) min=cap_ban_be[output[j]];
                            }  
                        }
                        if(min!=100) cap_ban_be[a->A[i]]=min+1;
                                    
                    }
                }
            } 
        }
        for(i=0;i<MAX1;i++) b->B[i].num=0;
        for(i=0;i<a->num;i++){
            if(cap_ban_be[a->A[i]] > b->total) b->total=cap_ban_be[a->A[i]];
            b->B[cap_ban_be[a->A[i]]-1].A[b->B[cap_ban_be[a->A[i]]-1].num]=a->A[i];
            b->B[cap_ban_be[a->A[i]]-1].num++;
        }        
    }
    return b;
}