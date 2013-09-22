/**
 * AngstromDB database access functions.
 *
 * @author	Amaury Bouchard <amaury@amaury.net>
 * @copyright	© 2013, Amaury Bouchard
 */
#include "angstrom.h"

// Open a LMDB database.
MDB_env *database_open(const char *path, size_t mapsize, unsigned int nbr_threads) {
	MDB_env *env = NULL;
	int rc;

	if ((rc = mdb_env_create(&env)) ||
	    (rc = mdb_env_set_mapsize(env, mapsize)) ||
	    (rc = mdb_env_set_maxreaders(env, nbr_threads)) ||
	    (rc = mdb_env_open(env, path, 0, 0664))) {
		fprintf(stderr, "Unable to open database: '%s'\n", mdb_strerror(rc));
		if (env)
			mdb_env_close(env);
		return (NULL);
	}
	return (env);
}

// Close a database and free its structure.
void database_close(MDB_env *env) {
	mdb_env_close(env);
}

// Add or update a key in database.
uint8_t database_put(MDB_env *db, uint16_t key_size, void *key_data, uint32_t value_size, void *value_data) {
	return (1);
}

// Remove a key from database.
uint8_t database_del(MDB_env *db, uint16_t key_size, void *key_data) {
	return (1);
}
