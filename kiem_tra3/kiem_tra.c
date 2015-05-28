#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jrb.h"
#include "fields.h"
#include "dllist.h"
#define MAX 100           //so luong dinh toi da co trong do thi
#define MAX1 1000          //so luong duong di toi da tren do thi
#define SIZE 10            //kich thuc cua ma tran vao(bat quai,me cung)

typedef JRB Graph;

typedef struct{
    int A[MAX];
    int num;
    float wei;
}mang1;

typedef struct{
    mang1 B[MAX1]; 
    int total;   
}mang2;

typedef struct
{     
    char name[MAX];
    char sort_name[10];
}Tinh;

Tinh NODE[MAX];
int total=0;

//********************************************************************************************
Graph create();
//tao Graph
int themdinh(Graph g,int v);
int themcanh(Graph g,int v1,int v2,float wei);//them mot canh tu v1 den v2
int xoadinh(Graph g,int id);
float trong_so(Graph g,int v1,int v2);
int layid(char* sort_name);//lay ma dinh tu ma tinh
int layid2(char *name);//lay ma dinh tu ten dinh
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
//mang2 *duongdi(Graph g,int start,int stop);//tat ca duong di giua hai dinh
void docfile(Graph g,FILE *f);
void duongdi_a(Graph g,mang2 *a,int v1,int v2,int stop);//tat ca duong di xuat phat tu dinh v1
mang2 *duongdi(Graph g,int start,int stop);//tat ca duong di giua hai dinh(neu dinh stop la -1 thi la tat ca duong di xuat phat
    //tu dinh start)
