#pragma once
#ifndef LIBRARY_H
#define LIBRARY_H

#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include <malloc.h>
#include <stdbool.h>

// ��������� ��� �������, ����� ��������� ���������� ������������� ������� ��� �������������� �����������
#define RUS_ENCODING 1251
// ���������� ����� � ���������� � �����, �� ������� ����� ���� �����
#define SEARCH_FIELDS_NUMBER 7
// ������������ ����� ������ � �������� � ����� ���� ��������� Book
#define MAX_FIELD_LENGTH 2000

typedef struct _book {
	char* title; // �������� �����
	char* author; // ����� ��� ������
	int issueYear; // ��� ������� int, ������ ��� ���������� �����, ���������� �� ����� ���
	char* publishingHouse; // ������������
	float readersScore; // ������ ��������� �� ����� �� 1 �� 10, ����� ���� �������, � ������� 5,5
	unsigned price; // ������� ���� �����
	char* genre; // ���� �������������
	char* shortDescription; // ������� ��������
	unsigned id; // ���������� ������������� ����� (��� ������ ������ � ��������)
} Book;

typedef struct _stackNode {
	Book* currentBook;
	struct _stackNode* nextNode;
} bookStackNode;

// ������, ��� ����� ��������� ��� �����
typedef struct _AVL_Tree_Node {
	/* ������� �����, � ������� �������� �����. � ������ �������� ������ ����� ��������� ������ ����� ����� ��� ���������
	�������� (����������) - ��������, ��� ����� ������ ������ ��� ������ ���� ������� ����� ��������� � ����� ��������
	���������������� ������ */
	bookStackNode* stackTopPtr;
	struct _AVL_Tree_Node* left; // ����� ���������
	struct _AVL_Tree_Node* right; // ������ ���������
	unsigned char height; // ������ (�� ��������� 1)
} treeNode;

/* ��������� ��� �������� ������ � ������� �������� ���������� � ������ ����� �������������� � ������ ��������, ������������ ����� ����� 
�������, �� ������ �� ���������� �� ���� ����� (��������, �����, ��� ������� � ��� �����) */
enum searchTypes {SEARCH_BY_TITLE, SEARCH_BY_AUTHOR, SEARCH_BY_YEAR, SEARCH_BY_PRICE, SEARCH_BY_GENRE, SEARCH_BY_SCORE, SEARCH_BY_PUBLISHING_HOUSE};
// ���������� ��������� ���� ��������� � ������������� ����
enum compareResults {SECOND_IS_SUPERIOR = -1, IS_EQUAL, FIRST_IS_SUPERIOR};

// ������� ������� ��� ������������ (�������� ������� ��������� � ��� ���� ����� ��� ���������������)
void usage();

/* ������� �����, �� ������� ��������� bookPtr, � ���-������ � �������� p. 
��������� ��� ������� (������/������) �������������� �� ���������� ���� compareType, ��������� �������� ��������
������� � ������������ searchTypes (�� ����, ����� ���������� �� ��������, ������, ������ � ��� �����) */
treeNode* insert(treeNode* p, Book* bookPtr, enum searchTypes compareType);

// �������� �����, �� ������� ��������� bookPtr, �� ���������� ���-������ � �������� p
treeNode* delete(treeNode* p, Book* bookPtr, enum searchTypes compareType);

/* ���� � ������ ����� (��� ������ ����) �� ������������� ���������.
�� ������ ���������� ���������� ������������� ����� � ����� ����������� �����, �� �������� � ���� �����.
���������� ��������� �� ������� ����� (������) ��������� ����. ��������, ���� ����� ������������� �� ������, ������
��������� �� ����, � ������� ����� ������ ��� ����� �������� ������. */
bookStackNode* search(treeNode* p, Book* bookPtr, enum searchTypes searchType);

/*�������� � ���������� ����� �� ��� �������. ��� ����������� �������� ������ �������� ����� ���� ���� - �� ���������� ����� ������ 
(searchTypes), ������ ������������� �� ������ ����*/
void insertInAllTrees(Book* bookPtr);
void deleteFromAllTrees(Book* bookPtr);

/* �������, � ������� ������� ������������ ����� ������� ����� �����, �������� ��� ��������� ����. 
���������� ��������� �� ������������� �����. �������������, ��� ��� ��������� ���� ����� ������� ����������� (malloc/calloc) */
Book* createBook(void);

/* ������� ����������� � ������������ ���� ������ (������ ���������� � inputString), ��������� ��������� ������������� ������,
* �������� ��� ��� ������ ������������ �������, ��������� ���� ������ � ���������� ��������� �� ���������� ������ */
char* getUserStringPtr(char* inputString);

#endif // !LIBRARY_H
