#include<stdio.h>
#include<stdlib.h>
#define base_url "https://www.chitkara.edu.in"
#include <sys/types.h>
#include <sys/stat.h>
#include<string.h>

struct node
{
	char url[150];
	int depth;
	int isvisited;
	struct node *prev;
	struct node *next;

};
struct node *head;
struct node *tail;
int size=1;
void testDir(char *dir)
{
  struct stat statbuf;
  if ( stat(dir, &statbuf) == -1 )
  {
    fprintf(stderr, "-----------------\n");
    fprintf(stderr, "Invalid directory\n");
    fprintf(stderr, "-----------------\n");
    exit(1);
  }

  //Both check if there's a directory and if it's writable
  if ( !S_ISDIR(statbuf.st_mode) )
  {
    fprintf(stderr, "-----------------------------------------------------\n");
    fprintf(stderr, "Invalid directory entry. Your input isn't a directory\n");
    fprintf(stderr, "-----------------------------------------------------\n");
    exit(1);
  }

  if ( (statbuf.st_mode & S_IWUSR) != S_IWUSR )
  {
    fprintf(stderr, "------------------------------------------\n");
    fprintf(stderr, "Invalid directory entry. It isn't writable\n");
    fprintf(stderr, "------------------------------------------\n");
    exit(1);
  }
  printf("VALID DIRECTORY\n");
}
int check_seedurl(char seed_url[50])
{
	int c=0,i;
	for(i=0;base_url[i]!='\0'&& seed_url[i]!='\0';i++)
	{
		if(base_url[i]!=seed_url[i])
		{
			c=1;
			break;
		}
	}
	if(c==1)
	{
		printf("Not same\n");
		return 2;
	}
	else
	{
		char str1[100]="wget --spider ";
		strcat(str1,seed_url);
		if(!system(str1))
		{
			printf("Valid URL\n");
			struct node *n;
			n=(struct node *)malloc(sizeof(struct node));
			head=(struct node *)malloc(sizeof(struct node));
			tail=(struct node *)malloc(sizeof(struct node));
			strcpy(n->url,seed_url);
			n->depth=0;
			n->isvisited=1;
			n->prev=NULL;
			n->next=NULL;
			head=n;
			tail=n;
			free(n);
			return 1;
		}
		else
		{
			printf("Invalid URL\n");
			return 2;
		}
		return 0;
	}
}
_Bool check_depth(int depth)
{
	if(depth<1 || depth>5)
	{
	printf("DEPTH Error");
	return 0;
	}
	else
	printf("DEPTH =%d\n",depth);
	return 1;
}
void getpage(char *url)
{
	char urlbuffer[500]={0};
	strcat(urlbuffer, "wget -O ");

	strcat(urlbuffer,"/home/disha/Desktop/target/temp.txt ");
	strcat(urlbuffer, url);

	system(urlbuffer);


}

char* newfile()
{
	static int filecount=1;
	char file[6]={0};
	char str[100]={0};

	strcat(str,"touch ");
	strcat(str,"/home/disha/Desktop/target/");
	sprintf(file,"%d",filecount);
	strcat(file,".txt");
	strcat(str,file);
	filecount++;
	system(str);
	char f1[50]="/home/disha/Desktop/target/";
	char *ptr=(char *)malloc(50*sizeof(char));
	strcat(f1,file);
	strcpy(ptr,f1);

	return ptr;

}
void copyfile(char file[100])
{
	FILE *f1,*f2;
	f1=fopen("/home/disha/Desktop/target/temp.txt","r");
	if(f1==NULL)
	{
	printf("File cant be opened\n");
	exit(0);
	}
	char ch=fgetc(f1);
	f2=fopen(file,"w");
	if(f2==NULL)
	{
	printf("File cant be opened\n");
	exit(0);

	}
	while(ch!=EOF)
	{
		fputc(ch,f2);
		ch=fgetc(f1);
		size++;
	}
	fclose(f1);
	f1=fopen("/home/disha/Desktop/target/temp.txt","w");
	fclose(f1);
	fclose(f2);


}

