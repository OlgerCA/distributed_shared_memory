
#ifndef DISTRIBUTED_SHARED_MEMORY_LOGGER_H
#define DISTRIBUTED_SHARED_MEMORY_LOGGER_H

typedef enum
{
    INFO, WARNING, ERROR
} Level;

void logger_init(char* filename);
void logger_log_message(char* message, Level level);
void logger_close();

#endif
