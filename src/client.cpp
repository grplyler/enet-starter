#include <enet/enet.h>
#include <stdio.h>
#include <string.h>
#include "network.h"


int main() {
    if (enet_initialize() != 0) {
        printf("An error occurred while initializing ENet.\n");
        return 1;
    }

    ENetHost *client;
    ENetAddress address;
    ENetEvent event;

    client = enet_host_create(NULL, 1, 2, 0, 0);
    if (client == NULL) {
        printf("An error occurred while trying to create an ENet client host.\n");
        return 1;
    }

    enet_address_set_host(&address, "localhost");
    address.port = 7777;

    ENetPeer *peer = enet_host_connect(client, &address, 2, 0);
    if (peer == NULL) {
        printf("No available peers for initiating an ENet connection.\n");
        return 1;
    }

    if (enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
        printf("Connection to localhost:7777 succeeded.\n");
    } else {
        enet_peer_reset(peer);
        printf("Connection to localhost:7777 failed.\n");
    }

    // Send GameJoin message
    Message message = MakeGameJoinMessage("PlayerOne", 1);
    SendMessage(peer, &message);

    int counter = 0;
    // Send the counter to the server every second then increment it
    while (true) {
        printf("Sending %d to server.\n", counter);
        Message message = MakeCounterMessage(counter);
        SendMessage(peer, &message);
        enet_host_service(client, &event, 1000);
        counter++;
    }

    enet_host_destroy(client);
    return 0;
}
