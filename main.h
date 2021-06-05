#pragma once
#ifndef LIBRARY_H
#define LIBRARY_H

#include <stdio.h>
#include <string.h>
#include <Windows.h>

// ��������� ��� �������, ����� ��������� ���������� ������������� ������� ��� �������������� �����������
#define RUS_ENCODING 1251

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

typedef struct _AVL_Tree_Node {
	Book* currentBook; // ����������� � ������� �������� �����
	struct _AVL_Tree_Node* left; // ����� ���������
	struct _AVL_Tree_Node* right; // ������ ���������
	unsigned char height; // ������ (�� ��������� 1)
} treeNode;

// ������� ������� ��� ������������ (�������� ������� ��������� � ��� ���� ����� ��� ���������������)
void usage();

#endif // !LIBRARY_H
