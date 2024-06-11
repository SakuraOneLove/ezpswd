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

char *db_path;
sqlite3 *db;

int init_db(const char* path)
{
	int rc;
	char *err_msg;
	/* Initialize sql queries */
	const char *create_table_user_query = \
		"create table if not exists user (id integer primary key autoincrement, login text, password text);";
	/* Allocate memory for path name */
	db_path = malloc(strlen(path) * sizeof(char) + 1);
	/* Copy path to db_path */
	strcpy(db_path, path);
	/* Open connection */
	rc = sqlite3_open(db_path, &db);	
	if (rc != SQLITE_OK) {
		printf("Can't connect to %s error code %d\n", path, rc);
		return rc;
	}
	/* Create default tables */
	rc = sqlite3_exec(db, create_table_user_query, 0, 0, &err_msg);
	if (rc != SQLITE_OK) {
		printf("Error while executing 'create user table':\n%s\n", err_msg);
		sqlite3_free(err_msg);
		return rc;
	}
	return 0;
}

int finish_db()
{
	/* Free memory */
	free(db_path);
	/* Close connection to database */
	if (db != NULL) {
		sqlite3_close(db);
	}
	return 0;
}

/* 
 * Realize some SQL operations,
 * such as Insert, Drop, Update and etc.
 */

/* For User table */
int insert_into_user(const char *login, const char *password)
{
	sqlite3_stmt *pstmt;
	int rc;
	const char *add_user_query = "insert into user(login, password) values(?, ?);";
	EVP_CIPHER_CTX* en = EVP_CIPHER_CTX_new();
	EVP_CIPHER_CTX* de = EVP_CIPHER_CTX_new();
	/* Create statement */
	rc = sqlite3_prepare_v2(db, add_user_query, -1, &pstmt, NULL); 
	if (rc != SQLITE_OK) {
		printf("Can't prepare statement 'insert into user', error code: %d\n", rc);
		return rc;
	}
	/* Bind parameters to sql query */
	sqlite3_bind_text(pstmt, 1, login, -1, NULL);
	sqlite3_bind_text(pstmt, 2, password, -1, NULL);
	/* Prepare statement */
	rc = sqlite3_step(pstmt);
	if (rc == SQLITE_ERROR) {
		printf("Can't execute sql statement 'insert into user', error code: %d\n", rc);
		return rc;
	}
	sqlite3_finalize(pstmt);
	return 0;
}

void print_db_name(void)
{
	printf("Database name: %s\n", db_path);
}
