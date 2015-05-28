#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jrb.h"
#include "fields.h"
#include "dllist.h"
#define max 100           //so luong dinh toi da co trong do thi
#define max1 30          //so luong duong di lon nhat giua hai dinh

typedef struct{
  JRB dinh;
  JRB canh;
}Graph;

typedef struct{
   int A[max1];
   int num;
}mang1;

typedef struct{
   mang1 B[max];   //tinh tu B[1] (ko tinh B[0])
   int total;   
}mang2;

//********************************************************************************************
Graph create();
//tao Graph

int taomadinh(Graph g);
int laymadinh(Graph g,char* name);
char*laytendinh(Graph g,int id);//lay ra ten dinh qua ma so
int themcanh(Graph g,int v1,int v2);//them mot canh tu v1 den v2
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
mang2 *duongdi(Graph g,int start,int stop);
void docfile(Graph g,FILE *f);
//*************************************************************************************************
main(){
   Graph g;
   FILE *f;
   JRB node,res,res2;
   int c;
   char d;
   char *name;
   char *name2;
   int v,v1,v2,n,res1,i,k,j;
   mang2 *b;
   int *output;
   int visited[max];
   g=create(); 
   char filename[30];
   // printf("nhap ten file:");
   // fgets(filename,30,stdin);
    //filename[strlen(filename)-1]='\0';
    //f=fopen(filename, "r");
   f=fopen("monhoc.txt", "r");
    if(f==NULL){
        printf("Khong mo duoc file %s\n",filename);
        exit(1);
    }
    docfile(g,f); 
  
   do{
        c=menu();
        switch(c){
     
            case 1:
                printf("Cac dinh co trong do thi la:\n");
                hienthidinh(g);
            break;
            case 2: if(DAG(g)==1) printf("do thi co chu trinh.\n");
                else printf("do thi ko co chu trinh.\n");
            break;
            case 3: //xoanutrong(g);            
                hienthi(g);
            break;    
            case 4:
                if(DAG(g)==1){
                    printf("Khong the sap xep theo topology vi do thi co chu trinh\n");
                    break;
                }
                printf("sap sep theo kieu topology la:");
                output=(int*)malloc(sizeof(int)*max);
                k=topology(g,output);
                for(i=0;i<k;i++) printf("%s\t",laytendinh(g,output[i]));
                printf("\n");
                free(output);
            break;    
            case 5:
                k=0;
                j=0;
                jrb_traverse(node,g.dinh){
                    k++;
                }
                jrb_traverse(node,g.dinh){
                    output=(int*)malloc(sizeof(int)*max);
                    n=DFS(g,jval_i(node->key),-1,output);
                    if(n<k){
                        printf("Do thi khong lien thong\n");
                        //free(output);
                        j=1;
                        break;
                    }
                    //free(output);
                }
                if(j==0) printf("Do thi lien thong.\n");
            break;
            case 6:name=(char*)malloc(sizeof(char)*20);
                printf("Nhap ten dinh bat ky:");
                fgets(name,20,stdin);
                name[strlen(name)-1]='\0';
                v=laymadinh(g,name);
                if(v==-1){
                    printf("Dinh \'%s\' ko co trong do thi moi xem lai.\n",name);
                    break;
                }
                for(i=0;i<max;i++) visited[i]=0;
                    printf("Cac thanh phan ket noi cua do thi la:\n");
                    output=(int*)malloc(sizeof(int)*max);
                    n=DFS(g,v,-1,output);
                    for(i=0;i<n;i++){
                        printf("%s\t",laytendinh(g,output[i]));
                        visited[output[i]]=1;
                    }
                    //free(output);
                    printf("\n");
                    jrb_traverse(node,g.dinh){
                        k=jval_i(node->key);
                        if(visited[k]==0){
                            output=(int*)malloc(sizeof(int)*max);
                            n=DFS(g,k,-1,output);
                            for(i=0;i<n;i++){
                                printf("%s\t",laytendinh(g,output[i]));
                                visited[output[i]]=1;
                            }
                            printf("\n");
                            //free(output);
                        }
                    }
                    // free(name);

            break;                                                                
     case 20:printf("Ban co muon thoat khoi chuong trinh khong(y/n):");scanf("%c",&d);
            d=tolower(d);
             if(d=='y') c=21;
             break;  
     }
   }while(c!=21);
}


int menu(){
   int c,i;
   char s[100];
   printf("\n**********************menu**********************\n");
   printf("1.hien thi cac dinh co trong do thi.\n2.Kiem tra dinh co chu trinh ko.\n3.Hien thi tat ca cac ket noi.\n4.sap xep theo thuat toan topology.\n5.Tinh lien thong cua do thi.\n6.Tim cac thanh phan ket noi cua do thi.\n20.thoat.\n");
   printf("************************************************\n");
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
   }while(c<1||c>20);
   return c;
}


//**************************************************************************************************

void docfile(Graph g,FILE *f){
    char line[300];
    char *name,*name2;
    int v1,v2,i;
    while(1){
        fgets(line,300, f);
        if(line[strlen(line) - 1]=='\n') line[strlen(line) - 1] = '\0';
        char *ptr;
        ptr = strtok(line, ":");
        name=(char*)malloc(sizeof(char)*20);
        strcpy(name,ptr);
        name[strlen(name)-1]='\0';//loai dau cach o cuoi tu
        //printf("name=(%s)\n",name);
        v1=laymadinh(g,name);
        if(v1==-1){
            v1=taomadinh(g);
            themdinh(g,v1,name);
        }
        ptr=strtok(NULL, " ");
        while(ptr){
            name2=(char*)malloc(sizeof(char)*20);
            strcpy(name2,ptr);
            //printf("name2=(%s)\n",name2);
            
            v2=laymadinh(g,name2);
            if(v2==-1){
                v2=taomadinh(g);
                themdinh(g,v2,name2);
            }
            i=themcanh(g,v1,v2);
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
  //if(res==NULL){printf("ko co dinh %d trong do thi.\n",id);return;}
  //else printf("ten cua dinh %d la:%s.\n",id,jval_s(res->val));
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
      //printf("tu dinh %s co the di den cac dinh:",laytendinh(g,jval_i(b->key)));
      c=(JRB)jval_v(b->val);
      jrb_traverse(d,c){
         printf("%s => %s\n",laytendinh(g,jval_i(b->key)),laytendinh(g,jval_i(d->key)));}
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
  int *C,D;
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
              //them duong di vao mang
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
          


