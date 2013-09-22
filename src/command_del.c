/**
 * DEL command execution.
 *
 * @author	Amaury Bouchard <amaury@amaury.net>
 * @copyright	© 2013, Amaury Bouchard
 */
#include "angstrom.h"

/* DEL command execution. */
void command_del(comm_thread_t *thread) {
	uint8_t key_size, response = PROTO_OK;
	void *key;
	writer_msg_t *msg;

	// read key length
	read(thread->client_sock, &key_size, sizeof(key_size));
	// read key data
	key = malloc(key_size);
	read(thread->client_sock, key, key_size);
	// create message
	msg = calloc(1, sizeof(writer_msg_t));
	msg->type = WRITE_DEL;
	msg->key.mv_size = (size_t)key_size;
	msg->key.mv_data = key;
	// send the message to the writer thread
	nn_send(thread->writer_sock, &msg, sizeof(msg), 0);
	// send response to the client
	write(thread->client_sock, &response, sizeof(response));
}
