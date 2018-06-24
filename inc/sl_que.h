#ifndef __SL_QUE_H__
#define __SL_QUE_H__

typedef struct __que_t
{
    int     read;
    int     write;
    int     block_num;
    char    **block;
}que_t;

#define ring_init(head, T, node_count) \
    do{ \
        (head) = (que_t*)malloc(sizeof(que_t)); \
        (head)->block_num = node_count + 1; \
        (head)->read = (head)->write = 0; \
        (head)->block = (char**)malloc(sizeof(T *) * (node_count + 1)); \
    }while(0)

#define can_read(head) ((head)->read != (head)->write)

#define can_write(head) ((head)->read != (((head)->write + 1) % (head)->block_num))

//#define get_read(head, T) ((T*)((unsigned char **)((head)->block) + (head)->read))
#define get_read(head, T) ((T*)(((head)->block)[(head)->read]))

//#define get_write(head, T) ((T*)((unsigned char *)((head)->block) + (head)->write))
#define get_write(head, T) (((head)->block)[(head)->write])

#define read_push(head, T, V) (can_read(head) ? ((V) = get_read(head, T), 1) : 0)

#define read_pop(head) ((head)->read = ((head)->read + 1) % (head)->block_num)

#define write_push(head, T, V) (can_write(head) ? (get_write(head, T) = (V), 1) : 0)

#define write_pop(head) ((head)->write = (((head)->write + 1) % (head)->block_num))

#define read_outval(head, T, V) (can_read(head) ? ((V) = get_read(head, T), read_pop(head), 1):0)

#define write_inval(head, T, V) (can_write(head) ? (get_write(head, T) = (char*)(V), write_pop(head), 1):0)
#endif
