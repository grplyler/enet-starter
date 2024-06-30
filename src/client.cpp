#include <enet/enet.h>
#include <stdio.h>
#include <string.h>
#include "msg.h"

void send_packet(ENetPeer *peer, unsigned char mtype, unsigned char dtype, unsigned int dsize, void *data) {
    Message msg;
    msg.mtype = mtype;
    msg.dtype = dtype;
    msg.dsize = dsize;
    msg.data = data;
    
    size_t packet_size = sizeof(Message) + dsize;
    ENetPacket *packet = enet_packet_create(NULL, packet_size, ENET_PACKET_FLAG_RELIABLE);
    memcpy(packet->data, &msg, sizeof(Message));
    memcpy(packet->data + sizeof(Message), data, dsize);

    enet_peer_send(peer, 0, packet);
}

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

    int counter = 0;
    // Send the counter to the server every second then increment it
    while (true) {
        printf("Sending %d to server.\n", counter);
        send_packet(peer, MT_MESSAGE, DT_INT, sizeof(int), &counter);
        enet_host_service(client, &event, 1000);
        counter++;

        if (counter == 5) {
            char *message = "Hello, Server!";
            printf("Sending string to server: %s\n", message);
            send_packet(peer, MT_MESSAGE, DT_STRING, strlen(message) + 1, message); // +1 for null terminator
        }
    }

    enet_host_destroy(client);
    return 0;
}
