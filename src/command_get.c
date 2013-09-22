/**
 * GET command execution.
 *
 * @author	Amaury Bouchard <amaury@amaury.net>
 * @copyright	© 2013, Amaury Bouchard
 */
#include "angstrom.h"

/* GET command execution. */
void command_get(comm_thread_t *thread) {
	uint8_t key_size, response = PROTO_OK;
	uint32_t value_size;
	MDB_val key, value;

	// read key length
	read(thread->client_sock, &key_size, sizeof(key_size));
	// read key data
	key.mv_data = malloc(key_size);
	read(thread->client_sock, key.mv_data, key_size);
	// get data
	key.mv_size = (size_t)key_size;
	database_get(thread->angstrom->db, &key, &value);
	// send response to the client
	write(thread->client_sock, &response, sizeof(response));
	value_size = htonl((uint32_t)value.mv_size);
	write(thread->client_sock, &value_size, sizeof(value_size));
	if (value_size)
		write(thread->client_sock, value.mv_data, value.mv_size);
}
