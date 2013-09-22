/**
 * @header	angstrom.h
 *
 * Definitions used in the AngstromDB server.
 *
 * @author	Amaury Bouchard <amaury@amaury.net>
 * @copyright	© 2013, Amaury Bouchard
 */
#ifndef __ANGSTROM_H__
#define __ANGSTROM_H__

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <pthread.h>
#include <stdint.h>
#include <arpa/inet.h>
#include "lmdb.h"
#include "nanomsg/nn.h"
#include "nanomsg/pipeline.h"

/* ********** DEFAULT CONFIGURATION VALUES ************ */
/** @const NBR_THREADS Number of communication threads. */
#define NBR_THREADS		10
/** @const DEFAULT_DB_PATH Default path to the database. */
#define DEFAULT_DB_PATH		"../var/database"
/** @const DEFAULT_PORT Default port number. */
#define DEFAULT_PORT		11139
/** @const DEFAULT_MAPSIZE Default map size (10 MB). */
#define DEFAULT_MAPSIZE		10485760
/** @const DEFAULT_TIMEOUT Default connection timeout (30 seconds). */
#define DEFAULT_TIMEOUT		30

/* ********** NANOMSG ENDPOINTS CONFIGURATION *********** */
/** @const ENDPOINT_THREADS_SOCKET Threads' connection endpoint. */
#define ENDPOINT_THREADS_SOCKET	"inproc://threads_socket"
/** @const ENDPOINT_WRITER_SOCKET Writer thread's connection endpoint. */
#define ENDPOINT_WRITER_SOCKET	"inproc://writer_socket"

/* ********** STRUCTURES ************ */
/**
 * @typedef	angstrom_t
 * 		Main structure of the AngstromDB server.
 * @field	db		Pointer to the database environment.
 * @field	socket		Socket descriptor for incoming connections.
 * @field	threads_socket	Nanomsg socket for threads communication.
 * @field	writer_tid	ID of the writer thread.
 * @field	comm_threads	Array of communication threads.
 */
typedef struct angstrom_s {
	MDB_env *db;
	int socket;
	int threads_socket;
	pthread_t writer_tid;
	struct comm_thread_s *comm_threads;
} angstrom_t;

/**
 * @typedef	comm_thread_t
 *		Structure for communication threads.
 * @field	tid		Thread's identifier.
 * @field	angstrom	Pointer to the server's structure.
 * @field	client_sock	Socket used to communicate with the client.
 * @field	writer_sock	Nanomsg socket to send data to the writer thread.
 */
typedef struct comm_thread_s {
	pthread_t tid;
	angstrom_t *angstrom;
	int client_sock;
	int writer_sock;
} comm_thread_t;

/**
 * typedef	writer_action_t
 *		Type of writer's action.
 * @const	WRITE_PUT	Add or update a key in database.
 * @const	WRITE_DEL	Remove a key from database.
 */
typedef enum writer_action_e {
	WRITE_PUT = 0,
	WRITE_DEL
} writer_action_t;

/**
 * @typedef	writer_msg_t
 *		Structure used to transfer data to the writer thread.
 * @field	type		Type of action (WRITE_PUT, WRITE_DEL).
 * @field	key_size	Length of the key.
 * @field	key_data	The key.
 * @field	value_size	Length of the data.
 * @field	value_data	The value.
 */
typedef struct writer_msg_s {
	writer_action_t type;
	uint16_t key_size;
	void *key_data;
	uint32_t value_size;
	void *value_data;
} writer_msg_t;

/* ************** FUNCTIONS *********** */
/* --- main thread --- */
/**
 * @function	main_thread_loop
 *		Main thread's execution loop.
 * @param	angstrom	Pointer to the server's structure.
 */
void main_thread_loop(angstrom_t *angstrom);

/* --- writer thread --- */
/**
 * @function	writer_loop
 *		Writer thread's execution loop.
 * @param	param	Pointer to the server's structure.
 * @return	Always NULL.
 */
void *writer_loop(void *param);

/* --- communication threads --- */
/**
 * @function	comm_thread_loop
 *		Communication threads' execution loop.
 * @param	param	Pointer to the thread's structure.
 * @return	Always NULL.
 */
void *comm_thread_loop(void *param);

/* --- database access --- */
/**
 * @function	database_open
 *		Open a LMDB database.
 * @param	path		Path to the database's data directory.
 * @param	mapsize		Database map size.
 * @param	nbr_threads	Number of threads.
 * @return	A pointer to the allocated environment, or NULL.
 */
MDB_env *database_open(const char *path, size_t mapsize, unsigned int nbr_threads);
/**                                                           
 * Close a database and free its structure.
 * @param	env	A pointer to the database environment.                      
 */
void database_close(MDB_env *env);
/**
 * @function	database_put
 *		Add or update a key in database.
 * @param	db		Pointer to the database environment.
 * @param	key_size	Size of the key.
 * @param	key_data	Key content.
 * @param	value_size	Size of the value.
 * @param	value_data	Value data.
 * @return	1 if OK, 0 otherwise.
 */
uint8_t database_put(MDB_env *db, uint16_t key_size, void *key_data,
                     uint32_t value_size, void *value_data);
/**
 * @function	database_del
 *		Remove a key from database.
 * @param	db		Pointer to the database environment.
 * @param	key_size	Size of the key.
 * @param	key_data	Key content.
 * @return	1 if OK, 0 otherwise.
 */
uint8_t database_del(MDB_env *db, uint16_t key_size, void *key_data);

#endif /* __ANGSTROM_H__ */
