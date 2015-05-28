#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jrb.h"
#include "fields.h"
#include "dllist.h"
#define MAX 100           //so luong dinh toi da co trong do thi
#define MAX1 1000
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
int check_mang_nguoc(int A[],int n,int B[],int m);//kiem tra hai mang A,B co phai la mang nguoc cua nhau ko    
void docfile(Graph g,FILE *f);   
char create_char(int i); 
int check_cua_vao_ra(char *name);//kiem tra xem dinh co phai la cua vao,ra ko.
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
    int *output;
    int A[MAX][MAX];
    int visited[MAX];
    g=create(); 
    f=fopen("data.txt", "r");
    do{
        c=menu();
        switch(c){
     
            case 1 :
                if(f==NULL){
                    printf("Khong mo duoc file data2.txt\n");
                }
                else docfile(g,f);
                //hienthidinh(g);
                //hienthi(g);
            break;
            case 2:
                i=0;
                jrb_traverse(node,g.dinh){
                    i++;
                }
                //hienthidinh(g);
                //hienthi(g);
                printf("Tong so dinh co trong do thi la: %d\n",i);
                i=0;
                jrb_traverse(node,g.dinh){
                    v=jval_i(node->key);
                    output=(int*)malloc(sizeof(int)*MAX);
                    n=out(g,v,output);
                    i=i+n;
                    free(output);
                }
                printf("Tong so canh trong do thi la: %d\n",i/2);
            break;       
            case 3 :
                name=(char*)malloc(sizeof(char)*20);
                printf("Nhap ten dinh:");
                fgets(name,20,stdin);
                name[strlen(name)-1]='\0';
                v=laymadinh(g,name);
                output=(int*)malloc(sizeof(int)*MAX);
                if(v==-1){
                    printf("Dinh \'%s\' ko co trong do thi.\n",name);
                    break;
                }
                else{ 
                    n=out(g, v, output);
                    if(n==0){
                        printf("Khong co dinh nao ke voi dinh \'%s\'.\n",name);
                        break;
                    }else{
                        printf("So luong dinh ke voi dinh \'%s\' la: %d.\n",name,n);
                        printf("Cac dinh do la:");
                        for(i=0;i<n;i++) printf("%s\t",laytendinh(g,output[i]));
                        printf("\n");    
                    }
                }
                free(output);
            break;  
            case 4 :
                name=(char*)malloc(sizeof(char)*20);
                name2=(char*)malloc(sizeof(char)*20);
                printf("Nhap ten dinh dau:");
                fgets(name,20,stdin);
                name[strlen(name)-1]='\0';
                v1=laymadinh(g,name);
                if(v1==-1){
                    printf("Dinh \'%s\' ko co trong do thi moi xem lai.\n",name);
                    break;
                    }
                printf("Nhap ten dinh sau:");
                fgets(name2,20,stdin);
                name2[strlen(name2)-1]='\0';
                v2=laymadinh(g,name2);
                if(v2==-1){
                    printf("Dinh \'%s\' ko co trong do thi moi xem lai.\n",name2);
                    break;
                }
                if(v1==v2){
                    printf("Nhap sai.Hai dinh trung nhau.\n");
                    break;
                }
                output=(int*)malloc(sizeof(int)*MAX);
                n=BFS1(g,v1,v2,output);
                if(n==0){
                    printf("khong co duong di tu \'%s\' den \'%s\'.\n",name,name2);
                    break;
                }
                printf("duong di ngan nhat tu dinh \'%s\' den dinh \'%s\' la: ",name,name2);
                for(i=0;i<n;i++) printf("%s\t",laytendinh(g,output[i]));
                printf("\n");
                free(output);
            break;
            case 5:
                name=(char*)malloc(sizeof(char)*20);
                printf("Nhap cua vao:");
                fgets(name,20,stdin);
                name[strlen(name)-1]='\0';
                v=laymadinh(g,name);
                if(v==-1){
                    printf("Dinh \'%s\' ko co trong do thi moi xem lai.\n",name);
                    break;
                }
                check=check_cua_vao_ra(name);
                if(check==1){
                    b=(mang2*)malloc(sizeof(mang2));
                    b->total=0;
                    jrb_traverse(node,g.dinh){
                        v2=jval_i(node->key);
                        if(v2!=v){
                            name2=(char*)malloc(sizeof(char)*20);
                            name2=laytendinh(g,v2);
                            check1=check_cua_vao_ra(name2);
                            if(check1==1){
                                b1=(mang2*)malloc(sizeof(mang2));
                                b1=duongdi(g,v,v2);
                                if(b1->total!=0){
                                    for(i=0;i<b1->total;i++){
                                        for(j=0;j<b1->B[i].num;j++){
                                            b->B[b->total].A[j]=b1->B[i].A[j];
                                        }
                                        b->B[b->total].num=b1->B[i].num;
                                        b->total++;
                                    }
                                }
                                free(b1);
                            }
                            //free(name2);
                        }
                        
                    }
                    if(b->total==0) printf("Khong co duong di giup thoat khoi me cung tu cua vao \'%s\'.\n",name);
                    else{
                        //printf("Ta ca duong di giup thoat khoi me cung tu cua vao \'%s\' la:%d\n",name,b->total);
                        //printf("cac duong di do la:\n");
                        min=b->B[0].num;
                        k=0;
                        for(i=0;i<b->total;i++){
                            if(b->B[i].num<min){
                                min=b->B[i].num;
                                k=i;
                            }
                            //for(j=0;j<b->B[i].num;j++) printf("\'%s\'\t",laytendinh(g,b->B[i].A[j]));
                            //printf("\n");
                        }
                        printf("Duong di ngan nhat giup thoat khoi me cung tu cua vao \'%s\' la: ",name );
                        for(j=0;j<b->B[k].num;j++) printf("\'%s\'\t",laytendinh(g,b->B[k].A[j]));
                        printf("\n");    
                    }
                    free(b);
                }
                else printf("Dinh \'%s\' ton tai trong do thi nhung khong phai la cua vao.\n",name);
                //free(name);

            break;      
            case 6 :
                b=(mang2*)malloc(sizeof(mang2));
                b->total=0;
                //k=0;
                jrb_traverse(node,g.dinh){
                    v1=jval_i(node->key);
                    b1=(mang2*)malloc(sizeof(mang2));
                    b1=duongdi(g,v1,-1);
                    if(b1->total!=0){
                        k=0;
                        max=b1->B[0].num;
                        for(i=0;i<b1->total;i++){
                            if(b1->B[i].num>max){
                                max=b1->B[i].num;
                                k=i;
                            }
                            /*
                            for(j=0;j<b1->B[i].num;j++){
                                printf("%s\t",laytendinh(g,b1->B[i].A[j]));
                            }
                            printf("\n");
                            */
                        }
                        //printf("n=%d\n",b1->total);
                        //getchar();
                        for(j=0;j<b1->B[k].num;j++){
                            b->B[b->total].A[j]=b1->B[k].A[j];
                        }
                        b->B[b->total].num=b1->B[k].num;
                        //printf("Duong di lon nhat:\n");
                        //for(j=0;j<b->B[b->total].num;j++) printf("%s\t",laytendinh(g,b->B[b->total].A[j]));
                        //printf("\n");
                        b->total++;
                        //printf("Total=%d\n",b->total);
                        //getchar();
                        free(b1);
                    }

                }
                //printf("n=%d\n",k);
                ///*
                //printf("So luong tat ca duong di dai nhat giua hai dinh la: %d\n",b->total);
                //printf("cac duong di do la:\n");
                max=b->B[0].num;
                k=0;
                for(i=0;i<b->total;i++){
                    if(b->B[i].num>max){
                        max=b->B[i].num;
                        k=i;
                    }
                    //for(j=0;j<b->B[i].num;j++) printf("%s\t",laytendinh(g,b->B[i].A[j]));
                    //printf("\n");
                }
                printf("Thanh phan lien thong lon nhat trong do thi la: \n");
                for(j=0;j<b->B[k].num;j++) printf("%s\t",laytendinh(g,b->B[k].A[j]));
                printf("\n");
                //*/
                free(b);
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
    printf("1.Doc ma tran me cung.\n2.Tong so dinh va tong so canh.\n3.Cac dinh ke.\n4.Duong di ngan nhat giua 2 dinh.\n");
    printf("5.Duong di thoat khoi me cung.\n6.Thanh phan lien thong lon nhat.\n7.Thoat.\n");
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
    char line[100];
    int v1,v2,i,j,n,k;
    char *name;
    char *name2;
    int A[SIZE][SIZE];
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
            A[i][j]=atoi(ptr);
            ptr = strtok(NULL, " ");
            j++;
        }
        if(feof(f)==1) break;
        i++;
    }
    n=i+1;
    size_matrix=n;
    printf("Ma Tran me cung la:\n");
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            printf("%d ",A[i][j]);
            if(A[i][j]==0){
                name=(char*)malloc(sizeof(char)*3);
                name[0]=create_char(i);
                name[1]=create_char(j);
                name[2]='\0';
                v1=laymadinh(g,name);
                //printf("v1=%d\n",v1);
                if(v1==-1){
                    v1=taomadinh(g);
                    themdinh(g,v1,name);
                } 
                //printf("name =(%s)\n",name);
                //printf("v1=%d\n",v1);
                //duoi
                k=i+1;
                if(k>=0 && k<n){
                    if(A[k][j]==0){
                        name2=(char*)malloc(sizeof(char)*3); 
                        name2[0]=create_char(k);
                        name2[1]=create_char(j);
                        name2[2]='\0';
                        
                        v2=laymadinh(g,name2);
                        //printf("v2=%d\n",v2);
                        if(v2==-1){
                            v2=taomadinh(g);
                            themdinh(g,v2,name2);
                        }
                        themcanh(g,v1,v2);
                        themcanh(g,v2,v1);
                        //printf("name2=(%s)\n",name2);
                        //printf("v2=%d\n",v2);
                        //free(name2);

                    }
                }
                //tren
                k=i-1;
                if(k>=0 && k<n){
                    if(A[k][j]==0){
                        name2=(char*)malloc(sizeof(char)*3); 
                        name2[0]=create_char(k);
                        name2[1]=create_char(j);
                        name2[2]='\0';
                        v2=laymadinh(g,name2);
                        //printf("v2=%d\n",v2);
                        if(v2==-1){
                            v2=taomadinh(g);
                            themdinh(g,v2,name2);
                        }
                        themcanh(g,v1,v2);
                        themcanh(g,v2,v1);
                        //printf("name2=(%s)\n",name2);
                        //printf("v2=%d\n",v2);
                       // free(name2);

                    }
                }
                //trai
                k=j-1;
                if(k>=0 && k<n){
                    if(A[i][k]==0){
                        name2=(char*)malloc(sizeof(char)*3); 
                        name2[0]=create_char(i);
                        name2[1]=create_char(k);
                        name2[2]='\0';
                        
                        v2=laymadinh(g,name2);
                        //printf("v2=%d\n",v2);
                        if(v2==-1){
                            v2=taomadinh(g);
                            themdinh(g,v2,name2);
                        }
                        themcanh(g,v1,v2);
                        themcanh(g,v2,v1);
                        //printf("name2=(%s)\n",name2);
                        //printf("v2=%d\n",v2);
                       // free(name2);

                    }
                }
                //phai
                k=j+1;
                if(k>=0 && k<n){
                    if(A[i][k]==0){
                        name2=(char*)malloc(sizeof(char)*3); 
                        name2[0]=create_char(i);
                        name2[1]=create_char(k);
                        name2[2]='\0';

                        v2=laymadinh(g,name2);
                        //printf("v2=%d\n",v2);
                        if(v2==-1){
                            v2=taomadinh(g);
                            themdinh(g,v2,name2);
                        }
                        themcanh(g,v1,v2);
                        themcanh(g,v2,v1);
                        //printf("name2=(%s)\n",name2);
                        //printf("v2=%d\n",v2);
                        //free(name2);

                    }
                }
            }
        }
        printf("\n");                   
    }   
    fclose(f);
}

char create_char(int i){

    switch(i){
        case 0:return '0';
        case 1:return '1';
        case 2:return '2';
        case 3:return '3';
        case 4:return '4';
        case 5:return '5';
        case 6:return '6';
        case 7:return '7';
        case 8:return '8';
        case 9:return '9';
        default:return '*';
        break;
    }
}

int check_cua_vao_ra(char *name){
    int k,i,j;
    k=atoi(name);
    //printf("k=%d\n",k);
    i=k/10;
    j=k%10;
    if(i==0 || i==size_matrix-1 || j==0 || j==size_matrix-1){
        return 1;
    }else return 0;

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