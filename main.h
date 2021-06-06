#pragma once
#ifndef LIBRARY_H
#define LIBRARY_H

#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include <malloc.h>
#include <stdbool.h>

// Кодировка для консоли, чтобы корректно выводились кириллические символы без дополнительных манипуляций
#define RUS_ENCODING 1251

typedef struct _book {
	char* title; // Название книги
	char* author; // Автор или авторы
	int issueYear; // Год выпуска int, потому что существуют книги, написанные до нашей эры
	char* publishingHouse; // Издательство
	float readersScore; // Оценка читателей по шкале от 1 до 10, может быть дробной, к примеру 5,5
	unsigned price; // Текущая цена книги
	char* genre; // Жанр повествования
	char* shortDescription; // Краткое описание
	unsigned id; // Уникальный идентификатор книги (для поиска клонов и удаления)
} Book;

typedef struct _stackNode {
	Book* currentBook;
	struct _stackNode* nextNode;
} bookStackNode;

// Дерево, где будут храниться все книги
typedef struct _AVL_Tree_Node {
	/* Вершина стека, в котором хранятся книги. В каждом элементе дерева может храниться больше одной книги для обработки
	* коллизий (совпадений) - например, все книги одного автора или одного года выпуска будут храниться в одном элементе
	соответствующего дерева */
	bookStackNode* stackTopPtr;
	struct _AVL_Tree_Node* left; // Левое поддерево
	struct _AVL_Tree_Node* right; // Правое поддерево
	unsigned char height; // Высота (по умолчанию 1)
} treeNode;

/* Поскольку для быстрого поиска и вставки хранение информации о каждой книге осуществляется в разных деревьях, пользователю нужно будет
* выбрать, по какому из параметров он ищет книгу (название, автор, год выпуска и так далее) */
enum searchTypes {SEARCH_BY_TITLE, SEARCH_BY_AUTHOR, SEARCH_BY_YEAR, SEARCH_BY_PRICE, SEARCH_BY_GENRE, SEARCH_BY_SCORE, SEARCH_BY_PUBLISHING_HOUSE};

// Выводит справку для пользователя (описание функций программы и как юзер может ими воспользоваться)
void usage();

#endif // !LIBRARY_H
