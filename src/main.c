#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define BUFSIZE 255

void writePrompt(void) {
    if (write(STDOUT_FILENO, "$ ", 2) == -1) {
        perror("write failed");
        _exit(1);
    }
}

ssize_t readUserInput(char* buf) {
    ssize_t numBytesRead;

    numBytesRead = read(STDIN_FILENO, buf, BUFSIZE);
    if (numBytesRead < 0) {
        perror("read failed");
        _exit(1);
    } else if (numBytesRead == 0) {
        write(STDOUT_FILENO, "goodbye\n", 8);
        _exit(0);
    } else{
        buf[numBytesRead] = '\0';
    }

    return numBytesRead;
}

void writeUserInput(char* buf, ssize_t numBytesRead) {
    if (strncmp(buf, "exit", numBytesRead) == 0) {
        write(STDOUT_FILENO, "goodbye\n", 8);
        _exit(0);
    }
    if (write(STDOUT_FILENO, buf, numBytesRead) == -1) {
        perror("write failed");
        _exit(1);
    }

    if (write(STDOUT_FILENO, "\n", 1) == -1) {
        perror("write failed");
        _exit(1);
    }
}

int main(void) {
    char buf[BUFSIZE+1];
    ssize_t numBytesRead;

    while (1) {
        writePrompt();
        numBytesRead = readUserInput(buf);
        writeUserInput(buf, numBytesRead);
    }

    return 0;
}