char* ram(char file[100])
{
	FILE *source;
	source=fopen(file,"r");
	int i=0;
	char *target=(char *)malloc(size*sizeof(char));
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

void removeWhiteSpace(char* html)
{
  int i;
  char *buffer = malloc(strlen(html)+1), *p=malloc (sizeof(char)+1);
  memset(buffer,0,strlen(html)+1);
  for (i=0;html[i];i++)
  {
    if(html[i]>32)
    {
      sprintf(p,"%c",html[i]);
      strcat(buffer,p);
    }
  }
  strcpy(html,buffer);
  free(buffer); free(p);
}

int GetNextURL(char* html, char* urlofthispage, char* result, int pos)
{
  char c;
  int len, i, j;
  char* p1;  //!< pointer pointed to the start of a new-founded URL.
  char* p2;  //!< pointer pointed to the end of a new-founded URL.

  // NEW
  // Clean up \n chars
  if(pos == 0) {
    removeWhiteSpace(html);
  }
  // /NEW

  // Find the <a> <A> HTML tag.
  while (0 != (c = html[pos]))
  {
    if ((c=='<') &&
        ((html[pos+1] == 'a') || (html[pos+1] == 'A'))) {
      break;
    }
    pos++;
  }
  //! Find the URL it the HTML tag. They usually look like <a href="www.abc.com">
  //! We try to find the quote mark in order to find the URL inside the quote mark.
  if (c)
  {
    // check for equals first... some HTML tags don't have quotes...or use single quotes instead
    p1 = strchr(&(html[pos+1]), '=');

    if ((!p1) || (*(p1-1) == 'e') || ((p1 - html - pos) > 10))
    {
      // keep going...
      return GetNextURL(html,urlofthispage,result,pos+1);
    }
    if (*(p1+1) == '\"' || *(p1+1) == '\'')
      p1++;

    p1++;

    p2 = strpbrk(p1, "\'\">");
    if (!p2)
    {
      // keep going...
      return GetNextURL(html,urlofthispage,result,pos+1);
    }
    if (*p1 == '#')
    { // Why bother returning anything here....recursively keep going...

      return GetNextURL(html,urlofthispage,result,pos+1);
    }
    if (!strncmp(p1, "mailto:",7))
      return GetNextURL(html, urlofthispage, result, pos+1);
    if (!strncmp(p1, "http", 4) || !strncmp(p1, "HTTP", 4))
    {
      //! Nice! The URL we found is in absolute path.
      strncpy(result, p1, (p2-p1));
      return  (int)(p2 - html + 1);
    } else {
      //! We find a URL. HTML is a terrible standard. So there are many ways to present a URL.
      if (p1[0] == '.') {
        //! Some URLs are like <a href="../../../a.txt"> I cannot handle this.
	// again...probably good to recursively keep going..
	// NEW

        return GetNextURL(html,urlofthispage,result,pos+1);
	// /NEW
      }
      if (p1[0] == '/') {
        //! this means the URL is the absolute path
        for (i = 7; i < strlen(urlofthispage); i++)
          if (urlofthispage[i] == '/')
            break;
        strcpy(result, urlofthispage);
        result[i] = 0;
        strncat(result, p1, (p2 - p1));
        return (int)(p2 - html + 1);
      } else {
        //! the URL is a absolute path.
        len = strlen(urlofthispage);
        for (i = (len - 1); i >= 0; i--)
          if (urlofthispage[i] == '/')
            break;
        for (j = (len - 1); j >= 0; j--)
          if (urlofthispage[j] == '.')
              break;
        if (i == (len -1)) {
          //! urlofthis page is like http://www.abc.com/
            strcpy(result, urlofthispage);
            result[i + 1] = 0;
            strncat(result, p1, p2 - p1);
            return (int)(p2 - html + 1);
        }
        if ((i <= 6)||(i > j)) {
          //! urlofthis page is like http://www.abc.com/~xyz
          //! or http://www.abc.com
          strcpy(result, urlofthispage);
          result[len] = '/';
          strncat(result, p1, p2 - p1);
          return (int)(p2 - html + 1);
        }
        strcpy(result, urlofthispage);
        result[i + 1] = 0;
        strncat(result, p1, p2 - p1);
        return (int)(p2 - html + 1);
      }
    }
  }
  return -1;
}

void collect_links(char *html)
{
	int pos=0;
	int j=0;
	char *links[100];
	while(j!=100&&pos!=-1)
	{
		char *result=(char *)malloc(100*sizeof(char));
		pos=GetNextURL(html,base_url,result,pos);
		links[j]=(char *)malloc(100*sizeof(char));
		strcpy(links[j],result);
		//printf("%s\n",links[j]);
		j++;
	}
	printf("*******************************************************************\n");
	filling_links(links);
}

void filling_links(char *links[100])
{
	int i=0,flag=1;
	while(i!=100)
	{
		flag=check_list(links[i]);
		//printf("%d",flag);
		if(flag==1)
		{
			struct node *n;
			n=(struct node *)malloc(sizeof(struct node));
			strcpy(n->url,links[i]);
			n->depth=1;
			n->isvisited=0;

			tail->next=n;
			n->prev=tail;
			n->next=NULL;
			tail=n;

		}
		i++;

	}

	printf("************************************************************\n");
	struct node *ptr;
	ptr=head;
	/*while(ptr->next!=NULL)
	{
		printf("%s  %d\n",ptr->url,ptr->depth);
		ptr=ptr->next;
	}*/
}

int check_list(char *link)
{

	struct node *ptr;
	ptr=head;
	while(ptr->next!=NULL)
	{

		if(strcmp(ptr->url,link)==0)
		return 0;
		else
		ptr=ptr->next;
	}
	//printf("***********************\n");
	return 1;
}

void list_traverse()
{
	int c=0;
	struct node *ptr;
	ptr=head;
	while(/*ptr->next!=NULL*/c!=5)
	{
		printf("%d\n****************************************888",c);
		if(ptr->isvisited==0)
		{
			int i;
			printf("%s\n*****************************************************8888",ptr->url);
			i=check_seedurl(ptr->url);
			if(i==2)
			{
			ptr=ptr->next;
			c++;
			continue;
			}
			getpage(ptr->url);
			char *path=newfile();
			//copyfile(path);
			ptr=ptr->next;
		}
		else
		ptr=ptr->next;
		c++;
	}
}
int main(int argc, char *argv[])
{
	int i;
	char *seed_url=argv[1];
	int depth=atoi(argv[2]);
	char *target_dir=argv[3];
	check_depth(depth);
	testDir(target_dir);
	i=check_seedurl(seed_url);
	if(i==2)
	return 0;
	getpage(seed_url);
	char *path=newfile();
	copyfile(path);
	char *html=ram(path);
	collect_links(html);

	list_traverse();
}
