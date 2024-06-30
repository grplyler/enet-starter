#ifndef MSG_H
#define MSG_H

#include <stdio.h>
#include <enet/enet.h>
#include <string.h>
#include <stdlib.h>

const unsigned char DT_INT = 0;
const unsigned char DT_FLOAT = 1;
const unsigned char DT_STRING = 2;
const unsigned char MT_MESSAGE = 0;

typedef struct {
    unsigned char mtype;
    unsigned char dtype;
    unsigned int dsize;
    void *data;
} Message;

void print_msg(Message msg) {
    printf("Message type: %d\n", msg.mtype);
    printf("Data type: %d\n", msg.dtype);
    printf("Data size: %d\n", msg.dsize);

    switch (msg.dtype) {
        case DT_INT:
            printf("Data: %d\n", *(int *)msg.data);
            break;
        case DT_FLOAT:
            printf("Data: %f\n", *(float *)msg.data);
            break;
        case DT_STRING:
            printf("Data: %s\n", (char *)msg.data);
            break;
        default:
            printf("Unknown data type\n");
    }
}

Message parse_msg(ENetPacket *packet) {
    Message msg;
    memcpy(&msg, packet->data, sizeof(Message));

    msg.data = malloc(msg.dsize);
    memcpy(msg.data, packet->data + sizeof(Message), msg.dsize);

    return msg;
}

#endif
