// src/commands/commands.cpp

#include "commands.h"

// ---------------------------------
void initCommandQueue(CommandQueue &queue) {
  queue.count = 0;
  queue.nextSequence = 0;
}

bool enqueueCommand(CommandQueue &queue, const char *name) {
  if (queue.count >= COMMAND_QUEUE_SIZE) {
    return false;
  }

  queue.items[queue.count].sequence = queue.nextSequence++;
  queue.items[queue.count].name = name;
  queue.count++;

  return true;
}

bool peekCommand(const CommandQueue &queue, Command &command) {
  if (queue.count == 0) {
    return false;
  }

  command = queue.items[0];
  return true;
}

void removeCommand(CommandQueue &queue) {
  if (queue.count == 0) {
    return;
  }

  for (uint8_t i = 1; i < queue.count; ++i) {
    queue.items[i - 1] = queue.items[i];
  }

  --queue.count;
}
