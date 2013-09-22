/**
 * AngstromDB writer thread.
 *
 * @author	Amaury Bouchard <amaury@amaury.net>
 * @copyright	© 2013, Amaury Bouchard
 */
#include "angstrom.h"

// Writer thread's execution loop.
void *writer_loop(void *param) {
	angstrom_t *angstrom = param;
	int socket;

	// create the nanomsg socket for threads communication
	socket = nn_socket(AF_SP, NN_PULL);
	nn_bind(socket, ENDPOINT_WRITER_SOCKET);
	// loop to process new connections
	for (; ; ) {
		writer_msg_t *msg;

		// waiting for a new connection to handle
		if (nn_recv(socket, &msg, sizeof(writer_msg_t*), 0) < 0)
			continue;
		// processing
		switch (msg->type) {
		case WRITE_PUT:
			database_put(angstrom->db, &msg->key, &msg->value);
			break;
		case WRITE_DEL:
			database_del(angstrom->db, &msg->key);
			break;
		}
		// free data
		if (msg->key.mv_data)
			free(msg->key.mv_data);
		if (msg->value.mv_data)
			free(msg->value.mv_data);
		free(msg);
	}
	return (NULL);
}

