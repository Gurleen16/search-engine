#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<string.h>
#define word_LENGTH 1000
struct hashtable
{
  struct Node *ptr;
  int count;
};
struct Node
{
  char word[500];
  struct Node *prev;
  struct Node *next;
  struct fileno *f_ptr;
};
struct fileno
{
  int count_;
  char url[100];
  struct fileno *up;
  struct fileno *down;
};
char *words[3000];
int findfilesize(char *str)
{
  struct stat st;
  stat(str,&st);
  return st.st_size;
}
void openfile(char *dir,int fileno)
{
  sprintf(dir,"/home/disha/Desktop/target/temp%d.txt",fileno);
}
void putinfile(struct Node *new)
{
FILE *f=fopen("/home/disha/Desktop/target/index.txt","w");
while(new!=NULL)
{
fprintf(f,new->word);
putc(' ',f);
while(new->f_ptr!=NULL)
{
putc('#',f);
fprintf(f,"%s",new->f_ptr->url);
putc('$',f);
fprintf(f,"%d",new->f_ptr->count_);
putc(' ',f);
new->f_ptr=new->f_ptr->down;
}
new=new->next;
putc('\n',f);

}
fclose(f);



}

char* ram(char file[100],int fsize)
{
	FILE *source;
	source=fopen(file,"r");
	int i=0;
	char *target=(char *)malloc(fsize*sizeof(char));
	char ch=fgetc(source);
	while(ch!=EOF)
	{
		target[i]=ch;
		ch=fgetc(source);
		i++;
	}
	fclose(source);
	return target;
}
char* getfile(int fileno,char *file)
{
  int fsize;
 //char file[100];
 openfile(file,fileno);
 fsize=findfilesize(file);
 //printf("%d",fsize);
 //printf("%s\n",file);
 char *html=(char *)malloc(fsize*sizeof(char));
 html=ram(file,fsize);
// printf("%s",html);
return html;
}
int findword(char *html,char *result,int pos)
{
  int j=0;
  for(int i=pos;html[i]!='\0';i++)
  {
    j=0;
    if(html[i]=='<'&&html[i+1]=='t'&&html[i+2]=='i'&&html[i+3]=='t'&&html[i+4]=='l'&&html[i+5]=='e')
    {
      j=0;
      i+=5;
      while(html[i]!='>')
      {
        i++;
      }
      i++;
      while(html[i]!='<'&&html[i+1]!='\\')
      {
        result[j]=html[i];
        i++;
        j++;
      }
    result[j]='\0';
    while(html[i]!='>')
    {
      i++;
    }
    return i+1;
    }

    else if(html[i]=='<'&&html[i+1]=='h'&&(html[i+2]=='1'||html[i+2]=='2'||html[i+2]=='3'||html[i+2]=='4'||html[i+2]=='5'||html[i+2]=='6'))
         {
           i=i+2;
           while(html[i]!='>')
           {
               i++;
           }
           i++;
           int j=0;
           while(html[i]!='<')
           {
               result[j]=html[i];
               i++;
               j++;
           }
           result[j]='\0';
           return i;

         }
  }
  return -1;
}

