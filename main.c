#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include "SINGLINK.h"

typedef struct date
{
    int dd,mm,yy;
}Date;

typedef struct book
{
    char name[100];
    char author[200];
    int id;
    int copies;
}Book;
typedef struct student
{
    int id;
    char name[20];
    int b1id,b2id;
    int issued;
    Date d1,d2;
}student;

//Functions for Books
int comparestr(void *ptr1,void *ptr2)                   ///Comparison based on ids
{
    if(*(int *)ptr1==(*(Book*)ptr2).id)    return 1;
    else return 0;
}
int printbk(void *ptr)
{
    printf("\t\t%2d|%+26s|%16s|%6d\n",(*(Book*)ptr).id,(*(Book*)ptr).name,(*(Book*)ptr).author,(*(Book*)ptr).copies);
}
int comparename(void *str1,void *ref)
{
    if(strcmpi((char*)str1,(*(Book*)ref).name)==0)  return 1;
    else return 0;
}
/* Greater is same as student*/

//Functions for students
int compareid(void *ptr1,void *ptr2)
{
    if(*(int*)ptr1==(*(student*)ptr2).id)    return 1;
    else return 0;
}
void printst(void *ptr)
{
    int i;
    printf("\t\t%2d|%+20s|%6d|",(*(student*)ptr).id,(*(student*)ptr).name,(*(student*)ptr).issued);
    printf("%d\\%d\\%d  %d\\%d\\%d ",(*(student*)ptr).d1.dd,(*(student*)ptr).d1.mm,(*(student*)ptr).d1.yy,(*(student*)ptr).d2.dd,(*(student*)ptr).d2.mm,(*(student*)ptr).d2.yy);
    if((*(student*)ptr).issued>0)
    {
        printf("Books id=");
        if((*(student*)ptr).b1id!=0 && (*(student*)ptr).b2id!=0)
            printf("%d %d\n",(*(student*)ptr).b1id,(*(student*)ptr).b2id);
        else if((*(student*)ptr).b1id!=0)
            printf("%d\n",(*(student*)ptr).b1id);
        else if((*(student*)ptr).b2id!=0)
            printf("%d\n",(*(student*)ptr).b2id);
    }
}
int greaterid(void *ptr1,void *ptr2)
{
    if((*(student*)ptr1).id>(*(student*)ptr2).id)   return 1;
    return 0;
}

