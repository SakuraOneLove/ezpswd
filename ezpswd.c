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
#include "src/hide_pass.h"

int main(int argc, char *argv[])
{
	/* Text constants here */
	const char dialog_choose[] =\
"1. Просмотр сохраненных записей\n\
2. Добавление новой записи\n\
3. Изменение записи\n\
4. Удаление записи";

	/* Переменные выбора сценария взаимодействия с пользователем */
	int dialog_answ;
	char *login;
	/*
	 * Не забыть освободить память память по указателю 'password',
	 * т.к. она выделена при помощи 'calloc()'.
	 * Освободить при помощи 'free()'.
	 */
	unsigned char *password;

	/* Авторизация */
	/* Ввод логина */
	/*printf("Input login:\n");*/
	/*scanf("%m[^\n]", &login);*/
	/* Ввод пароля */
	/*printf("Input password:\n");*/
	puts("Input password:");
	password = getepass();

	/*printf("Password: %s\n", password);*/
	/*printf("\nLogin: %s\nPassword: %s\n", login, password);*/
	/*free(password);*/
	/*printf("%s\n", password);*/

	/* Печать диалогового меню  выбора дальнейшего действия */
	puts(dialog_choose);
	/* Выбор режима программы */
	/*scanf("%d", &dialog_answ);*/
	

	switch (dialog_answ)
	{ 
		case 1:
			puts("Choose 1");
			break;
		case 2:
			puts("Choose 2");
			break;
		case 3:
			puts("Choose 3");
			break;
		case 4:
			puts("Choose 4");
			break;
		default:
			break;
	}	

	return 0;
}