void All_node_visited_a(Graph g,int v,mang1 *a,int stop);
mang1* All_node_visited(Graph g,int v);//tat ca cac node phai den truoc khi tham node v  
void All_node_visit_a(Graph g,int v,mang1 *a,int stop);
mang1* All_node_visit(Graph g,int v);//tat ca cac node ma tu node v co the den duoc  
mang2 *phan_cap_node(Graph g,int v);//chia bac(bac 1,2,3,...) tat ca cac node ma node v co the den duoc(coi node v la goc co bac la 0)  
//*************************************************************************************************
main(){
    Graph g;
    FILE *f;
    JRB node,node2,res,res2;
    int c;
    char d;
    char *name;
    char *name2;
    int v,v1,v2,n,i,k,j,count,check,check1,max;
    mang2 *b,*b1;
    mang1 *a;
    int *output;
    int A[MAX][MAX];
    int visited[MAX];
    float gia_ve[MAX];
    float wei,min;
    char filename[30];
    g=create(); 
    
   do{
        c=menu();
         switch(c){
     
            case 1 :
                printf("Nhap ten file:");gets(filename);
                f=fopen(filename, "r");
                if(f==NULL){
                    printf("Khong mo duoc file %s\n",filename);
                    break;
                }
                else docfile(g,f);
                /*
                printf("Cac dinh co trong do thi la:\n");
                for(i=0;i<total;i++){
                    printf("%-5d%-5s%-20s\n",i,NODE[i].sort_name,NODE[i].name);
                }
                //*/
                for(i=0;i<MAX;i++){
                    for(j=0;j<MAX;j++) A[i][j]=0;
                }
                count=0;
                jrb_traverse(node,g){
                    v=jval_i(node->key);
                    output=(int*)malloc(sizeof(int)*MAX);
                    n=out(g, v, output);
                    for(i=0;i<n;i++){
                        wei=trong_so(g,v,output[i]);
                        if(count==0){
                             gia_ve[count]=wei;
                             count++;
                        }
                        else{
                            check=0;
                            for(j=0;j<count;j++){
                                if(wei==gia_ve[j]){
                                    check=1;
                                    break;
                                }
                            }
                            if(check==0){
                                gia_ve[count]=wei;
                                count++;
                            }
                        }
                    }
                    free(output);
                }
                //printf("count=%d\n",count );
                for(i=0;i<count-1;i++){
                    for(j=i+1;j<count;j++){
                        if(gia_ve[i]>gia_ve[j]){
                            wei=gia_ve[i];
                            gia_ve[i]=gia_ve[j];
                            gia_ve[j]=wei;
                        }
                    }
                }
                for(i=0;i<count;i++){
                    printf("Chuyen bay co gia ve la %.1f trieu dong la:\n", gia_ve[i]);
                    jrb_traverse(node,g){
                        v=jval_i(node->key);
                        output=(int*)malloc(sizeof(int)*MAX);
                        n=out(g, v, output);
                        for(j=0;j<n;j++){
                            wei=trong_so(g,v,output[j]);
                            if(wei==gia_ve[i]){
                                if(A[v][output[j]]==0){
                                    printf("%s => %s.\n",NODE[v].name,NODE[output[j]].name);
                                    A[v][output[j]]=1;
                                    A[output[j]][v]=1;
                                }
                                
                            }
                        }
                        free(output);
                       
                    }
                    printf("\n");
                }
            break;
            case 2: 
                printf("So diem bay cua hang hang khong la:%d\n",total);
                i=0;
                jrb_traverse(node,g){
                    v=jval_i(node->key);
                    output=(int*)malloc(sizeof(int)*MAX);
                    n=out(g,v,output);
                    i=i+n;
                    free(output);
                }
                printf("So chuyen bay cua hang hang khong la:%d\n",i/2);
            break; 
            case 3:
                name=(char*)malloc(sizeof(char)*30);
                printf("Ten tinh:");gets(name);
                v=layid2(name);
                if(v==-1) printf("Tinh \'%s\' khong phai la diem bay cua hang hang khong.\n",name);
                else{
                    printf("Cac tinh co chuyen bay thang den tinh \'%s\' la: \n",name);
                    output=(int*)malloc(sizeof(int)*MAX);
                    n=out(g,v,output);
                    for(i=0;i<n;i++) printf("%s\n",NODE[output[i]].name);

                }
            break;      
            case 4 :
                max=0;
                jrb_traverse(node,g){
                    v=jval_i(node->key);
                    output=(int*)malloc(sizeof(int)*MAX);
                    n=out(g,v,output);
                    if(n>max) max=n;
                    free(output);
                }
                printf("Danh sach cac tinh co so luong duong bay nhieu nhat la:\n");
                jrb_traverse(node,g){
                    v=jval_i(node->key);
                    output=(int*)malloc(sizeof(int)*MAX);
                    n=out(g,v,output);
                    if(n==max) printf("%s\n",NODE[v].name);
                    free(output);
                }
            break;
            case 5:
                printf("Nhap ma tinh dau:");
                name=(char*)malloc(sizeof(char)*30);
                gets(name);
                v1=layid(name);
                if(v1==-1){
                    printf("Tinh co ma \'%s\' khong phai la diem bay cua hang hang khong.\n",name);
                    break;
                }
                printf("Nhap ma tinh cuoi:");
                name2=(char*)malloc(sizeof(char)*30);
                gets(name2);
                v2=layid(name2);
                if(v2==-1){
                    printf("Tinh co ma\'%s\' khong phai la diem bay cua hang hang khong.\n",name2);
                    break;
                }
                if(v1==v2){
                    printf("Nhap sai.Hai tinh trung nhau.\n");
                    break;
                }
                b=(mang2*)malloc(sizeof(mang2));
                b=duongdi(g,v1,v2);
                if(b->total==0) printf("Khong co hanh trinh bay tu tinh \'%s\' den tinh \'%s\'.\n",NODE[v1].name,NODE[v2].name);
                else{
                    printf("Mot hanh trinh bay tu tinh \'%s\' den tinh \'%s\' la:\n",NODE[v1].name,NODE[v2].name);
                    for(i=0;i<b->B[0].num;i++){
                        if(i==b->B[0].num-1) printf("%s.\n",NODE[b->B[0].A[i]].name);
                        else printf("%s => ",NODE[b->B[0].A[i]].name);
                    } 
                }
                free(b);
            break;    
            case 6 :
                printf("Nhap ma tinh:");
                name=(char*)malloc(sizeof(char)*30);
                gets(name);
                v=layid(name);
                if(v==-1){
                    printf("Tinh co ma\'%s\' khong phai la diem bay cua hang hang khong.\n",name);
                    break;
                }
                //printf("v=%d\n",v);
                //b=(mang2*)malloc(sizeof(mang2));
                //b->total=0;
                output=(int*)malloc(sizeof(int)*MAX);
                n=DFS(g,v,-1,output);
                printf("Thanh phan lien thong lon nhat chua dinh \'%s\' la:\n",NODE[v].name);
                for(i=0;i<n;i++){
                    if(i==n-1) printf("%s.\n",NODE[output[i]].name);
                    else printf("%s, ",NODE[output[i]].name );
                }
                printf("So luong cac dinh co trong do la: %d\n",n);
                free(output);
            break;
            case 7 : 
                printf("Nhap ma tinh dau:");
                name=(char*)malloc(sizeof(char)*30);
                gets(name);
                v1=layid(name);
                if(v1==-1){
                    printf("Tinh co ma \'%s\' khong phai la diem bay cua hang hang khong.\n",name);
                    break;
                }
                printf("Nhap ma tinh cuoi:");
                name2=(char*)malloc(sizeof(char)*30);
                gets(name2);
                v2=layid(name2);
                if(v2==-1){
                    printf("Tinh co ma\'%s\' khong phai la diem bay cua hang hang khong.\n",name2);
                    break;
                }
                if(v1==v2){
                    printf("Nhap sai.Hai tinh trung nhau.\n");
                    break;
                }
                b=(mang2*)malloc(sizeof(mang2));
                b=duongdi(g,v1,v2);
                if(b->total==0) printf("Khong co hanh trinh bay tu tinh \'%s\' den tinh \'%s\'.\n",NODE[v1].name,NODE[v2].name);
                else{
                    for(i=0;i<b->total;i++){
                        for(j=0;j<b->B[i].num;j++){
                           // if(j==b->B[i].num-1) printf("%s.",NODE[b->B[i].A[j]].name);
                            //else printf("%s => ",NODE[b->B[i].A[j]].name);
                        }
                        //printf("wei=%.1f\n",b->B[i].wei);
                    }
                    k=1000;
                    min=1000.0;
                    for(i=0;i<b->total;i++){
                        if(b->B[i].wei<min) min=b->B[i].wei;
                    }
                    for(i=0;i<b->total;i++){
                        if(b->B[i].wei==min){
                            if(b->B[i].num<k) k=b->B[i].num;
                        }
                    }
                   // printf("min=%.1f,k=%d\n",min,k);
                    printf("Chuyen bay tu tinh \'%s\' toi tinh \'%s\' voi gia ve nho nhat va toi uu la:\n",NODE[v1].name,NODE[v2].name);
                    for(i=0;i<b->total;i++){
                        if(b->B[i].wei==min && b->B[i].num==k){
                            for(j=0;j<b->B[i].num;j++){
                                if(j==b->B[i].num-1) printf("%s.",NODE[b->B[i].A[j]].name);
                                else printf("%s => ",NODE[b->B[i].A[j]].name);
                            } 
                             break;
                        }
                       
                    }
                }
                free(b); 
            break;     
                                                          
            case 8:printf("Ban co muon thoat khoi chuong trinh khong(y/n):");scanf("%c",&d);
                d=tolower(d);
                if(d=='y') c=100;
                break;  
        }
    }while(c!=100);
}

