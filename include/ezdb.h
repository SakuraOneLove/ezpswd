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

/* Create tables, store users and data */

#ifndef EZDB_H
#define EZDB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"
#include "ezcrypt.h"

/* Init statements and database */
int init_db(const char*);
int auth_user(const char*, const char*);
/* Finitialize working with database */
int finish_db();
/* Create table for store users */
int create_users_table(const char*);
/* Add user to table */
int create_user(const char*);
/* Sql operations with user table */
int insert_into_user(const char*, const char*);
/* Sql operations with storage table */
int insert_into_storage(const char*, const char*, const char*);
/* Functions for debugging */
void print_db_name(void);
#endif
