#include "platform/android/jni/MessagePipe.h"
#include <fcntl.h>
#include <unistd.h>
#include <android/log.h>
#include "platform/android/jni/JniCocosActivity.h"

namespace cc{
MessagePipe::MessagePipe() {
    int messagePipe[2] = {0};
    if (pipe(messagePipe)) {
        LOGV("Can not create pipe: %s", strerror(errno));
    }

    _pipeRead  = messagePipe[0];
    _pipeWrite = messagePipe[1];

    if (fcntl(_pipeRead, F_SETFL, O_NONBLOCK) < 0) {
       LOGV("Can not make pipe read to non blocking mode.");
    }
}

MessagePipe::~MessagePipe(){
    close(_pipeRead);
    close(_pipeWrite);
}

void MessagePipe::writeCommand(int8_t cmd) const {
    write(_pipeWrite, &cmd, sizeof(cmd));
}

int MessagePipe::readCommand(int8_t &cmd) const {
    return read(_pipeRead, &cmd, sizeof(cmd));
}

}