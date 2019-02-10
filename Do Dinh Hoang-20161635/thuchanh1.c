#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
typedef struct line{
    int data;
    struct line *next;
}line;
typedef struct list
{
    char word[30];
    line *first;
    struct list *left;
    struct list *right; 
}list;
//list:DSLK doi,line:DSLK don(khong bi gioi han van ban)
list *root;
line *addLine(line *first,int l)
{
    line *p=first,*p1;
    line *temp = (line *)malloc(sizeof(line));
    temp->data=l;
    temp->next=NULL;
    if(p==NULL)
    {
        first=temp;
    }
    else
    {
        while(p!=NULL)
        {
            p1=p;
            if(p->data==l)//neu da dem 1 tu tren 1 dong r thi khong can theem vao list(vidu: were)
            {
                return first;
            }
            p=p->next;
        }
        p1->next=temp;
    }
    
    return first;

}
list *makeNode(char word[],int newLine)
{

    list *newNode=NULL;
    newNode=(list*)malloc(sizeof(list));
    strcpy(newNode->word,word);
    newNode->first=(line *)malloc(sizeof(line));
    newNode->first->data=newLine;
    newNode->first->next=NULL;
    return newNode;
}
list *insert(list *root,char word[],int newLine)
{
    line *p;
    if(root==NULL)
    {
        root=makeNode(word,newLine);
    }
    else if(strcmp(word,root->word)<0)
    {
        root->left=insert(root->left,word,newLine);
    }
    else if(strcmp(word,root->word)>0)
    {
        root->right=insert(root->right,word,newLine);
    }
    else if(strcmp(word,root->word)==0)
    {
        
        root->first=addLine(root->first,newLine);
        
    }
    return root;
}

void Display(list *tree)
{
    line *p;
    if(tree != NULL)
    {
        Display(tree->left);
        printf("%-15s",tree->word);
        p=tree->first;
        while(p!=NULL)
        {
            printf("%d",p->data);
            p=p->next;
            if(p!=NULL)
            {
                printf(",");
            }
        }
        printf("\n");
        Display(tree->right);
    }                   
}
char *readStop()//doc file stopw.txt
{
    char *text;
    char c;
    int i=0;
    FILE *f= fopen("stopw.txt","rt");
    FILE *f1= fopen("stopw.txt","rt");
    while(fscanf(f,"%c",&c)>0)
    {
        i++;
    }
    fclose(f);
    text = (char *)malloc(sizeof(char)*i);
    i=0;
    while(fscanf(f1,"%c",&c)>0)
    {
        text[i]=c;
        i++;
    }
    fclose(f1);
    return text;
}
int checkStop(char *text,char *word)// phat hien "word" co trong "stopw.txt" khong
{
    if(strstr(text,word)!=NULL) return 1;
    return 0;
}
void readToList(char *text,list *root)
{
    char word[30],buff[200];
    char *temp,*str1,*str2;//*temp:vi tri cac dau cach,*str1:con tro dau cua tu hien tai,*str2: luu lai vi tri cua tu tiep theo
    int line=1,checkN=0;//checkN: kiem tra danh tu rieng
    FILE *f;
    f = fopen("vanban.txt", "rt");
    while (fscanf(f,"%[^\n]\n",buff)>0)// doc tung dong vao buff
    {
        buff[strlen(buff)]='\0';
        str1=buff;
        str2=str1;
        while(1){
            temp=strchr(str1,' ');//tra ve vi tri dau tien co dau cach
            if(temp==NULL) break;
            str2=temp;
            str2++;// tro den vi tri dau tien cua tu tiep theo
            *temp='\0';// cat tu truoc dau cach
            if(isalpha(str1[0]))
            {
                if(isupper(str1[0])&&checkN==1)//neu tu hien tai co ki tu dau viet hoa va dung sau dau ".""
                {   
                    
                    strcpy(word,str1);
                    
                    if (strchr(word,'.')!=NULL)
                    {
                        checkN=1;
                    }else checkN=0;
                    word[0]=tolower(word[0]);
                    if(checkStop(text,word)==0){
                        if(!isalpha(word[strlen(word)-1])) word[strlen(word)-1]='\0';//loai bo cac ki tu khong phai chu cai o cuoi cua tu 
                        root= insert(root,word,line);
                        
                    }
                    
                }
                else{
                    strcpy(word,str1);
                    if (islower(word[0])&&checkStop(text,word)==0)
                    {
                        if(!isalpha(word[strlen(word)-1])) word[strlen(word)-1]='\0';
                        root= insert(root,word,line);
                    }
                    if (strchr(word,'.')!=NULL)
                    {
                        checkN=1;
                    }else checkN=0;

                }
            }
            str1=str2;
        }
        //xu li tu cuoi cung cua dong
        if(isalpha(str1[0])){
            strcpy(word,str1);
            word[strlen(word)-1]='\0';
            if (islower(word[0])&&checkStop(text,word)==0)
                {
                    root= insert(root,word,line);
                }
            if (strchr(word,'.')!=NULL)
                {
                    checkN=1;
                }else checkN =0;
         }
        line++;
    }
    //Khong xu li duoc tu 'tribal' do loi nhap van ban
    fclose(f);
    Display(root);
    
}
int main(void)
{
    char *text=readStop();
    readToList(text,root);
}