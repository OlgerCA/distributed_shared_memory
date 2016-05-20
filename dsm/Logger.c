#include <stdio.h>
#include "Logger.h"

void logger_init(char *filename) {

}

void logger_log_message(char *message, Level level) {
    // TODO Log to real file with datetime information

    puts(message);
}

void logger_close() {

}
