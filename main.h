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
// Количество полей в информации о книге, по которым может идти поиск
#define SEARCH_FIELDS_NUMBER 7
// Максимальная длина строки в символах в любом поле структуры Book
#define MAX_FIELD_LENGTH 2000

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
	коллизий (совпадений) - например, все книги одного автора или одного года выпуска будут храниться в одном элементе
	соответствующего дерева */
	bookStackNode* stackTopPtr;
	struct _AVL_Tree_Node* left; // Левое поддерево
	struct _AVL_Tree_Node* right; // Правое поддерево
	unsigned char height; // Высота (по умолчанию 1)
} treeNode;

/* Поскольку для быстрого поиска и вставки хранение информации о каждой книге осуществляется в разных деревьях, пользователю нужно будет 
выбрать, по какому из параметров он ищет книгу (название, автор, год выпуска и так далее) */
enum searchTypes {SEARCH_BY_TITLE, SEARCH_BY_AUTHOR, SEARCH_BY_YEAR, SEARCH_BY_PRICE, SEARCH_BY_GENRE, SEARCH_BY_SCORE, SEARCH_BY_PUBLISHING_HOUSE};
// Результаты сравнения книг функциями в удобочитаемом виде
enum compareResults {SECOND_IS_SUPERIOR = -1, IS_EQUAL, FIRST_IS_SUPERIOR};

// Выводит справку для пользователя (описание функций программы и как юзер может ими воспользоваться)
void usage();

/* Вставка книги, на которую указывает bookPtr, в АВЛ-дерево с вершиной p. 
Сравнения для вставки (больше/меньше) осуществляются по выбранному типу compareType, возможные значения которого
описаны в перечислении searchTypes (то есть, можно сравнивать по названию, автору, прайсу и так далее) */
treeNode* insert(treeNode* p, Book* bookPtr, enum searchTypes compareType);

// Удаление книги, на которую указывает bookPtr, из указанного АВЛ-дерева с вершиной p
treeNode* delete(treeNode* p, Book* bookPtr, enum searchTypes compareType);

/* Ищет в дереве книгу (или список книг) по определенному параметру.
Во второй переменной передается новосозданная книга с одним заполненным полем, по которому и идет поиск.
Возвращает указатель на вершину стека (списка) найденных книг. Например, если поиск осуществлялся по автору, вернет
указатель на стек, в котором будут лежать все книги искомого автора. */
bookStackNode* search(treeNode* p, Book* bookPtr, enum searchTypes searchType);

/*Удаление и добавление книги во все деревья. Для оптимизации скорости поиска деревьев всего семь штук - по количеству типов поиска 
(searchTypes), каждое отсортировано по своему типу*/
void insertInAllTrees(Book* bookPtr);
void deleteFromAllTrees(Book* bookPtr);

/* Функция, с помощью которой пользователь может создать новую книгу, заполнив все требуемые поля. 
Возвращает указатель на новосозданную книгу. Гарантируется, что все строковые поля будут созданы динамически (malloc/calloc) */
Book* createBook(void);

/* Функция запрашивает у пользователя ввод строки (вопрос передается в inputString), считывает введенную пользователем строку,
* выделяет под нее память оптимального размера, сохраняет туда строку и возвращает указатель на выделенную память */
char* getUserStringPtr(char* inputString);

#endif // !LIBRARY_H
