#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jrb.h"
#include "fields.h"
#include "dllist.h"
#define MAX 100           //so luong dinh toi da co trong do thi
#define MAX1 300          //so luong duong di lon nhat giua hai dinh
#define INFINITIVE_VALUE 100000

//typedef JRB Graph;
typedef struct{
  JRB dinh;
  JRB canh;
}Graph;

typedef struct{
   int A[MAX1];
   int num;
   int wei;
}mang1;

typedef struct{
   mang1 B[MAX];   
   int total;   
}mang2;

//********************************************************************************************
Graph create();
//tao Graph

int taomadinh(Graph g);
int laymadinh(Graph g,char* name);
char*laytendinh(Graph g,int id);//lay ra ten dinh qua ma so
int themcanh(Graph g,int v1,int v2,int wei);//them mot canh tu v1 den v2
int trong_so(Graph g,int v1,int v2);//tra trong so ket noi tu v1 den v2
int Dijktra(Graph g,int start,int stop,int output[],int *length);
void themdinh(Graph g,int id,char* name);//them mot dinh
int xoadinh(Graph g,int id);
int ketnoi(Graph g,int v1,int v2);//kiem tra xem co ket noi tu v1 den v2 ko
int in(Graph g,int v,int output[]);//tat ca cac dinh di den v
int out(Graph g,int v,int output[]);//tat ca cac dinh ma dinh v di den
int DAG(Graph g);//kiem tra xem do thi g co chu trinh hay ko dung DFS
void hienthi(Graph g);
void hienthidinh(Graph g);
void xoatatca(Graph g);
int xoaketnoi(Graph g,int v1,int v2);//tra ve 1 neu ket noi da xoa tra ve 0 neu ko co ket noi
void xoanutrong(Graph g);
void xoamotdinh(Graph g,int v);
int topology(Graph g,int ra[]);
int BFS(Graph g,int start,int stpop,int ra[]);//ham duyet theo chieu rong dung queue
int DFS(Graph g,int start,int stop,int ra[]);//duyet theo chieu sau dung stack
int BFS1(Graph g,int start,int stop,int ra[]);//duong di ngan nhat giua 2 dinh
//mang2 *duongdi(Graph g,int start,int stop);
void duongdi_a(Graph g,mang2 *a,int v1,int v2,int stop);//tat ca duong di xuat phat tu dinh v1
mang2 *duongdi(Graph g,int start,int stop);//tat ca duong di giua hai dinh(neu dinh stop la -1 thi la tat ca duong di xuat phat
    //tu dinh start )
mang2 *duongdi2(Graph g,int stop);//tat ca duong di ket thuc tai dinh stop(dinh bat dau la dinh khong co dinh nao ket noi voi no)   
void docfile(Graph g,FILE *f);    
void All_node_visited_a(Graph g,int v,mang1 *a,int stop);
mang1* All_node_visited(Graph g,int v);//tat ca cac node phai tham truoc khi tham node v