int word_count(char *word,int index,char *words[3000],int words_index)
{
  int count=1;
  //printf("%s ",word);
  for(int i=index+1;i<words_index;i++)
  {
    if(word==""||words[i]=="")
    continue;
    else if(strcmp(word,words[i])==0)
    {
      count++;
      words[i]="";
    }
  }
  return count;
}
int generate_key(char * word)
{
int size=strlen(word);
static int s=3;
static int i=1;
int size_base=20;
int ch=(int)word[size-word_LENGTH];
int key=(size+ch);
key=key-s;
key=key*100+i;
i++;
key=key%1877;
s=s%10+8;
return key;
}
int check_hash(int k,struct hashtable *hash)
{

if(hash[k].ptr==NULL)
return 1;
else
return 0;

}
void insert_in_hash(char *words[3000],int words_index,struct Node **head,struct hashtable *hash, int page,char *url1)
{


  int count=0;

  for(int i=0;i<words_index;i++)
  {

    count=word_count(words[i],i,words,words_index);
    //printf("%d\n",count);
    int key=generate_key(words[i]);
    if(key<0)
    key*=(-1);
  //  printf("%d\n",key);
    int toappend=0;
    int toinsert=0;
    int todo=check_hash(key,hash);
    if(todo==1)
     toappend=1;
    else
     toinsert=1;
     if(toappend&&words[i]!="")
     {
         struct Node *node;
         node=(struct Node *)malloc(sizeof(struct Node));
         struct Node *last=*head;
         struct fileno *file;
         file=(struct fileno *)malloc(sizeof(struct fileno));
         file->count_=count;
         //printf("%dvcvv\n",file->count_);
         file->down=NULL;
         file->up=NULL;
         strcpy(file->url,url1);

       strcpy(node->word,words[i]);
       node->next=NULL;
      while(last->next!=NULL)
        last=last->next;
      last->next=node;
      node->prev=last;
      node->f_ptr=file;
      hash[key].ptr=node;
      hash[key].count=1;
     }
     else if(toinsert&&words[i]!="")
     {

       struct Node *last=*head;
       struct fileno *file;
      file=(struct fileno *)malloc(sizeof(struct fileno));
       struct Node *addr=(struct Node*)malloc(sizeof(struct Node));
        addr=hash[key].ptr;
     int totrace=hash[key].count;
     while(totrace>0)
      {

        if(strcmp(addr->word,words[i])==0)
        {
        //  printf("***************************llll\n");
          struct fileno *f;
          f=addr->f_ptr;
          while(f->count_>count)
          f=f->down;

          file->count_=count;
          strcpy(file->url,url1);
          file->down=f->down;
          file->up=f;
          f->down=file;
          toinsert=0;
          break;
        }
        if(totrace!=1)
         addr=addr->next;

        totrace--;
      }
       if(toinsert)
       {
         struct Node *node;
         node=(struct Node *)malloc(sizeof(struct Node));
       if(addr->next!=NULL)
       {
         node->next=addr->next;
         node->prev=addr;
         addr->next=node;

         strcpy(node->word,words[i]);

          (hash[key].count)++;
        }
        else{

          strcpy(node->word,words[i]);
          node->next=NULL;
          //while(last->next!=NULL)
          //last=last->next;
          addr->next=node;
          node->prev=addr;
          (hash[key].count)++;
        }
       strcpy(file->url,url1);
      // printf("%dkk",count);
        file->count_=count;
      //  printf("%dll",file->count_);
        file->down=NULL;
        file->up=NULL;
       node->f_ptr=file;
      }
    }


    // add_list(key,hash,)
    //printf("%d\n",count);
    //strcpy(node->word,words[i]);
    //node->key=generate_key(words[i]);
  //}
}
}

char* find_url(char *file)
{
  int i=0;
  FILE *f1;
  f1=fopen(file,"r");
  char* url;
  char ch;
  ch=fgetc(f1);
  while(ch!='\n')
  {
    url[i]=ch;
    i++;
    ch=fgetc(f1);
  }
  return url;
}
int main()
{
  struct hashtable *hash=(struct hashtable *)malloc(1877*sizeof(struct hashtable));
struct Node* head=(struct Node*)malloc(sizeof(struct Node));
char **words=(char **)malloc(3000*sizeof(char *));
for(int i=0;i<3000;i++)
{
words[i]=(char *)malloc(500*sizeof(char));
}
int wordindex=0;
  int fileid;
  int  k=0;
  int g=0;
   for(int i=1;i<11;i++)
   {


     char *file=(char *)malloc(100*sizeof(char));
      int pos=0;
     fileid=i;

    char  *html=getfile(i,file);
    char *url=(char *)malloc(100*sizeof(char));
    url=find_url(file);

     char *sentence[1000];
     char a[50];
     while(pos!=-1)
     {
       sentence[k]=(char *)malloc(5000*sizeof(char));
     pos=findword(html,sentence[k],pos);

    for(int t=0;sentence[k][t]!='\0';t++)
     {
     if(sentence[k][t]==' ')
      {
       a[g]='\0';
       strcpy(words[wordindex],a);
       g=0;
      wordindex++;
      while(sentence[k][t]==' '){
      t++;}
      t--;
      }
      else
      {
      if((sentence[k][t]>=65&&sentence[k][t]<=90)||(sentence[k][t]>=97&&sentence[k][t]<=122)||(sentence[k][t]>=48&&sentence[k][t]<=57))
      a[g++]=sentence[k][t];
    }

    }


     k++;
     free(sentence[k]);

   }
    insert_in_hash(words,wordindex,&head,hash,i,url);
  //
  free(html);

   }
   putinfile(head);
   struct Node *temp;
   temp=head;
   int n=1;
   while(temp!=NULL)
   {
     n++;
  // printf("%s\n",temp->word);
    struct fileno *f;
    f=temp->f_ptr;
    while(f!=NULL)
    {
    //printf("**%d\n",f->count_);
    //printf("%s\n",f->url);
    f=f->down;
  }
   temp=temp->next;
 }

 return 0;
}
