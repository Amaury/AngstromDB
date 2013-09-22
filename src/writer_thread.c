/**
 * AngstromDB writer thread.
 *
 * @author	Amaury Bouchard <amaury@amaury.net>
 * @copyright	© 2013, Amaury Bouchard
 */
#include "angstrom.h"

/* *** PRIVATE FUNCTIONS *** */
static unsigned char _database_put(MDB_env *db, uint16_t key_size, void *key_data, uint32_t value_size, void *value_data);
static unsigned char _database_del(MDB_env *db, uint16_t key_size, void *key_data);

// Writer thread's execution loop.
void *writer_loop(void *param) {
	angstrom_t *angstrom = param;
	int socket;

	// create the nanomsg socket for threads communication
	if ((socket = nn_socket(AF_SP, NN_PULL)) < 0 ||
	    nn_bind(socket, ENDPOINT_WRITER_SOCKET) < 0) {
		fprintf(stderr, "Unable to create socket in writer thread.");
		exit(6);
	}
	// loop to process new connections
	for (; ; ) {
		writer_msg_t *msg;

		// waiting for a new connection to handle
		if (nn_recv(socket, &msg, sizeof(writer_msg_t*), 0) < 0)
			continue;
		if (msg->type == WRITE_PUT) {
			// add data in database
			if (!_database_put(angstrom->db, msg->key_size, msg->key_data, msg->value_size, msg->value_data))
				fprintf(stderr, "Unable to write data into database.\n");
		} else if (msg->type == WRITE_DEL) {
			// remove data from database
			if (!_database_del(angstrom->db, msg->key_size, msg->key_data))
				fprintf(stderr, "Unable to delete data into database.\n");
		}
		// free data
		if (msg->key_data)
			free(msg->key_data);
		if (msg->value_data)
			free(msg->value_data);
		free(msg);
	}
	return (NULL);
}

static unsigned char _database_put(MDB_env *db, uint16_t key_size, void *key_data, uint32_t value_size, void *value_data) {
	return (1);
}
static unsigned char _database_del(MDB_env *db, uint16_t key_size, void *key_data) {
	return (1);
}
