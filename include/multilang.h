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

#ifndef MULTILANG_H
#define MULTILANG_H

/* Set english by default */
#define ENGLISH

/* Define English language output */
#ifdef ENGLISH

/* Define text constant here */
/* Absolute first dialog then we choose create new user or auth */
const char init_message[] = "Choose action:\n";
const char auth_message[] = "\n=Authorization=\n";
const char init_error[] = "Wrong answer! End programm...\n";
const char init_dialog[] = "\
1. Use existing login and password\n\
2. Create new user\n\
3. Exit\n";

/* Final message */
const char finish_message[] = "Exiting...Goodbye!\n";

/* After auth or creating new user */ 
const char start_dialog[] = "\
1. List saved passwords\n\
2. Add new password\n\
3. Edit password\n\
4. Remove password\n\
5. Add new account\n\
6. Edit account\n";
const char input_login[] = "Input login=>:\n";
const char input_password[] = "Input password=>:\n";
const char create_message[] = "\n=Creating new user=\n";
const char open_menu_message[] = "\n==Choose action:==\n";
const char create_login[] = "Input new login name=>:\n";
const char create_password[] = "Input password for new user=>:\n";

#endif

/* Define Russian language output */
#ifdef RUSSIAN

/* Define text constant here */
/* Absolute first dialog then we choose create new user or auth */
const char init_message[] = "Выберите опцию:\n";
const char auth_message[] = "\n=Авторизация=\n";
const char init_error[] = "Вы выбрали неправильный вариант. Конец программы...\n";
const char init_dialog[] = "\
1. Авторизоваться по существующему логину и паролю\n\
2. Создать нового пользователя\n\
3. Выход\n";

/* Final message */
const char finish_message[] = "Завершение работы программы...Всего доброго!\n";

/* After auth or creating new user */ 
const char start_dialog[] = "\
1. Просмотр сохраненных записей\n\
2. Добавление новой записи\n\
3. Изменение записи\n\
4. Удаление записи\n\
5. Добавить нового пользователя\n\
6. Изменить данные пользователя\n";
const char input_login[] = "Введите имя пользователя=>:\n";
const char input_password[] = "Введите пароль=>:\n";
const char create_message[] = "\n=Создание нового пользователя=\n";
const char open_menu_message[] = "\n==Выберите действие:==\n";
const char create_login[] = "Придумайте имя для пользователя=>:\n";
const char create_password[] = "Придумайте пароль для пользователя=>:\n";

#endif

#endif
