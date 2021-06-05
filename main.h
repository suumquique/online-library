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
	size_t price; // ������� ���� �����
	char* genre; // ���� �������������
	char* shortDescription; // ������� ��������
} Book;

// ������� ������� ��� ������������ (�������� ������� ��������� � ��� ���� ����� ��� ���������������)
void usage();

#endif // !LIBRARY_H
