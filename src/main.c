/**
 * AngstromDB server.
 *
 * @author	Amaury Bouchard <amaury@amaury.net>
 * @copyright	© 2013, Amaury Bouchard
 */
#include "angstrom.h"

/**
 * Main function of the program.
 */
int main(int argc, char *argv[]) {
	angstrom_t *angstrom;
	char *path = DEFAULT_DB_PATH;
	int i;

	// server init
	angstrom = calloc(1, sizeof(angstrom_t));
	angstrom->socket = angstrom->threads_socket = -1;
	angstrom->comm_threads = calloc(NBR_THREADS, sizeof(comm_thread_t));
	// open the database
	if (argc > 1)
		path = argv[1];
	if (!(angstrom->db = database_open(path, DEFAULT_MAPSIZE, NBR_THREADS))) {
		fprintf(stderr, "Unable to open database\n");
		exit(1);
	}
	// create the nanomsg socket for threads communication
	if ((angstrom->threads_socket = nn_socket(AF_SP, NN_PUSH)) < 0 ||
	    nn_bind(angstrom->threads_socket, ENDPOINT_THREADS_SOCKET) < 0) {
		fprintf(stderr, "Unable to create threads socket.\n");
		mdb_env_close(angstrom->db);
		exit(2);
	}
	// create the writer thread
	if (pthread_create(&angstrom->writer_tid, NULL, writer_loop, angstrom)) {
		fprintf(stderr, "Unable to create writer thread.\n");
		mdb_env_close(angstrom->db);
		exit(3);
	}
	// create connection threads
	for (i = 0; i < NBR_THREADS; i++) {
		comm_thread_t *thread = &(angstrom->comm_threads[i]);

		thread->client_sock = -1;
		thread->angstrom = angstrom;
		pthread_create(&thread->tid, 0, comm_thread_loop, thread);
		pthread_detach(thread->tid);
	}
	// server loop
	main_thread_loop(angstrom);
	return (0);
}
