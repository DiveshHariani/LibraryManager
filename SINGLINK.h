#include<malloc.h>
typedef struct node         ///Self referential structure...
{
    void *data;
    struct node *link;
}NODE;

typedef struct
{
    NODE *start;
    NODE *end;
    int count;
    int (*compare)(void *,void *);
    int (*greater)(void *,void *);
    void (*print)(void *);
}HEAD;

HEAD *create(int (*compare)(void *,void *) , void (*print)(void *) , int (*greater)(void *,void *))
{
    HEAD *linklist;
    linklist=(HEAD *)malloc(sizeof(HEAD));
    linklist->start=NULL;
    linklist->end=NULL;
    linklist->count=0;
    linklist->compare=compare;
    linklist->greater=greater;
    linklist->print=print;
}

void insert(void *datap,int pos,HEAD *linklist)
{
    NODE *node,*ptr;
    int position=1;
    node=(NODE *)malloc(sizeof(NODE));
    node->data=datap;
    node->link=NULL;
    if(linklist->count==0)
    {
        linklist->start=linklist->end=node;
    }
    if(pos<=1)
    {
        node->link=linklist->start;
        linklist->start=node;
    }
    else if(pos>=linklist->count)
    {
        linklist->end->link=node;
        linklist->end=node;
    }
    else
    {
        ptr=linklist->start;
        while(position!=pos-1)
        {
            ptr=ptr->link;
            position++;
        }
        node->link=ptr->link;
        ptr->link=node;
    }
    linklist->count++;
}
void traverse(HEAD *linklist)
{
    NODE *ptr;
    ptr=linklist->start;
    while(ptr!=NULL)
    {
       linklist->print(ptr->data);
       ptr=ptr->link;
    }
}
void* search(HEAD *linklist,void *datap)
{
    NODE *ptr;
    ptr=linklist->start;
    while(ptr!=NULL)
    {
        if(linklist->compare(datap,ptr->data))
            return ptr->data;
        ptr=ptr->link;
    }
    return NULL;
}
void sort(HEAD *linklist)
{
    NODE *ptr,*preptr;
    int i,j;
    void *datap;
    ptr=linklist->start;
    for(i=linklist->count-1;i>0;i--)
    {
        preptr=ptr;
        ptr=ptr->link;
        for(j=0;j<i;j++)
            if(linklist->greater(preptr->data,ptr->data)==1)
            {
                datap=preptr->data;
                preptr->data=ptr->data;
                ptr->data=datap;
            }
    }
}
void *delete(HEAD *linklist,void *datap)
{
    NODE *ptr,*preptr=NULL;
    void *dataptr;
    ptr=linklist->start;
    while(ptr!=NULL && linklist->compare(ptr->data,datap)!=1)
    {
        preptr=ptr;
        ptr=ptr->link;
    }
    if(ptr==NULL)
        return NULL;
    else
    {
        if(preptr==NULL)            //Element to be deleted is at start.
        {
            linklist->start=linklist->start->link;
        }
        else if(ptr->link==NULL)    //Element to be deleted is at end.
        {
            preptr->link=NULL;
            linklist->end=preptr;
        }
        else{
            preptr->link=ptr->link;
        }
        free(ptr);
        linklist->count--;
    }
}
void *retrieve(HEAD *linklist,int pos)
{
    int position=1;
    NODE*ptr;
    ptr=linklist->start;
    while(position!=pos && ptr!=NULL)
    {
        ptr=ptr->link;
        position++;
    }
    return ptr->data;
}
void destroy(HEAD *linklist)
{
    NODE *ptr,*preptr;
    ptr=linklist->start;
    while(ptr!=NULL)
    {
        preptr=ptr;
        ptr=ptr->link;
        free(preptr);
    }
    free(linklist);
   // printf("Destroyed!");
}
