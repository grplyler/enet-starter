#include <enet/enet.h>
#include <stdio.h>
#include <string.h>
#include "msg.h"

int main() {
    if (enet_initialize() != 0) {
        printf("An error occurred while initializing ENet.\n");
        return 1;
    }
    atexit(enet_deinitialize);

    ENetAddress address;
    ENetHost *server;
    address.host = ENET_HOST_ANY;
    address.port = 7777;

    server = enet_host_create(&address, 32, 2, 0, 0);
    if (server == NULL) {
        printf("An error occurred while trying to create an ENet server host.\n");
        return 1;
    }

    printf("Server started on localhost:7777.\n");

    ENetEvent event;
    while (enet_host_service(server, &event, 1000) >= 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT:
                printf("A new client connected from %x:%u.\n", event.peer->address.host, event.peer->address.port);
                break;
            case ENET_EVENT_TYPE_RECEIVE:
                printf("A packet of length %u was received from %p on channel %u.\n",
                    event.packet->dataLength,
                    event.peer->data,
                    event.channelID
                );

                Message msg;
                msg = parse_msg(event.packet);
                print_msg(msg);

                // Free the allocated memory for the message data
                free(msg.data);
                enet_packet_destroy(event.packet);
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                printf("%p disconnected.\n", event.peer->data);
                event.peer->data = NULL;
                break;
        }
    }

    enet_host_destroy(server);
    return 0;
}
