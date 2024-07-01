#include <enet/enet.h>
#include <stdio.h>
#include <string.h>
#include "network.h"

void HandleMessage(ENetPeer *peer, Message *message) {
    switch (message->type) {
        case MT_GAME_JOIN:
            printf("[GAMEJOIN] %s with player_id %d.\n", message->data.gameJoin.username, message->data.gameJoin.player_id);
            break;
        case MT_COUNTER:
            printf("[COUNTER] %d.\n", message->data.counter.counter);
            break;
        case MT_DRAW_COMMAND:
            printf("[DRAWCOMMAND] x: %d, y: %d, color: %d.\n",
                message->data.drawCommand.x,
                message->data.drawCommand.y,
                message->data.drawCommand.color
            );
            break;
        default:
            printf("Received unknown message type.\n");
            break;
    }
}

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

                Message message;
                DecodeMessage(event.packet->data, event.packet->dataLength, &message);
                HandleMessage(event.peer, &message);
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
