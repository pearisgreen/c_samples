#include "map.h"

#include <stdlib.h>
#include <string.h>

//---------- MAP ---------- 

void map_init(struct map_t *this)
{
    this->head = 0;
    this->tail = 0;
    this->size = 0;
}

void map_delete(struct map_t *this)
{
    if(!this->head)
        return;

    struct map_node *entry = this->head;
    struct map_node *free_entry;

    while(entry->next)
    {
        free_entry = entry;
        entry = entry->next;
        free(free_entry->key);
        free(free_entry);
    }
    free(entry->key);
    free(entry);

    this->head = 0;
    this->tail = 0;
    this->size = 0;
}

void map_delete_all(struct map_t *this)
{
    if(!this->head)
        return;

    struct map_node *entry = this->head;
    struct map_node *free_entry;

    while(entry->next)
    {
        if(!entry->data)
            free(entry->data);

        free_entry = entry;
        entry = entry->next;
        free(free_entry->key);
        free(free_entry);
    }
    if(!entry->data)
        free(entry->data);
    free(entry->key);
    free(entry);

    this->head = 0;
    this->tail = 0;
    this->size = 0;
}

//note: key get saved in map_node without 0 terminator

void map_push_front(struct map_t *this, char *key, void *data)
{
    struct map_node *entry = malloc(sizeof(struct map_node));

    entry->data = data;
    entry->key = malloc(sizeof(char) * strlen(key));
    memcpy(entry->key, key, strlen(key));
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

void map_push_back(struct map_t *this, char *key, void *data)
{
    struct map_node *entry = malloc(sizeof(struct map_node)); 

    entry->data = data;
    entry->key = malloc(sizeof(char) * strlen(key));
    memcpy(entry->key, key, strlen(key));
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

void *map_pop_front(struct map_t *this)
{
    if(!this->head)
        return 0;

    struct map_node *entry = this->head;
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

void *map_pop_back(struct map_t *this)
{
    if(!this->head)
        return 0;

    struct map_node *entry = this->tail;
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

void *map_get(struct map_t *this, char *key)
{
    if(!this->head)
        return 0;

    struct map_node *entry;
    entry = this->head;
    int rescomp = 1;
    while((rescomp = strncmp(entry->key, key, strlen(key))) && entry->next)
    {
        entry = entry->next;     
    }

    if(!rescomp)
        return entry->data;
    else
        return 0;
}

void *map_at(struct map_t *this, int x)
{
    if(!this->head)
        return 0;

    struct map_node *entry;
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

void *map_remove(struct map_t *this, char *key)
{
    if(!this->head)
        return 0;

    void *data;

    struct map_node **entry;
    entry = &this->head;
    int rescmp = 1;
    while((rescmp = strncmp((*entry)->key, key, strlen(key))) && (*entry)->next)
    {
        entry = &(*entry)->next;
    }
    if(!rescmp)
    {
        if((*entry)->next )
        {
            (*entry)->next->prev = (*entry)->prev;
        }
        else
        {
            this->tail = (*entry)->prev;
        }

        data = (*entry)->data;

        struct map_node *free_entry = *entry;

        *entry = (*entry)->next;

        free(free_entry);
        this->size--;

        if(!this->size)
        {
            this->head = 0;
            this->tail = 0;
        }

        return data;
    }
    else
    {
        return 0;
    }

}

void *map_remove_at(struct map_t *this, int x)
{
    if(!this->head)
        return 0;

    void *data;

    struct map_node **entry;
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

        struct map_node *free_entry = *entry;

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

        struct map_node *free_entry = *entry;

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

void *map_for_each(struct map_t *this, void *(*fun)(void *data, void *ref, struct map_info *info), void *ref)
{
    if(!(this->head))
        return 0;

    struct map_node **entry = &(this->head);

    void *retval = 0;

    struct map_info info;
    info.this = this;
    info.entry = *entry;
    info.pos = 0;
    info.size = this->size;
    info.key = (*entry)->key;

    while(!(retval = fun((*entry)->data, ref, &info)) && (*entry)->next)
    {
        entry = &(*entry)->next;

        info.entry = *entry;
        info.pos++;
        info.key = (*entry)->key;
    }
    return retval;
}