///Driver's Code
int main()
{
    int option,student_id,i,opt1,opt2,lid;
    struct date dt;
    char str[100],pass[10],c;

    void addDatabk(HEAD *);                     //File->RAM(book)
    void addbkfile(HEAD *);                     //RAM->File(book)

    void addDatast(HEAD *);                     //File->RAM(student)
    void addstfile(HEAD *);                     //RAM->File(student)

    int checkpwd(char [],int);
    student *student_ref;
    time_t T= time(NULL);
    struct  tm tm = *localtime(&T);
    Book *book_ref;
    HEAD *scoll,*bcoll,*bcoll_name;
    int getpass(int);
    void returnBook(student *,HEAD *,HEAD *);
    void createAccount(HEAD *);
    void addBookLib(HEAD *,HEAD *);

    scoll=create(compareid,printst,greaterid);
    addDatast(scoll);                           //File->RAM

    bcoll=create(comparestr,printbk,greaterid);
    addDatabk(bcoll);                           //File->RAM

    bcoll_name=create(comparename,printbk,greaterid);
    addDatabk(bcoll_name);                     //File->RAM
home:printf("\t\t***Log in***\n");
    printf("\t\t  1->As student\n");
    printf("\t\t  2->As Librarian\n");
    printf("\t\t  3->Close Application and save data\n");
    printf("\t\t  Enter option=");
    scanf("%d",&option);
    if(option==1)
    {
        system("cls");
        printf("Enter student id=");
        scanf("%d",&student_id);
        printf("Enter password=");
        if(getpass(student_id)==0)
        {
            printf("\nWrong password");
            printf("\nEnter any key to continue to home page\n");
            i=getch();
            system("cls");
            goto home;
        }
        system("cls");
        student_ref=(student *)search(scoll,&student_id);
home1:  printf("\t\t\tHi %s\n",student_ref->name);
        printf("\t\t\t1.View Profile\n");
        printf("\t\t\t2.Issue Book\n");
        printf("\t\t\t3.Return Book\n");
        printf("\t\t\t4.View Books\n");
        printf("\t\t\t5.Log out");
        printf("\t\t\tEnter option=");
        scanf("%d",&opt1);
        system("cls");
        switch(opt1)
        {
            case 1: printf("Id=%d Name:%20s Number of books issued=%d\n",student_ref->id,student_ref->name,student_ref->issued);
                    printf("Books:\n");
                    if(student_ref->b1id!=0 && student_ref->b2id!=0)
                    {
                        book_ref=(Book*)search(bcoll,&student_ref->b1id);               ///Profile
                        printf("1)%d  %s\n",book_ref->id,book_ref->name);
                        book_ref=(Book*)search(bcoll,&student_ref->b2id);
                        printf("2)%d  %s\n",book_ref->id,book_ref->name);
                    }
                    else if(student_ref->b1id!=0)
                    {
                        book_ref=(Book*)search(bcoll,&student_ref->b1id);
                        printf("1)%d  %s\n",book_ref->id,book_ref->name);
                    }
                    else if(student_ref->b2id!=0)
                    {
                        book_ref=(Book*)search(bcoll,&student_ref->b2id);
                        printf("1)%d  %s\n",book_ref->id,book_ref->name);
                    }
                    else printf(" No Books issued\n");
                    break;
            case 2: if(student_ref->issued>=2)                          ///Issuing books
                        printf("Cannot issue more books\n");
                    else
                    {
                        printf("Enter name of book:");
                        fflush(stdin);
                        gets(str);
                        book_ref=(Book*)search(bcoll_name,str);
                        if(book_ref!=NULL)
                            if(book_ref->copies>=2)
                            {
                                printf("Book available\n");
                                printf("1.Issue\n");
                                printf("2.Goto Home\nEnter Option:");
                                scanf("%d",&opt2);
                                if(opt2==1)
                                {
                                    student_ref->issued++;
                                    if(student_ref->b1id==0)
                                        {
                                            student_ref->b1id=book_ref->id;
                                            student_ref->d1.dd=tm.tm_mday;
                                            student_ref->d1.mm=tm.tm_mon+1;
                                            student_ref->d1.yy=tm.tm_year+1900;
                                        }
                                    else if(student_ref->b2id==0)
                                        {
                                            student_ref->b2id=book_ref->id;
                                            student_ref->d2.dd=tm.tm_mday;
                                            student_ref->d2.mm=tm.tm_mon+1;
                                            student_ref->d2.yy=tm.tm_year+1900;
                                        }
                                    book_ref->copies=book_ref->copies-1;
                                    printf("Issued\n");
                                    printf("Enter any character to go to home\n");
                                    i=getch();
                                }
                                else goto home1;
                            }
                            else printf("Shortage...\n");
                        else printf("Book Not found\n");
                    }
                    printf("Enter any character to go to home");
                    system("cls");
                    i=getch();
                    goto home1;
                    break;
        case 3: system("cls");
                returnBook(student_ref,bcoll,bcoll_name);
                break;
        case 4: system("cls");
                traverse(bcoll);

        case 5: goto home;
        }
        printf("Enter any key to return to home\n");
        i=getch();
        system("cls");
        goto home1;
    }
    else if(option==2)
    {
        system("cls");
        printf("Enter Librarian ID=");
        scanf("%d",&lid);
        printf("Enter password:");
        i=0;
        do
        {
            c=getch();
            if(c!='\r')
            {
                pass[i]=c;
                printf("*");
                i++;
            }
        }
        while(c!='\r');
        pass[i]='\0';
        if(strcmp(pass,"100")!=0)
        {
            printf("\nWrong password");
            printf("\nEnter any key to continue");
            i=getch();
            system("cls");
            goto home;
        }
        if(strcmp(pass,"100")==0 && lid==100)
        system("cls");
home2:  printf("1.Create Student Account\n");
        printf("2.View Books\n");
        printf("3.Add a Book\n");
        printf("4.Log out\n");
        printf("Enter option=");
        scanf("%d",&opt1);
        switch(opt1)
        {
            case 1: createAccount(scoll);
                    break;
            case 2: printf("\t\tId| Name     | Author         |Copies\n");
                    traverse(bcoll);
                    break;
            case 3: addBookLib(bcoll_name,bcoll);
                    break;
            case 4: goto home;
        }
        printf("Enter any key to continue");
        i=getch();
        system("cls");
        goto home2;
    }
    else
    {
            addbkfile(bcoll_name);
            addstfile(scoll);
            destroy(scoll);
            destroy(bcoll);
            destroy(bcoll_name);
            free(student_ref);
            free(book_ref);
            ///Add data to file and close
    }
    return 0;
}
void addDatast(HEAD *st)
{
    FILE *f;
    int id,issued;
    char name[10];
    student *s;
    f=fopen("Student.txt","r");
    if(f==NULL)
        printf("File not found");
    else
    {
        do
        {
            s=(student*)malloc(sizeof(student));
            fscanf(f,"%d%d\"%[^\"]\"%d%d%d%d%d%d%d%d",&s->id,&s->issued,s->name,&s->b1id,&s->b2id,&s->d1.dd,&s->d1.mm,&s->d1.yy,&s->d2.dd,&s->d2.mm,&s->d2.yy);
            //printf("%d %d %s\n",s->id,s->issued,s->name);
            insert(s,100,st);
        }
        while(!feof(f));
    }
    fclose(f);
}
void addDatabk(HEAD *book)
{
    int id,cop;
    char name[10],auth[20];
    FILE *f;
    Book *b;
    f=fopen("Book.txt","r");
    if(f==NULL)
        printf("File not found");
    else
    {
       do
        {
            b=(Book*)malloc(sizeof(Book));
            fscanf(f,"%d%d %[^.].%[^\n]",&b->id,&b->copies,b->name,b->author);
            //printf("%d %d %s %s\n",b->id,b->copies,b->name,b->author);
            insert(b,100,book);
        }
       while(!feof(f));
    }
    fclose(f);
}
int getpass(int student_id)
{
    char pwd[20],c;
    int i=0;
    do
    {
        c=getch();
        if(c!='\r')
        {
            pwd[i]=c;
            printf("*");
            i++;
        }
    }
    while(c!='\r');
    pwd[i]='\0';
    return checkpwd(pwd,student_id);
}
int checkpwd(char pwd[],int student_id)
{
    FILE *f;
    char checkpwd[10];
    int id;
    f=fopen("StudentLogin.txt","r");
    if(f==NULL)
        printf("Not found login file");
    else
    {
        do
        {
            fscanf(f,"%d %[^\n]",&id,checkpwd);
            if(id==student_id && strcmp(checkpwd,pwd)==0)
                return 1;                                   ///Successful matching
        }
        while(!feof(f));
    }
    fclose(f);
    return 0;
}
void addbkfile(HEAD *b)
{
    NODE *p;
    Book *ref;
    FILE *f;
    f=fopen("Book.txt","w");
    if(f!=NULL)
    {
        p=b->start;
        while(p!=NULL)
        {
            ref=(Book *)p->data;
            if(p->link!=NULL)
                fprintf(f,"%d %d %s.%s\n",ref->id,ref->copies,ref->name,ref->author);
            else
                fprintf(f,"%d %d %s.%s",ref->id,ref->copies,ref->name,ref->author);
            p=p->link;
        }
    }
    fclose(f);
}
void addstfile(HEAD *s)
{
    NODE *p;
    FILE *f;
    student *ref;
    f=fopen("Student.txt","w");
    if(f!=NULL)
    {
        p=s->start;
        while(p!=NULL)
        {
            ref=(student *)p->data;
            if(p->link!=NULL)
                fprintf(f,"%d %d\"%s\" %d %d %d %d %d %d %d %d\n",ref->id,ref->issued,ref->name,ref->b1id,ref->b2id,ref->d1.dd,ref->d1.mm,ref->d1.yy,ref->d2.dd,ref->d2.mm,ref->d2.yy);
            else
                fprintf(f,"%d %d\"%s\" %d %d %d %d %d %d %d %d",ref->id,ref->issued,ref->name,ref->b1id,ref->b2id,ref->d1.dd,ref->d1.mm,ref->d1.yy,ref->d2.dd,ref->d2.mm,ref->d2.yy);
            p=p->link;
        }
    }
}
void returnBook(student *s_ref,HEAD *bcoll,HEAD *bcoll_name)
{
    Book *b_ref;
    int b_id;
    printf("Enter book id=");
    scanf("%d",&b_id);
    b_ref=(Book *)search(bcoll,&b_id);
    b_ref=(Book*)search(bcoll_name,b_ref->name);
    if(b_ref!=NULL)
    {
        if(b_id==s_ref->b1id)
        {
            b_ref->copies=b_ref->copies+1;
            s_ref->b1id=0;
            s_ref->d1.dd=s_ref->d1.mm=s_ref->d1.yy=0;
            s_ref->issued=s_ref->issued-1;
            printf("Returned\n");
        }
        else if(b_id==s_ref->b2id)
        {
            b_ref->copies=b_ref->copies+1;
            s_ref->b2id=0;
            s_ref->d2.dd=s_ref->d2.mm=s_ref->d2.yy=0;
            s_ref->issued=s_ref->issued-1;
            printf("Returned\n");
        }
        else printf("No book with this ID found in your record.Please check Id\n");
    }
}
void createAccount(HEAD *scoll)
{
    student *s,*ref;
    int id,l,id_ref;
    FILE *f;
    char pass[10],c,pass1[10];
    int i;
    s=(student *)malloc(sizeof(student));
    f=fopen("StudentLogin.txt","a+");
    if(f!=NULL)
    {
        printf("Enter student id=");
        scanf("%d",&s->id);
        ref=(student*)search(scoll,&s->id);
        fflush(stdin);
        printf("Enter student name=");
        scanf("%[^\n]",s->name);
        s->b1id=s->b2id=s->issued=s->d1.dd=s->d1.mm=s->d1.yy=s->d2.dd=s->d2.mm=s->d2.yy=0;
        insert(s,scoll->count+1,scoll);
    pwd:printf("Enter password=");
        i=0;
        do
        {
            c=getch();
            if(c!='\r')
            {
                pass[i]=c;
                printf("*");
                i++;
            }
        }
            while(c!='\r');
            pass[i]='\0';

            /// confirm password
            printf("\nconfirm password=");
            i=0;
            do
            {
                c=getch();
                if(c!='\r')
                {
                    pass1[i]=c;
                    printf("*");
                    i++;
                }
            }
            while(c!='\r');
            pass1[i]='\0';
            if(strcmp(pass,pass1)==0)
            {
                fprintf(f,"\n%d %s",s->id,pass);
                printf("\nAccount created\n");
            }
            else
            {
                printf("Password mismatch\n");
                goto pwd;
            }

    fclose(f);

    }
    else    printf("File not found\n");
}
void addBookLib(HEAD *bcoll_name,HEAD *bcoll)
{
   Book *ref,*s_ref;
   int copy,id;
   ref=(Book*)malloc(sizeof(Book));
   s_ref=(Book*)malloc(sizeof(Book));
   printf("Enter book id=");
   scanf("%d",&id);
   s_ref=(Book*)search(bcoll,&id);
   if(s_ref!=NULL)
    {
        printf("Book with same id is already present\n");
        s_ref=(Book *)search(bcoll_name,s_ref->name);
        printf("Enter total number of copies=");
        scanf("%d",&copy);
        s_ref->copies=copy;
    }
   else
    {
        ref->id=id;
        fflush(stdin);
        printf("Enter name=");
        scanf("%[^\n]",ref->name);
        fflush(stdin);
        printf("Enter author name=");
        scanf("%[^\n]",ref->author);
        printf("Enter number of copies=");
        scanf("%d",&ref->copies);
        insert(ref,100,bcoll_name);
    }
    printf("List Modified\n");
}
