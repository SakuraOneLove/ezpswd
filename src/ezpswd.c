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

#include <stdio.h>
#include <stdlib.h>
#include "ezio.h"
#include "ezdb.h"
#include "ezcrypt.h"
#include "multilang.h"

#define LOGIN_SIZE 30
#define PASS_SIZE 50

const char *db_name = "test.db";

int main(int argc, char *argv[])
{
	/* Переменные выбора сценария взаимодействия с пользователем */
	int init_answer;
	int menu_answer;
	/*int start_answer;*/
	char *login;
	char *password;

	/* Allocate memory for login and password */
	login = malloc(LOGIN_SIZE * sizeof(char));
	password = malloc(PASS_SIZE * sizeof(char));

	/* Initialize Sqlite3 data */
	init_db(db_name);
	init_stmt();

	/* Print initial programm message */
	fputs(init_message, stdout);
	fputs(init_dialog, stdout);
	/* Read answer */
	scanf("%d", &init_answer);

	if (init_answer == 1) {
		fputs(auth_message, stdout);
		/* Authorization */
		fputs(input_login, stdout);
		scanf("%s", login);
		/* Clear stdin */
		clear_input();
		/* Input password */
		fputs(input_password, stdout);
		getepass(password, PASS_SIZE);
		/* Add blank line */
		puts("");
		/*[> Check user <]*/
		if (auth_user(login, password) == 0) {
			/* Print initial dialogue */
			fputs(start_dialog, stdout);
			fputs(open_menu_message, stdout);
			scanf("%d", &menu_answer);
			switch (menu_answer) {
				/* List passwords */
				case 1:
					puts("Choose 1");
					break;
				/* Add new password */
				case 2:
					puts("Choose 2");
					break;
				/* Edit password */
				case 3:
					puts("Choose 3");
					break;
				/* Remove password */
				case 4:
					puts("Choose 4");
					break;
				/* Add new user */
				case 5:
					/* Print message */
					break;
				/* Edit user */
				case 6:
					break;
				default:
					break;
			}
		} else {
			puts("User not existing or bad password");
		}

	} else if (init_answer == 2) {
		fputs(create_message, stdout);
		/* Creating new user */
		fputs(input_login, stdout);
		scanf("%s", login);

		/* Clear stdin */
		clear_input();

		fputs(input_password, stdout);
		getepass(password, PASS_SIZE);

		/* Add blank line */
		puts("");
		/* Save user */
		insert_into_user(login, password);
	} else if (init_answer == 3) {
			fputs(finish_message, stdout);
			goto finish_label;
	} else {
		fputs(init_error, stdout);
	}
finish_label:
	/* Destroy Sqlite3 variables */
	finish_db();
	finish_stmt();
	/* Clean memory here */
	free(login);
	free(password);

	return 0;
}
