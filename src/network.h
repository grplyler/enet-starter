#ifndef NETWORK_H
#define NETWORK_H
#include <enet/enet.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MESSAGE_SIZE 512

// Enum for message types
typedef enum {
    MT_DRAW_COMMAND,
    MT_GAME_JOIN,
    MT_COUNTER
    // Add more message types here as needed
} MessageType;

// Struct for DrawCommand
typedef struct {
    uint32_t x;
    uint32_t y;
    uint32_t color;
} DrawCommand;

// Struct for GameJoin
typedef struct {
    char username[32];
    uint32_t player_id;
} GameJoin;

typedef struct {
    uint32_t counter;
} Counter;

// Union for message data
typedef union {
    DrawCommand drawCommand;
    GameJoin gameJoin;
    Counter counter;
    // Add more message structs here as needed
} MessageData;


// Struct for the complete message
typedef struct {
    MessageType type;
    MessageData data;
} Message;

Message MakeCounterMessage(uint32_t counter) {
    Message message;
    message.type = MT_COUNTER;
    message.data.counter.counter = counter;
    return message;
}

Message MakeDrawCommandMessage(uint32_t x, uint32_t y, uint32_t color) {
    Message message;
    message.type = MT_DRAW_COMMAND;
    message.data.drawCommand.x = x;
    message.data.drawCommand.y = y;
    message.data.drawCommand.color = color;
    return message;
}

Message MakeGameJoinMessage(const char *username, uint32_t player_id) {
    Message message;
    message.type = MT_GAME_JOIN;
    strcpy(message.data.gameJoin.username, username);
    message.data.gameJoin.player_id = player_id;
    return message;
}

// Function to serialize a message
void EncodeMessage(const Message *message, uint8_t *buffer, size_t *length) {
    memcpy(buffer, &message->type, sizeof(MessageType));
    *length = sizeof(MessageType);

    switch (message->type) {
        case MT_DRAW_COMMAND:
            memcpy(buffer + *length, &message->data.drawCommand, sizeof(DrawCommand));
            *length += sizeof(DrawCommand);
            break;
        case MT_GAME_JOIN:
            memcpy(buffer + *length, &message->data.gameJoin, sizeof(GameJoin));
            *length += sizeof(GameJoin);
            break;
        case MT_COUNTER:
            memcpy(buffer + *length, &message->data.counter, sizeof(Counter));
            *length += sizeof(Counter);
            break;
        // Handle other message types here as needed
    }
}

// Function to deserialize a message
void DecodeMessage(const uint8_t *buffer, size_t length, Message *message) {
    memcpy(&message->type, buffer, sizeof(MessageType));
    size_t offset = sizeof(MessageType);

    switch (message->type) {
        case MT_DRAW_COMMAND:
            memcpy(&message->data.drawCommand, buffer + offset, sizeof(DrawCommand));
            break;
        case MT_GAME_JOIN:
            memcpy(&message->data.gameJoin, buffer + offset, sizeof(GameJoin));
            break;
        case MT_COUNTER:
            memcpy(&message->data.counter, buffer + offset, sizeof(Counter));
            break;
        // Handle other message types here as needed
    }
}

void SendMessage(ENetPeer *peer, const Message *message) {
    size_t length;
    uint8_t buffer[MAX_MESSAGE_SIZE];
    EncodeMessage(message, buffer, &length);

    ENetPacket *packet = enet_packet_create(buffer, length, ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);
}
#endif


// int main(int argc, char **argv) {
//     // Initialize ENet
//     if (enet_initialize() != 0) {
//         fprintf(stderr, "An error occurred while initializing ENet.\n");
//         return EXIT_FAILURE;
//     }
//     atexit(enet_deinitialize);

//     // Example usage
//     Message message;
//     message.type = MESSAGE_TYPE_GAME_JOIN;
//     strcpy(message.data.gameJoin.username, "PlayerOne");
//     message.data.gameJoin.player_id = 1;

//     uint8_t buffer[MAX_MESSAGE_SIZE];
//     size_t length;

//     EncodeMessage(&message, buffer, &length);

//     // Simulate sending and receiving the message over ENet
//     Message receivedMessage;
//     DecodeMessage(buffer, length, &receivedMessage);

//     // Print the received message
//     if (receivedMessage.type == MESSAGE_TYPE_GAME_JOIN) {
//         printf("Received GameJoin message: username=%s, player_id=%u\n",
//                receivedMessage.data.gameJoin.username,
//                receivedMessage.data.gameJoin.player_id);
//     }

//     return 0;
// }