//*************************************************************************************************
main(){
   Graph g;
   JRB node,res,res2;
   int c;
   char d;
   FILE *f;
   char *name;
   char *name2;
   int v,v1,v2,n,res1,i,k,j;
   mang2 *b;
   mang1 *a;
   int *output;
   int visited[MAX];
   g=create(); 
    f=fopen("data.txt", "r");
    if(f==NULL){
        printf("Khong mo duoc file data2.txt\n");
        exit(1);
    }
    else docfile(g,f);
    do{
        c=menu();
        switch(c){
            case 1 :
            i=0;
            jrb_traverse(node,g.dinh){
                i++;
            }
            //hienthidinh(g);
            //hienthi(g);
            printf("Tong so nut co trong do thi la:%d\n",i);
            i=0;
            jrb_traverse(node,g.dinh){
                v=jval_i(node->key);
                output=(int*)malloc(sizeof(int)*MAX);
                n=out(g,v,output);
                i=i+n;
                free(output);
            }
            printf("Tong so cung trong do thi la:%d\n",i);
            break;
            case 2:
            name=(char*)malloc(sizeof(char)*20);
            printf("Nhap ten cong viec:");
            fgets(name,20,stdin);
            name[strlen(name)-1]='\0';
            for(i=0;i<strlen(name);i++) name[i]=toupper(name[i]);
            v=laymadinh(g,name);
            if(v==-1){
                printf("Khong co cong viec \'%s\' trong chuong trinh.\n",name);
            }
            else{
                output=(int*)malloc(sizeof(int)*MAX);
                n=in(g,v,output);
                printf("So luong cong viec phai lam ngay truoc cong viec \'%s\' la:%d.\n",name,n);
                if(n!=0){
                    printf("danh sach cong viec do la:");
                    for(i=0;i<n;i++){
                        printf("%s\t",laytendinh(g,output[i]));
                    }
                    printf("\n");
                }
                free(output);
                a=(mang1*)malloc(sizeof(mang1));
                a=All_node_visited(g,v);
                printf("So luong tat ca cong viec phai lam ngay cong viec \'%s\' la:%d.\n",name,a->num);
                if(a->num!=0){
                    printf("Danh sach cong viec do la:");
                    for(i=a->num-1;i>=0;i--){
                        printf("%s\t",laytendinh(g,a->A[i]));
                    }
                    printf("\n");   
                }
                free(a);
            }      
            break;       
            case 3 :
            printf("Ban co muon thuc hien lai cong viec 2 khong(y/n):");scanf("%c",&d);
            d=tolower(d);
            if(d=='y'){
                while(getchar()!='\n');
                name=(char*)malloc(sizeof(char)*20);
                printf("Nhap ten cong viec:");
                fgets(name,20,stdin);
                name[strlen(name)-1]='\0';
                for(i=0;i<strlen(name);i++) name[i]=toupper(name[i]);
                v=laymadinh(g,name);
                if(v==-1){
                    printf("Khong co cong viec \'%s\' trong chuong trinh.\n",name);
                }
                else{
                    output=(int*)malloc(sizeof(int)*MAX);
                    n=in(g,v,output);
                    printf("So luong cong viec phai lam ngay truoc cong viec \'%s\' la:%d.\n",name,n);
                    if(n!=0){
                        printf("danh sach cong viec do la:");
                        for(i=0;i<n;i++){
                            printf("%s\t",laytendinh(g,output[i]));
                        }
                        printf("\n");
                    }
                    free(output);
                    a=(mang1*)malloc(sizeof(mang1));
                    a=All_node_visited(g,v);
                    printf("So luong tat ca cong viec phai lam ngay cong viec \'%s\' la:%d.\n",name,a->num);
                    if(a->num!=0){
                        printf("Danh sach cong viec do la:");
                        for(i=a->num-1;i>=0;i--){
                            printf("%s\t",laytendinh(g,a->A[i]));
                        }
                        printf("\n");   
                    }
                    free(a);
                }
            }
            else if(d=='n'){
                if(DAG(g)==1){
                    printf("Khong co thu tu sap xep cong viec theo trinh tu truoc sau la\n");
                    break;
                }
                printf("Thu tu sap xep cong viec theo trinh tu truoc sau la:");
                output=(int*)malloc(sizeof(int)*MAX);
                n=topology(g,output);
                for(i=0;i<n;i++) printf("\'%s\'\t",laytendinh(g,output[i]));
                printf("\n");
                free(output);
            }
            else printf("Ban nhap khong dung.\n");
            break;
            case 4 :
            if(DAG(g)==1){
                printf("Khong co thu tu sap xep cong viec theo trinh tu truoc sau la\n");
                break;
            }
            printf("Thu tu sap xep cong viec theo trinh tu truoc sau la:");
             output=(int*)malloc(sizeof(int)*MAX);
             n=topology(g,output);
             for(i=0;i<n;i++) printf("\'%s\'\t",laytendinh(g,output[i]));
             printf("\n");
             free(output);
            break;
            case 5:
                name=(char*)malloc(sizeof(char)*20);
                printf("Nhap ten cong viec:");
                fgets(name,20,stdin);
                name[strlen(name)-1]='\0';
                for(i=0;i<strlen(name);i++) name[i]=toupper(name[i]);
                v=laymadinh(g,name);
                if(v==-1){
                    printf("Dinh \'%s\' ko co trong do thi moi xem lai.\n",name);
                    break;
                }
                b=(mang2*)malloc(sizeof(mang2));
                b=duongdi2(g,v);
                if(b->total==0){
                    printf("thoi gian toi thieu de hoan thanh cong viec \'%s\' la:0 (ngay).\n",name);
                    break;
                }
                n=b->B[0].wei;
                for(i=1;i<b->total;i++){
                    //for(j=0;j<b->B[i].num;j++) printf("%s\t",laytendinh(g,b->B[i].A[j]));
                    //printf("\n");
                    //printf("Trong so:%d\n",b->B[i].wei);
                    if(b->B[i].wei<n) n=b->B[i].wei;
                }
                printf("thoi gian toi thieu de hoan thanh cong viec \'%s\' la:%d (ngay).\n",name,n);
            break;                
            case 6 : 
            if(DAG(g)==1){
                printf("Do thi topo khong hop le(co chu trinh).\n");
            }
            else printf("Do thi topo hop le(khong co chu trinh)\n");
            break;                                                                     
            case 7:
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
    printf("1.Chuc nang 1.\n2.Chuc nang 2.\n3.Chuc nang 3.\n4.Chuc nang 4.\n5.Chuc nang 5.");
    printf("\n6.Chuc nang 6.\n7.Thoat.\n");

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
   }while(c<1||c>7);
   return c;
}


//**************************************************************************************************

void docfile(Graph g,FILE *f){
    char line[300];
    char *name,*name2,*s;
    int v1,v2,i,wei;
    while(1){
        fgets(line,300, f);
        if(line[strlen(line) - 1]=='\n') line[strlen(line) - 1] = '\0';
        char *ptr;
        ptr = strtok(line, ":");
        name=(char*)malloc(sizeof(char)*20);
        strcpy(name,ptr);
        name[strlen(name)-1]='\0';//loai dau cach o cuoi tu
        
        ///*
        
        v1=laymadinh(g,name);
        if(v1==-1){
            //v1=taomadinh(g);
            v1=atoi(name+1);
            themdinh(g,v1,name);
        }
        //*/
        ptr=strtok(NULL, " ");
        //if(ptr==NULL)  printf("name=(%s)\n",name);
        while(ptr){
            name2=(char*)malloc(sizeof(char)*20);
            s=(char*)malloc(sizeof(char)*20);
            strcpy(s,ptr);
            for(i=0;i<strlen(s);i++){
                if(s[i]=='-') break;
                name2[i]=s[i];
            }
            name2[i]='\0';
            wei=atoi(s+i+1);
            //printf("name=(%s)\n",name);
            //printf("name2=(%s)\n",name2);
            //printf("wei=%d\n",wei);
            ///*
            v2=laymadinh(g,name2);
            if(v2==-1){
                //v2=taomadinh(g);
                v2=atoi(name2+1);
                themdinh(g,v2,name2);
            }
            i=themcanh(g,v2,v1,wei);
            //*/
            ptr=strtok(NULL, " ");
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
int xoadinh(Graph g,int id){
    JRB res;
    res=jrb_find_int(g.dinh,id);
    if(res==NULL) return 0;
    else{
        jrb_delete_node(res);
        return 1;
    }
}

char*laytendinh(Graph g,int id){
  JRB res;
  res=jrb_find_int(g.dinh,id);
  if(res==NULL) return " ";
  //else printf("ten cua dinh %d la:\'%s\'.\n",id,jval_s(res->val));
  return jval_s(res->val);
}


int themcanh(Graph g,int v1,int v2,int wei){
    int f,f2;
    JRB res,res2,tree;
  res=jrb_find_gte_int(g.canh, v1, &f);
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
    jrb_insert_int(g.canh,v1,new_jval_v(tree));
    jrb_insert_int(tree,v2,new_jval_i(wei));
    return 1;
  }
   
}

int trong_so(Graph  g,int v1,int v2){
    JRB node,tree,node2;
    if(ketnoi(g,v1,v2)==0) return 0;
    else{
        node=jrb_find_int(g.canh,v1);
        tree=(JRB)jval_v(node->val);
        node2=jrb_find_int(tree,v2);
        return jval_i(node2->val);
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
}

void xoatatca(Graph g){
  JRB b,c;
  jrb_traverse(b,g.canh){
     jrb_delete_node(b);     
  }
 jrb_traverse(c,g.dinh){
     jrb_delete_node(c);     
  }
}

void xoanutrong(Graph g){
  JRB b,res;
   int n=0,v;
   int output[50];
   jrb_traverse(b,g.canh){
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
  res=jrb_find_int(g.canh,v1);
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

void hienthidinh(Graph g){
   JRB b,c;
   printf("%-15s %-10s\n","Ten dinh","Ma dinh");
   jrb_traverse(b,g.dinh){
      printf("%-15s %-10d\n",jval_s(b->val),jval_i(b->key));
   }
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
   jrb_traverse(b,g.dinh){
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
          
void duongdi_a(Graph g,mang2 *a,int v1,int v2,int stop){
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
                for(i=0;i<n;i++) duongdi_a(g,a,v2,output[i],stop);
            }
            else{
                if(v2!=stop){
                    n=out(g,v2,output);
                    for(i=0;i<n;i++) duongdi_a(g,a,v2,output[i],stop);
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
                for(i=0;i<n;i++) duongdi_a(g,a,v2,output[i],stop);
                }
                else{
                    if(v2!=stop){
                        n=out(g,v2,output);
                        for(i=0;i<n;i++) duongdi_a(g,a,v2,output[i],stop);
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
        duongdi_a(g,a,start,output[i],stop);
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


mang2 *duongdi2(Graph g,int stop){
    mang2 *a;
    mang2 *b;
    int v,i,j,n;
    int output[MAX];
    JRB node;
    a=(mang2*)malloc(sizeof(mang2));
    a->total=0;
 
    jrb_traverse(node,g.dinh){
        v=jval_i(node->key);
        n=in(g,v,output);
        if(n==0 && v!=stop){
            //printf("v1=%d\n",v);
            //printf("v2=%d\n",stop);
            b=(mang2*)malloc(sizeof(mang2));
            b=duongdi(g,v,stop);
            //printf("Total=%d\n",b->total);
            if(b->total!=0){
                for(i=0;i<b->total;i++){
                    for(j=0;j<b->B[i].num;j++){
                        a->B[a->total].A[j]=b->B[i].A[j];
                    }
                    a->B[a->total].num=b->B[i].num;
                    a->B[a->total].wei=b->B[i].wei;
                    a->total++;

                }
            }
            free(b);
        }
    }
    return a;

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
        /*
        //kiem tra de quy
        for(i=0;i<n;i++){
            check=0;
            for(j=0;j<(*a).num;j++){
                if(output[i]==(*a).A[j] || output[i]==stop){
                   check=1;
                   break; 
                } 
            }//neu co moi de quy
            if(check==1){
                All_node_visited_a(g,output[i],a,stop);
            }
        }
        //*/
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