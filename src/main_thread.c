/**
 * AngstromDB main thread.
 *
 * @author	Amaury Bouchard <amaury@amaury.net>
 * @copyright	© 2013, Amaury Bouchard
 */
#include "angstrom.h"

void main_thread_loop(angstrom_t *angstrom) {
	int fd;
        struct sockaddr_in addr;
        unsigned int addr_size;
        const int on = 1;

        addr_size = sizeof(addr);
        for (; ; ) {
		bzero(&addr, addr_size);
                // accept a new connection
                if ((fd = accept(angstrom->socket, (struct sockaddr*)&addr,
                                 &addr_size)) < 0)
                        continue ;
                setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (void*)&on, sizeof(on));
                // send the file descriptor number to communication threads
		nn_send(angstrom->threads_socket, &fd, sizeof(fd), 0);
        }
}