int menu(){
    int c,i;
    char s[100];
    printf("\n*********************************MENU**************************************\n");
    printf("1.Doc file va thong tin chuyen bay co cung gia ve.\n2.So diem bay va so chuyen bay.\n");
    printf("3.Ten cac tinh co chuyen bay truc tiep den mot tinh.\n4.Danh sach cac tinh co so duong bay la nhieu nhat.\n");
    printf("5.Hanh trinh bay giua hai tinh.\n6.Thanh phan lien thong lon nhat chua tinh-so dinh.\n");
    printf("7.Lo trinh bay re nhat va toi uu.\n");
    printf("8.thoat.\n");
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
   }while(c<1||c>8);
   return c;
}


//**************************************************************************************************

void docfile(Graph g,FILE *f){
    char line[300];
    char *name,*name2;
    int v1,v2,i,j,v;
    Tinh *tinh;
    float wei;
    while(1){
        fgets(line,300, f);
         if(feof(f)==1) break;
        if(line[strlen(line) - 1]=='\n') line[strlen(line) - 1] = '\0';
        char *ptr;
        ptr = strtok(line, "=");
        //printf("name=(%s)\n",ptr);
        
        if(strlen(ptr)!=3){
            ptr=strtok(line," ");
            name=(char*)malloc(sizeof(char)*30);
            strcpy(name,ptr);
            v1=layid(name);
            //printf("name=(%s),",ptr);
            ptr=strtok(NULL," ");
            name2=(char*)malloc(sizeof(char)*30);
            strcpy(name2,ptr);
            v2=layid(name2);
            //printf("name2=(%s),",ptr);
            ptr=strtok(NULL," ");
            wei=atof(ptr);
            themcanh(g,v1,v2,wei);
            themcanh(g,v2,v1,wei);
            //printf("wei=%.1f\n",wei);
        }
        else{
            v=layid(ptr);
            if(v==-1){
                themdinh(g,total);
                //tinh[total].number=total;
                strcpy(NODE[total].sort_name,ptr);
                name=(char*)malloc(sizeof(char)*30);
                strcpy(name,ptr+strlen(ptr)+1);
                name2=(char*)malloc(sizeof(char)*30);
                for(i=0;i<strlen(name)-2;i++) name2[i]=name[i+1];  
                strcpy(NODE[total].name,name2);
                total++;
            }
            

        }
        
       
    }
}

