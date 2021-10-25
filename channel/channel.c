
#include "channel.h"

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

struct chan_t {
    // TODO: Your channel struct here...
};

typedef struct chan_t chan_t;

chan_t *chan_create() {
    chan_t *chan = (chan_t *)malloc(sizeof(chan_t));
    return chan;
}

void chan_destroy(chan_t *chan) {
    free(chan);
}

void chan_send_int(chan_t *chan, int i) {
    // TODO: Sending logic
}

int chan_recv_int(chan_t *chan) {
    // TODO: Receiving logic
    return 0;
}