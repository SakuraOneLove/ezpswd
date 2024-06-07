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

int init_db(const char* path)
{
	int rc;
	char *err_msg;
	sqlite3 *db;
	/* Initialize sql queries */
	const char *create_table_user_query = \
		"create table if not exists user (id integer primary key autoincrement, login text, password text);";
	/* Allocate memory for path name */
	db_path = malloc(strlen(path) * sizeof(char));
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
	sqlite3_close(db);
	return 0;
}

int finish_db()
{
	/* Free memory */
	free(db_path);
	return 0;
}

/* 
 * Realize some SQL operations,
 * such as Insert, Drop, Update and etc.
 */

/* For User table */
int insert_into_user(const char *login, const char *password)
{
	sqlite3 *db;
	sqlite3_stmt *pstmt;
	int rc;

	const char *add_user_query = "insert into user(login, password) values(?, ?);";

	rc = sqlite3_open("test.db", &db);
	if (rc != SQLITE_OK) {
		printf("Can't connect to %s error code %d\n", db_path, rc);
		return rc;
	}
	rc = sqlite3_prepare_v2(db, add_user_query, -1, &pstmt, NULL); 
	if (rc != SQLITE_OK) {
		printf("Can't prepare statement 'insert into user', error code: %d\n", rc);
		return rc;
	}
	sqlite3_bind_text(pstmt, 1, login, -1, NULL);
	sqlite3_bind_text(pstmt, 2, password, -1, NULL);
	rc = sqlite3_step(pstmt);
	if (rc != SQLITE_OK) {
		printf("Can't execute sql statement 'insert into user', error code: %d\n", rc);
		return rc;
	}
	sqlite3_finalize(pstmt);
	sqlite3_close(db);
	return 0;
}

int test_func(const char *login, const char *password)
{
		int rc;	
		const char *insert_query = "insert into user(login, password) values(?, ?);";
    sqlite3 *db;
		rc = sqlite3_open("test.db", &db);
		printf("Sqlite3_open rc: %d\n", rc);
    if (rc) {
      printf("Could not open the.db\n");
      exit(-1);
    }

 //
 // Prepare a statement for multiple use:
 //
    sqlite3_stmt *stmt;
		rc = sqlite3_prepare_v2(db, insert_query, -1, &stmt, NULL);
		printf("Sqlite3_prepare_v2 rc: %d\n", rc);
    if (rc) {
       printf("Error executing sql statement\n");
       sqlite3_close(db);
       exit(-1);
    }

 //
 // Bind the values for the first insert:
 //
    sqlite3_bind_text(stmt, 1, login, -1, NULL);
    sqlite3_bind_text(stmt, 2, password, -1, NULL);

 //
 // Do the first insert:
 //
    sqlite3_step(stmt);

 //
 // Reset the prepared statement to the initial state.
 // This seems to be necessary in order to
 // use the prepared statement again for another
 // insert:
 //
    /*sqlite3_reset(stmt);*/

 //
 // Bind the values for the second insert
 //
    /*sqlite3_bind_int (stmt, 1, 4      );*/
    /*sqlite3_bind_text(stmt, 2, "four" , -1, NULL);*/
    /*sqlite3_bind_null(stmt, 3);*/

 //
 // To the second insert
 //
    /*sqlite3_step(stmt);*/

 //
 // Get rid of the memory allocated for stmt:
 //
    sqlite3_finalize(stmt);

    sqlite3_close(db);
		return 0;
}

int create_users_table(const char *fname)
{
	/*sqlite3 *db;*/
	/*char *err_msg = 0;*/
	/*int result = 0;*/
	/*int dbcon = 0; [> Database connection status <]*/

	/*dbcon = sqlite3_open(fname, &db);*/

	/*if (dbcon == SQLITE_OK) {*/
		/*result = sqlite3_exec(db, sql_create_users, 0, 0, &err_msg);*/
		/*sqlite3_close(db);*/

		/*if (result != SQLITE_OK)*/
			/*printf("Error while processing sql transaction:\n%s", err_msg);*/
	/*} else {*/
		/*result = dbcon;*/
		/*puts("Can't connect to DB");*/
	/*}*/

 /*return result;*/
	return 0;
}
