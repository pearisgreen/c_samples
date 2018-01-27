//hdr
#ifndef CONTAINER_H
#define CONTAINER_H

//---------- LIST ---------- 

struct list_node
{
    struct list_node *next;
#ifndef S_LINKED
    struct list_node *prev;
#endif
    void *data;
};

struct list
{
    struct list_node *head;
    struct list_node *tail;
    int size;
};

void list_init(struct list *this);

void list_delete(struct list *this);
void list_delete_all(struct list *this);

void list_push_front(struct list *this, void *data);
void list_push_back(struct list *this, void *data);

void *list_pop_front(struct list *this);
void *list_pop_back(struct list *this);

void *list_at(struct list *this, int x);
void *list_remove_at(struct list *this, int x);

//------------------------- 

//---------- MAP ---------- 

struct map_node
{
    struct map_node *next;
    char *key;
    void *data;
};

struct map
{
    struct map_node *head;
    struct map_node *tail;
    int size;
};

void map_init(struct map *this);

void map_delete(struct map *this);
void map_delete_all(struct map *this);

void map_push_front(struct map *this, char *key, void *data);
void map_push_back(struct map *this, char *key, void *data);

void map_pop_front(struct map *this, char *key, void *data);
void map_pop_back(struct map *this, char *key, void *data);

void *map_get(struct map *this, char *key);
void *map_at(struct map *this, int x);

void *map_remove(struct map *this, char *key);
void *map_remove_at(struct map *this, int x);


#endif

//src
#ifndef CONTAINER_C

#include <stdlib.h>
#include <string.h>

//---------- LIST ---------- 

void list_init(struct list *this)
{
    this->size = 0;
    this->head = 0;
    this->tail = 0;
}

void list_delete(struct list *this)
{
    if(!this->head)
        return;

    struct list_node *entry = this->head;
    struct list_node *free_entry;

    while(entry->next)
    {
        free_entry = entry;
        entry = entry->next;
        free(free_entry);
    }
    free(entry);

    this->head = 0;
    this->tail = 0;
    this->size = 0;
}

void list_delete_all(struct list *this)
{
    if(!this->head)
        return;

    struct list_node *entry = this->head;
    struct list_node *free_entry;

    while(entry->next)
    {
        if(!entry->data)
            free(entry->data);

        free_entry = entry;
        entry = entry->next;
        free(free_entry);
    }
    if(!entry->data)
        free(entry->data);
    free(entry);

    this->head = 0;
    this->tail = 0;
    this->size = 0;
}

void list_push_front(struct list *this, void *data)
{
    struct list_node *entry = malloc(sizeof(struct list_node));

    entry->data = data;
    entry->next = this->head;
#ifndef S_LINKED
    entry->prev = 0;
#endif

#ifndef S_LINKED
    if(this->head)
        this->head->prev = entry;
#endif

    if(!this->tail)
        this->tail = entry;
    this->size++;

    this->head = entry; 
}

void list_push_back(struct list *this, void *data)
{
    struct list_node *entry = malloc(sizeof(struct list_node)); 

    entry->data = data;
    entry->next = 0;
#ifndef S_LINKED
    entry->prev = this->tail;
#endif

    if(this->tail)
        this->tail->next = entry;

    if(!this->head)
        this->head = entry;
    this->size++;

    this->tail = entry;
}

void *list_pop_front(struct list *this)
{
    if(!this->head)
        return 0;

    struct list_node *entry = this->head;
    void *data = entry->data;

    if(entry->next)
    {
        this->head = entry->next;
#ifndef S_LINKED
        this->head->prev = 0;
#endif
    }
    else
    {
        this->head = 0;
        this->tail = 0;
    }
    this->size--;

    free(entry);
    return data;
}

void *list_pop_back(struct list *this)
{
    if(!this->head)
        return 0;

    struct list_node *entry = this->tail;
    void *data = entry->data;

    if(entry->prev)
    {
        this->tail = entry->prev;
#ifndef S_LINKED
        this->tail->next = 0;
#endif
    }
    else
    {
        this->head = 0;
        this->tail = 0;
    }
    this->size--;

    free(entry);
    return data;
}

void *list_at(struct list *this, int x)
{
    if(!this->head)
        return 0;

    struct list_node *entry;
    //walk forward if x < size/2
#ifndef S_LINKED
    if(x < this->size/2)
    {
#endif
        entry = this->head;
        while(entry->next && --x >= 0)
        {
            entry = entry->next;     
        }
#ifndef S_LINKED
    }
    else
    {
        x = this->size - x;
        x--;

        entry = this->tail;
        while(entry->prev && --x >= 0)
        {
            entry = entry->prev;
        }
    }
#endif

    return entry->data;
}

void *list_remove_at(struct list *this, int x)
{
    if(!this->head)
        return 0;

    void *data;

    struct list_node **entry;
    //walk forward if x < size/2
#ifndef S_LINKED
    if(x < this->size/2)
    {
#endif
        entry = &this->head;
        while((*entry)->next && --x >= 0)
        {
            entry = &(*entry)->next;
        }
        if((*entry)->next )
        {
            (*entry)->next->prev = (*entry)->prev;
        }
        else
        {
            this->tail = (*entry)->prev;
        }
        
        data = (*entry)->data;

        struct list_node *free_entry = *entry;

        *entry = (*entry)->next;

        free(free_entry);
#ifndef S_LINKED
    }
    else
    {
        x = this->size - x;
        x--;

        entry = &this->tail;
        while((*entry)->prev && --x >= 0)
        {
            entry = &(*entry)->prev;
        }
        if((*entry)->prev)
        {
            (*entry)->prev->next = (*entry)->next;
        }
        else
        {
            this->head = (*entry)->next;
        }

        data = (*entry)->data;

        struct list_node *free_entry = *entry;

        *entry = (*entry)->prev;

        free(free_entry);
    }
#endif
    this->size--;

    if(!this->size)
    {
        this->head = 0;
        this->tail = 0;
    }
    
    return data;
}

//------------------------- 

//---------- MAP ---------- 

void map_init(struct map *this)
{
    this->head = 0;
    this->tail = 0;
    this->size = 0;
}

void map_delete(struct map *this)
{
}

void map_delete_all(struct map *this)
{
}

void map_push_front(struct map *this, char *key, void *data)
{
}

void map_push_back(struct map *this, char *key, void *data)
{
}

void map_pop_front(struct map *this, char *key, void *data)
{
}

void map_pop_back(struct map *this, char *key, void *data)
{
}

void *map_get(struct map *this, char *key)
{
}

void *map_at(struct map *this, int x)
{
}

void *map_remove(struct map *this, char *key)
{
}

void *map_remove_at(struct map *this, int x)
{
}


//------------------------- 

#endif
















