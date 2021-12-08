/* server.c
 *
 * Copyright (C) 2021 wolfSSL Inc.
 *
 * This file is part of wolfSSL.
 *
 * wolfSSL is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * wolfSSL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA
 */

#include "common.h"

extern volatile int keep_running;

int main(int argc, char *argv[])
{
    WOLFSSL_CTX *ctx = NULL;
    WOLFSSL_METHOD* method = NULL;
    WOLFSSL* ssl = NULL;
    int ret;

    if (argc != 2) {
        printf("Usage: ./receive <CAN interface>\n");
        return -1;
    }

    ret = setup_connection(argv[1], SERVER_ID, CLIENT_ID);
    if (ret) {
        return ret;
    }

    ret = setup_ssl(SERVICE_TYPE_SERVER, &ctx, &method, &ssl);
    if (ret) {
        return ret;
    }

    while(keep_running) {
        char reply[64];
        int input = wolfSSL_read(ssl, reply, sizeof(reply));
        if (input > 0) {
            printf("\n\nGot message: %.*s\n", input, reply);
        }
    }

    close_ssl(ctx, ssl);

    return 0;
}
