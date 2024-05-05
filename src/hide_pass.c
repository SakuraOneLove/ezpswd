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

/* Модуль ввода пароля в зашифрованном виде (со звездочками) */

#include "hide_pass.h"

#define SIZE 100

unsigned char *getepass()
{
	int i = 0;
	int ch;
	struct termios oldt, newt;
	unsigned char *ptr;
	unsigned char buffer[SIZE + 1];

	/* Сохраняем старые настройки терминала */
	tcgetattr(fileno(stdin), &oldt);
	newt = oldt;
	newt.c_lflag &= ~ECHO;

	/* Устанавливаем новые настройки терминала */
	tcsetattr(fileno(stdin), 0, &newt);

	/*
	 * Вводим первый символ без цикла, потому что
	 * может быть возврат строки ('\n') первым символом
	 * и тогда функция сразу завершится.
	 */
	/*ch = getchar();*/
	/*putchar(ch);*/
	/*
	 * Если первый символ '\n', то пропускаем его,
	 * а иначе добавляем в буффер и увеличиваем счетчик.
	 */
	/*if (ch != '\n') {*/
		/*buffer[i] = ch;*/
		/*i++;*/
	/*}*/

	/* Вывод терминала скрыт */
	while (i < 5) {
		ch = getchar();
		i++;
	/*while (((ch = getchar()) != '\n') && (i < SIZE) && (ch != EOF)) {*/
		/*if (ch != '\b') {*/
			/*buffer[i] = ch;*/
			/*i++;*/
		/*} else if (i != 0) {*/
			/*i--;*/
		/*}*/
	}

	/* Выделяем память под строку */
	/*ptr = calloc(i, sizeof(char));*/

	/* Устанавливаем значения в переменную, под которую выделена память */
	for (int n; n < i; n++) {
		putchar(buffer[n]);
		/*ptr[n] = buffer[n];*/
	}
	/* Возвращаем обратно исходные настройки */
	tcsetattr(fileno(stdin), 0, &oldt);

	return NULL;
}
/*
 * Ограничение размера пароля в 100 символов.
 * Вообще 100 символов даже очень много для пароля,
 * но пусть будет так.
 */

char *gethpass()
{
	char buffer[SIZE + 1];
	char symb;
	int count;

	/*
	 * Вводим пароль до тех пор пока не будет нажата клавиша 'Enter'
	 * или не будет достигнут лимит по размеру пароля.
	 */

	while (((symb = getchar())) != '\n' && (count < 100)) {
		buffer[count] = symb;
		/*putchar('\b');*/
		printf("%s", "\b \b");
		count++;
	}

	buffer[count] = '\0'; /* Добавляем последним символом конец строки */

	for (int i = 0; i < count; i++) {
		putchar(buffer[i]);
	}

	return 0;
}
