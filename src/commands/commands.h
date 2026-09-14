#pragma once

#include <Arduino.h>

static const char MANUAL_READ_COMMAND[] = "manual_read";

#define COMMAND_QUEUE_SIZE 4

struct Command {
  uint32_t sequence;
  const char *name;
};

struct CommandQueue {
  Command items[COMMAND_QUEUE_SIZE];
  uint8_t count;
  uint32_t nextSequence;
};

void initCommandQueue(CommandQueue &queue);
bool enqueueCommand(CommandQueue &queue, const char *name);
bool peekCommand(const CommandQueue &queue, Command &command);
void removeCommand(CommandQueue &queue);