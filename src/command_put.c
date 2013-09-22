/**
 * PUT command execution.
 *
 * @author	Amaury Bouchard <amaury@amaury.net>
 * @copyright	© 2013, Amaury Bouchard
 */
#include "angstrom.h"

/* PUT command execution. */
void command_put(comm_thread_t *thread) {
	uint8_t key_size, response = PROTO_OK;
	void *key, *value = NULL;
	uint32_t value_size;
	writer_msg_t *msg;

	// read key length
	read(thread->client_sock, &key_size, sizeof(key_size));
	// read key data
	key = malloc(key_size);
	read(thread->client_sock, key, key_size);
	// read value length
	read(thread->client_sock, &value_size, sizeof(value_size));
	value_size = ntohl(value_size);
	if (value_size > 0) {
		// read value data
		value = malloc(value_size);
		read(thread->client_sock, value, value_size);
	}
	// create message
	msg = malloc(sizeof(writer_msg_t));
	msg->type = WRITE_PUT;
	msg->key.mv_size = (size_t)key_size;
	msg->key.mv_data = key;
	msg->value.mv_size = (size_t)value_size;
	msg->value.mv_data = value;
	// send the message to the writer thread
	nn_send(thread->writer_sock, &msg, sizeof(msg), 0);
	// send response to the client
	write(thread->client_sock, &response, sizeof(response));
}
