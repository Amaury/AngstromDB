/**
 * AngstromDB communication threads.
 *
 * @author	Amaury Bouchard <amaury@amaury.net>
 * @copyright	© 2013, Amaury Bouchard
 */
#include "angstrom.h"

void *comm_thread_loop(void *param) {
	comm_thread_t *thread = param;
	int incoming_socket;

	// opening a connection to the writer thread
	if ((thread->writer_sock = nn_socket(AF_SP, NN_PUSH)) < 0 ||
	    nn_connect(thread->writer_sock, ENDPOINT_WRITER_SOCKET) < 0) {
		fprintf(stderr, "[comm] Unable to connect to writer's socket.\n");
		pthread_exit(NULL);
	}
	// opening a connection to the main thread
	if ((incoming_socket = nn_socket(AF_SP, NN_PULL)) < 0 ||
	    nn_connect(incoming_socket, ENDPOINT_THREADS_SOCKET) < 0) {
		fprintf(stderr, "Unable to connect to main thread's socket.\n");
		pthread_exit(NULL);
	}
	// loop to process new connections
	for (; ; ) {
	}
	return (NULL);
}
