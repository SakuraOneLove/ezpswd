/*
 * Copyright (c) 2024 SakuraOneLove Moscow
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *		list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this
 *		list of conditions and the following disclaimer in the documentation and/or other
 *		materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its contributors may
 *		be used to endorse or promote products derived from this software without specific
 *		prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS” AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * Usefull links:
 * static c: https://habr.com/ru/articles/527044/
 * sqlite3_prepare usage: https://github.com/libretro/Craft/blob/34c1341caa094ff4fe4f06e8ceb6ec316062b1fa/src/db.c#L171
 * another example: https://lloydrochester.com/post/c/sqlite-api/
 * full usage of c/c++ sqlite interface: https://renenyffenegger.ch/notes/development/databases/SQLite/c-interface/basic/index
 */

/* Create tables, store users and data */

#include "ezdb.h"

#define KEY_SIZE 50
/* Sqlite3 data */
sqlite3 *db;
/* Sqlite3 statements */
sqlite3_stmt *insert_into_storage_stmt;
char *db_path;
char *key_data;
static int key_data_len;
/*
 * Auth user status:
 * 0 - authorized user
 * 1 - default value (no check user auth)
 * 2 - user is not authorized
 */
static int user_status; 
static int user_id;

int init_db(const char* path)
{
	int rc;
	char *err_msg;
	/* Initialize sql queries */
	const char *create_table_queries[] = {
		"create table if not exists user (id integer primary key autoincrement, \
			login text unique not null, password text not null);",
		"create table if not exists storage (id integer primary key autoincrement, \
			name text not null, login text not null, password text not null, user_id integer not null);"
		};
	const size_t arr_tables_len = sizeof(create_table_queries) / sizeof(create_table_queries[0]); 
	/* Allocate memory for path name */
	db_path = malloc((strlen(path) + 1) * sizeof(char));
	/* Copy path to db_path */
	strcpy(db_path, path);
	/* Open connection */
	rc = sqlite3_open(db_path, &db);	
	if (rc != SQLITE_OK) {
		printf("Can't connect to %s error code %d\n", path, rc);
		return rc;
	}
	/* Create default tables */
	for (int i = 0; i < (const int)arr_tables_len; i++) {
		rc = sqlite3_exec(db, create_table_queries[i], 0, 0, &err_msg);
		if (rc != SQLITE_OK) {
			printf("Error while executing '%s':\n%s\n", create_table_queries[i], err_msg);
			sqlite3_free(err_msg);
			return rc;
		}
	}
	/* Init symmetrical key for encryption */
	key_data = (char*)malloc(sizeof(char) * KEY_SIZE);
	/* Init other variables */
	user_status = 1;
	user_id = -1;
	return 0;
}

/* Init statements for operation with 'storage' table */
int init_stmt()
{
	int rc = 0;
	const char* insert_into_storage_query = \
						"insert into storage (name, login, password, user_id) values (?, ?, ?, ?);";
	/* Initialization of statements */
	rc = sqlite3_prepare_v2(db, insert_into_storage_query, -1, &insert_into_storage_stmt, NULL);
	if (rc != SQLITE_OK) {
		printf("Can't prepare statement '%s', error code: %d\n", insert_into_storage_query, rc);
		goto err_label;
	}
err_label:
	return rc;
}

/* Free memory after prepared statemts */
void finish_stmt(void)
{
	/* Destroy statemts */
	sqlite3_finalize(insert_into_storage_stmt);
}

int finish_db()
{
	/* Free memory */
	free(db_path);
	free(key_data);
	/* Close connection to database */
	if (db != NULL) {
		sqlite3_close(db);
	}
	return 0;
}

/* 
 * Returned status codes for 'auth_user':
 * 0 - "ok" user authorized
 * negative num - not authorized
 * positive num - not authorized
 */
