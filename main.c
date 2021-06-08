#include "main.h"

// ������� ���� ����� ��� ����������, ���������� ����������, ����� ��� ������� ����� � ��� ������ � �� ���� ��������
unsigned int id = 0;

void main() {
	SetConsoleCP(RUS_ENCODING);
	SetConsoleOutputCP(RUS_ENCODING);

	/* ������������ ��������� �������� � ����������: ���������� �����, ��������, ��������������, ����� � �����, ����������
	� ���� ������ � �������� �� ���, ����� ������� � ����� �� ���������. � ������ ������ ����� � �������� "�������� �������",
	����� ������������ ��� ������� ��� ������� ���� ��� � ��������. */
	enum Actions { ADD = 1, REMOVE, EDIT, SEARCH, SAVE, DOWNLOAD, PRINT_REFERENCE, EXIT } action = PRINT_REFERENCE;
	
	while (action != EXIT) {
		switch (action) {
			case PRINT_REFERENCE:
				usage();
				break;
			case ADD: {
				Book* newBookPtr = createBook(); 
				insertInAllTrees(newBookPtr);
			}
			case EXIT:
				exit(ERROR_SUCCESS);
			default:
				puts("�� ������� �������������� ��������.\n");
				usage();
				break;
		}

		printf("����� �������� ��������� ���������? > ");
		scanf("%u", &action);
		getchar();
		putchar('\n');
	}
	
}


void usage(){
	printf("\t----------------------��������, ��� ����� �������:-----------------------\n"
		"\t| 1. �������� ����� � ������-����������                                |\n"
		"\t| 2. ������� ����� �� ����������                                       |\n"
		"\t| 3. �������� ���������� � ������� ����� (����� ����)                  |\n"
		"\t| 4. ����� ����� �� ������ ���� (������, ����� � ��� �����)            |\n"
		"\t| 5. ��������� ��� ����� �� ������� ���������� � ���� ������           |\n"
		"\t| 6. ��������� ��� ����� �� ���� ������ (�����) � ����������           |\n"
		"\t| 7. ����� �������� ��� �������                                        |\n"
		"\t| 8. ����� �� ���������                                                |\n"
		"\t-------------------------------------------------------------------------\n\n");
}

Book* createBook() {
	puts("���, �� �������� �������� � ����������� � ����� �����. ������� �������� ���� ������� �����: \n");

	Book* newBook = (Book*) calloc(1, sizeof(Book));
	newBook->title = getUserStringPtr("1. ������� �������� ����� > ");
	newBook->author = getUserStringPtr("2. ������� ������ ����������� ����� > ");
	newBook->publishingHouse = getUserStringPtr("3. �������, ����� ������������ ��������� ����� >");
	newBook->genre = getUserStringPtr("4. ������� ���� ����� > ");
	newBook->shortDescription = getUserStringPtr("5. �������� ������� �������� ������ � ����������� ����� > ");

	printf("6. ������� ���� ����� � ������ (�����, ��� �������������� �����������) > ");
	scanf("%u", &(newBook->price));
	getchar();
	putchar('\n');
	
	printf("7. ������� ���������� � ���� ��������� ����� (����� �����, ���� �� ����� ��� - ������� �� ������ �����) > ");
	scanf("%d", &(newBook->issueYear));
	getchar();
	putchar('\n');

	printf("8. ������� ������ ����� ���������� (�� ����� �� 0 �� 10, ������� ������ ���������) > ");
	scanf("%f", &(newBook->readersScore));
	getchar();
	putchar('\n');

	// ������ ���������� id � ����������� ���������� ����������, ����� � ��������� ����� ��� ������ id
	newBook->id = id++;

	return newBook;
}

char* getUserStringPtr(char* inputString) {
	char* temp = (char*)malloc(MAX_FIELD_LENGTH * sizeof(char));
	printf(inputString);
	gets_s(temp, MAX_FIELD_LENGTH);
	// ������� ������� ������ ����� ����������
	putchar('\n');

	// �������� ����� ��������� ������������� ������ � ������� ��� ��� ������ �������� � ��� ����� �������� (��� �������� ������)
	size_t sLen = strlen(temp);
	char* resultString = (char*)malloc((sLen + 1) * sizeof(char));
	strcpy(resultString, temp);
	// �� �������� �������� ������, ���������� ��� �������� ������
	free(temp);

	return resultString;
}
