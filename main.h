#pragma once
#ifndef LIBRARY_H
#define LIBRARY_H

#include <stdio.h>
#include <string.h>
#include <Windows.h>

// Кодировка для консоли, чтобы корректно выводились кириллические символы без дополнительных манипуляций
#define RUS_ENCODING 1251

typedef struct _book {
	char* title; // Название книги
	char* author; // Автор или авторы
	int issueYear; // Год выпуска int, потому что существуют книги, написанные до нашей эры
	char* publishingHouse; // Издательство
	float readersScore; // Оценка читателей по шкале от 1 до 10, может быть дробной, к примеру 5,5
	size_t price; // Текущая цена книги
	char* genre; // Жанр повествования
	char* shortDescription; // Краткое описание
} Book;

// Выводит справку для пользователя (описание функций программы и как юзер может ими воспользоваться)
void usage();

#endif // !LIBRARY_H