int auth_user(const char *login, const char *password)
{
	sqlite3_stmt *fstmt;
	int rc, buffer_size = 65;
	char *returned_password;
	char *sha_password_buffer = (char*)malloc(sizeof(char) * buffer_size);
	const char *find_user_by_login = "select password, id from user where login=?;"; 
	/* Calculate checksum */
	sha256_digest((const unsigned char*)password, strlen(password), sha_password_buffer);
	/* Create statement */
	rc = sqlite3_prepare_v2(db, find_user_by_login, -1, &fstmt, NULL); 
	if (rc != SQLITE_OK) {
		printf("Can't prepare statement '%s', error code: %d\n", find_user_by_login, rc);
		return rc;
	}
	/* Bind parameters to sql query */
	sqlite3_bind_text(fstmt, 1, login, -1, NULL);
	/* Get password by login */
	if(sqlite3_step(fstmt) == SQLITE_ROW){
		returned_password	= (char*)sqlite3_column_text(fstmt, 0);
		rc = strcmp(sha_password_buffer, returned_password);
		/* Set user auth status code */
		if (rc == 0) {
			user_status = 0;
			strcpy(key_data, password);
			key_data_len = (int)strlen(key_data);
			user_id = sqlite3_column_int(fstmt, 1);
		}
		else {
			user_status = 2;
		}
	}
		else {
		user_status = 2;
		rc = 1;
	}
	/* Free memory */
	free(sha_password_buffer);
	sqlite3_finalize(fstmt);
	return rc;
}
/*
 * Realize some SQL operations,
 * such as Insert, Drop, Update and etc.
 */

/* For User table */
int insert_into_user(const char *login, const char *password)
{
	sqlite3_stmt *pstmt;
	int rc, buffer_size = 65;
	char *sha_password_buffer = (char*)malloc(sizeof(char) * buffer_size);
	const char *add_user_query = "insert into user(login, password) values(?, ?);";
	/* Get sha256 checksum */
	sha256_digest((const unsigned char*)password, strlen(password), sha_password_buffer);
	/* Create statement */
	rc = sqlite3_prepare_v2(db, add_user_query, -1, &pstmt, NULL);
	if (rc != SQLITE_OK) {
		printf("Can't prepare statement 'insert into user', error code: %d\n", rc);
		return rc;
	}
	/* Bind parameters to sql query */
	sqlite3_bind_text(pstmt, 1, login, -1, NULL);
	sqlite3_bind_text(pstmt, 2, sha_password_buffer, -1, NULL);
	/* Prepare statement */
	rc = sqlite3_step(pstmt);
	if (rc == SQLITE_ERROR) {
		printf("Can't execute sql statement 'insert into user', error code: %d\n", rc);
		return rc;
	}

	/* Free memory */
	free(sha_password_buffer);
	sqlite3_finalize(pstmt);
	return 0;
}

int insert_into_storage(const char *name, const char *login, const char *password)
{
	int len, rc;
	unsigned int salt[] = {64353, 95375};
	unsigned char *ciphertext;
	/* "opaque" encryption, decryption ctx structures that libcrypto uses to record
		 status of enc/dec operations */
	EVP_CIPHER_CTX* en = EVP_CIPHER_CTX_new();
	EVP_CIPHER_CTX* de = EVP_CIPHER_CTX_new();
	/* 8 bytes to salt the key_data during key generation. This is an example of
		 compiled in salt. We just read the bit pattern created by these two 4 byte
		 integers on the stack as 64 bits of contigous salt material -
		 ofcourse this only works if sizeof(int) >= 4 */
	/* gen key and iv. init the cipher ctx object */
	if (aes_init((unsigned char*)key_data, key_data_len, (unsigned char *)&salt, en, de)) {
		printf("Couldn't initialize AES cipher\n");
		return -1;
	}
	/* encrypt password */
	/* The enc/dec functions deal with binary data and not C strings. strlen() will
		 return length of the string without counting the '\0' string marker. We always
		 pass in the marker byte to the encrypt/decrypt functions so that after decryption
		 we end up with a legal C string */
	len = strlen(password) + 1;
	ciphertext = aes_encrypt(en, (unsigned char *)password, &len);
	/* Bind parameters to sql query */
	sqlite3_bind_text(insert_into_storage_stmt, 1, name, -1, NULL);
	sqlite3_bind_text(insert_into_storage_stmt, 2, login, -1, NULL);
	sqlite3_bind_text(insert_into_storage_stmt, 3, (const char*)ciphertext, -1, NULL);
	sqlite3_bind_int(insert_into_storage_stmt, 4, user_id);
	/* Prepare statement */
	rc = sqlite3_step(insert_into_storage_stmt);
	if (rc == SQLITE_ERROR) {
		printf("Can't execute sql statement 'insert into storage', error code: %d\n", rc);
		return rc;
	}
	/* Reset statement */
	sqlite3_reset(insert_into_storage_stmt);
	/* Free memory */
	free(ciphertext);
	EVP_CIPHER_CTX_free(en);
	EVP_CIPHER_CTX_free(de);
	return 0;
}

void print_db_name(void)
{
	printf("Database name: %s\n", db_path);
}