Graph create(){
  Graph g;
  g=make_jrb();
  return g;
}

char *layten(int v){
    return NODE[v].name;
}

int layid(char *sort_name){
    int i;
    for(i=0;i<total;i++){
        if(strcmp(NODE[i].sort_name,sort_name)==0) return i;
    }
    return -1;
}

int layid2(char *name){
    int i;
    for(i=0;i<total;i++){
        if(strcmp(NODE[i].name,name)==0) return i;
    }
    return -1;
}

float trong_so(Graph g,int v1,int v2){
    JRB node,tree,node2;
    if(ketnoi(g,v1,v2)==0) return 0;
    else{
        node=jrb_find_int(g,v1);
        tree=(JRB)jval_v(node->val);
        node2=jrb_find_int(tree,v2);
        return jval_f(node2->val);
    }
}

int themdinh(Graph g,int v){
    JRB node,tree;
    node=jrb_find_int(g,v);
    if(node==NULL){
        tree=make_jrb();
        jrb_insert_int(g,v,new_jval_v(tree));
        return 1;
    }
    return 0;
}

int xoadinh(Graph g,int v){
    JRB node,tree;
    node=jrb_find_int(g,v);
    if(node==NULL){
        return 0;
    }
    else{
        jrb_delete_node(node);
        return 1;
    }
}
int themcanh(Graph g,int v1,int v2,float wei){
    int f,f2;
    JRB res,res2,tree;
    res=jrb_find_gte_int(g, v1, &f);
    if(f==1){
        tree=(JRB)jval_v(res->val);
        res2=jrb_find_gte_int(tree, v2, &f2);
        if(f2==0){
            jrb_insert_int(tree,v2,new_jval_f(wei));
            return 1;
        } 
        else{ return 0;}
    }else{
        tree=make_jrb();
        jrb_insert_int(g,v1,new_jval_v(tree));
        jrb_insert_int(tree,v2,new_jval_f(wei));
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
    n=n+1;}
  }
  return n;

}

int out(Graph g,int v,int output[]){
  JRB res,b;
  int f,n,x;
  n=0;
  jrb_traverse(b,g){
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
 
    jrb_traverse(node,g){
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
    }
    
    return a;
}

mang2 *phan_cap_node(Graph g,int v){
    //b->B[x].A[] trong do x lap cap cua node,mang A chua cac node co cap la x (khong co node co cap la 0)
    mang1 *a;
    mang2 *b;
    int *output;
    int cap_node[MAX];
    int check,check1,i,j,k,n,min,max;
    b=(mang2*)malloc(sizeof(mang2));
    b->total=0;
    a=(mang1*)malloc(sizeof(mang1));
    a=All_node_visit(g,v);
    printf("Tong so tat ca cac node ma node %d co the di den la: %d.\n",v,a->num);
    if(a->num!=0){
        for(i=0;i<a->num;i++) printf("%d\t",a->A[i]);
        printf("\n");    
        for(i=0;i<MAX;i++) cap_node[i]=0;
        output=(int*)malloc(sizeof(int)*MAX);
        n=out(g,v,output);
        for(i=0;i<n;i++) cap_node[output[i]]=1; 
        free(output);    
        while(1){
            check=1;
            for(i=0;i<a->num;i++){
                if(cap_node[a->A[i]]==0){
                    check=0;//co mot nguoi chua co cap ban be
                    break;
                }
            }
            if(check==1) break;//tat ca deu co cap ban be thi dung lai.
            for(i=0;i<a->num;i++){
                if(cap_node[a->A[i]]==0){
                    output=(int*)malloc(sizeof(int)*MAX);
                    n=in(g,a->A[i],output);
                    min=100;
                    if(n!=0){
                        for(j=0;j<n;j++){
                            if(cap_node[output[j]]!=0){
                                if(cap_node[output[j]]<min) min=cap_node[output[j]];
                            }  
                        }
                        if(min!=100) cap_node[a->A[i]]=min+1;
                                    
                    }
                    free(output);
                }
            } 
        }
        //printf("3\n");
        for(i=0;i<MAX1;i++) b->B[i].num=0;
        for(i=0;i<a->num;i++){
            if(cap_node[a->A[i]] > b->total) b->total=cap_node[a->A[i]];
            b->B[cap_node[a->A[i]]-1].A[b->B[cap_node[a->A[i]]-1].num]=a->A[i];
            b->B[cap_node[a->A[i]]-1].num++;
        }        
    }
    free(a);
    return b;
}