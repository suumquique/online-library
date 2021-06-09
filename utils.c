#include "main.h"

char* getUserStringPtr(char* inputString) {
	char* temp = (char*)malloc(MAX_FIELD_LENGTH * sizeof(char));
	if (temp == NULL) {
		puts("������������ ������ � ���� ��� ��������� ��������� ������ ��� ����������������� �����");
		return NULL;
	}
	printf(inputString);
	gets_s(temp, MAX_FIELD_LENGTH);
	// ������� ������� ������ ����� ����������
	putchar('\n');

	// �������� ����� ��������� ������������� ������ � ������� ��� ��� ������ �������� � ��� ����� �������� (��� �������� ������)
	size_t sLen = strlen(temp);
	char* resultString = (char*)malloc((sLen + 1) * sizeof(char));
	if (resultString == NULL) {
		puts("������������ ��������� ������ � ���� ��� ��������� ������ ��� ����������������� �����");
		return NULL;
	}
	strcpy(resultString, temp);
	// �� �������� �������� ������, ���������� ��� �������� ������
	free(temp);

	return resultString;
}

double getUserNumber(char* inputString) {
	double number;
	printf(inputString);
	scanf("%lf", &number);
	getchar();
	putchar('\n');

	return number;
}

void printBook(Book* bookPtr) {
	if (bookPtr == NULL) return;

	printf("\n�������� �����: %s                                                                                 \n"
		"����� �����: %s                                                                                    \n"
		"���� ������������: %s                                                                              \n"
		"��� ���������: %d                                                                                  \n"
		"������������: %s                                                                                   \n"
		"������ ���������: %.1f                                                                               \n"
		"���� �����: %u                                                                                     \n"
		"������� �������� ������: %s                                                                        \n\n"
		"ID �����: %u                                                                                       \n\n"
		"----------------------------------------------------------------------------------------------------\n\n",
		bookPtr->title, bookPtr->author, bookPtr->genre, bookPtr->issueYear, bookPtr->publishingHouse, bookPtr->readersScore,
		bookPtr->price, bookPtr->shortDescription, bookPtr->id);
}

char intToChar(int digit) {
	return '0' + ((digit > 0 && digit < 10) ? digit : 0);
}