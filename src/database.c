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

	mdb_env_create(&env);
	mdb_env_set_mapsize(env, mapsize);
	mdb_env_set_maxreaders(env, nbr_threads);
	mdb_env_open(env, path, 0, 0664);
	return (env);
}

// Close a database and free its structure.
void database_close(MDB_env *env) {
	mdb_env_close(env);
}

// Add or update a key in database.
void database_put(MDB_env *db, MDB_val *key, MDB_val *value) {
	MDB_dbi dbi;
	MDB_txn *txn;

	// transaction init
	mdb_txn_begin(db, NULL, 0, &txn);
	// open database in read-write mode
	mdb_dbi_open(txn, NULL, 0, &dbi);
	// put data
	mdb_put(txn, dbi, key, value, 0);
	// close database
	mdb_dbi_close(db, dbi);
	// transaction commit
	mdb_txn_commit(txn);
}

// Remove a key from database.
void database_del(MDB_env *db, MDB_val *key) {
	MDB_dbi dbi;
	MDB_txn *txn;

	// transaction init
	mdb_txn_begin(db, NULL, 0, &txn);
	// open database in read-write mode
	mdb_dbi_open(txn, NULL, 0, &dbi);
	// delete key
	mdb_del(txn, dbi, key, NULL);
	// close database
	mdb_dbi_close(db, dbi);
	// transaction commit
	mdb_txn_commit(txn);
}

// Return a value from database.
void database_get(MDB_env *db, MDB_val *key, MDB_val *value) {
	MDB_dbi dbi;
	MDB_txn *txn;

	// transaction init
	mdb_txn_begin(db, NULL, MDB_RDONLY, &txn);
	// open database in read-write mode
	mdb_dbi_open(txn, NULL, 0, &dbi);
	// get data
	if (mdb_get(txn, dbi, key, value))
		bzero(value, sizeof(*value));
	// end of transaction
	mdb_txn_abort(txn);
	// close database
	mdb_dbi_close(db, dbi);
}
