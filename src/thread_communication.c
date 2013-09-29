/**
 * AngstromDB communication threads.
 *
 * @author	Amaury Bouchard <amaury@amaury.net>
 * @copyright	© 2013, Amaury Bouchard
 */
#include "angstrom.h"

/* *** Prototype of private function. *** */
static void _process_connection(comm_thread_t *thread);

/* Communication threads' execution loop. */
void *thread_comm_loop(void *param) {
	comm_thread_t *thread = param;
	int in_sock;

	// opening a connection to the writer thread
	thread->writer_sock = nn_socket(AF_SP, NN_PUSH);
	nn_connect(thread->writer_sock, ENDPOINT_WRITER_SOCKET);
	// opening a connection to the main thread
	in_sock = nn_socket(AF_SP, NN_PULL);
	nn_connect(in_sock, ENDPOINT_THREADS_SOCKET);
	// loop to process new connections
	for (; ; ) {
		// waiting for a new connection to handle
		nn_recv(in_sock, &thread->client_sock, sizeof(int), 0);
		// process connection
		_process_connection(thread);
	}
	return (NULL);
}

/* ****************** PRIVATE FUNCTION **************** */
/**
 * @function	_process_connection
 *		Handle and process an incoming connection.
 * @param	thread	Pointer to the thread's structure.
 */
static void _process_connection(comm_thread_t *thread) {
	uint8_t cmd;

	// loop on incoming requests
	for (; ; ) {
		// read command byte
		if (read(thread->client_sock, &cmd, sizeof(cmd)) <= 0) {
			close(thread->client_sock);
			break;
		}
		// interpret command
		switch (cmd) {
		case PROTO_PUT:
			command_put(thread);
			break;
		case PROTO_DELETE:
			command_delete(thread);
			break;
		case PROTO_GET:
			command_get(thread);
			break;
		}
	}
}
