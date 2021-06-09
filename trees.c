#include "main.h"

/*Деревья, отсортированные по различным полям информации о книге (автор, название, прайс и так далее). Всего параметров
для сортировки 7 (По краткому содержанию и id сортировки нет), следовательно, всего семь деревьев.
При добавлении или удалении книги она добавляется или удаляется соответственно изо всех семи деревьев, но в
результате столь неэкономного использования памяти получается крайне оптимальный поиск.*/
treeNode* trees[7] = { 0 };

void insertInAllTrees(Book* bookPtr) {
	for (size_t searchType = 0; searchType < SEARCH_FIELDS_NUMBER; searchType++) {
		trees[searchType] = insert(trees[searchType], bookPtr, searchType);
	}
}

void deleteFromAllTrees(Book* bookPtr) {
	for (size_t searchType = 0; searchType < SEARCH_FIELDS_NUMBER; searchType++) {
		trees[searchType] = delete(trees[searchType], bookPtr, searchType);
	}

	free(bookPtr->author);
	free(bookPtr->title);
	free(bookPtr->genre);
	free(bookPtr->publishingHouse);
	free(bookPtr->shortDescription);
	free(bookPtr);
}

// Сравнивает числа, если они равны, возвращает 0, если первое больше второго - единицу, если наоборот - "-1"
int compareNumbers(double first, double second) {
	return first == second ? 0 : first > second ? 1 : -1;
}

/*Сравнивает книги по определенному параметру (названию, автору, цене и т. п.). 
* Если первая книга "больше", возвращает единицу;
* Если книги "равны" (например, один и тот же автор или одинаковая цена), возвращает ноль;
* Если вторая книга "больше", возвращает значение -1. */
enum compareResults compareBooks(Book* firstBook, Book* secondBook, enum searchTypes compareType) {
	switch (compareType) {
		case SEARCH_BY_TITLE:
			return strcmp(firstBook->title, secondBook->title);
		case SEARCH_BY_AUTHOR:
			return strcmp(firstBook->author, secondBook->author);
		case SEARCH_BY_GENRE:
			return strcmp(firstBook->genre, secondBook->genre);
		case SEARCH_BY_PUBLISHING_HOUSE:
			return strcmp(firstBook->publishingHouse, secondBook->publishingHouse);
		case SEARCH_BY_PRICE:
			return compareNumbers(firstBook->price, secondBook->price);
		case SEARCH_BY_YEAR:
			return compareNumbers(firstBook->issueYear, secondBook->issueYear);
		case SEARCH_BY_SCORE:
			return compareNumbers(firstBook->readersScore, secondBook->readersScore);
		default:
			return 0;
	}
}

unsigned char getHeight(treeNode* p)
{
	return p ? p->height : 0;
}

int getBalanceFactor(treeNode* p)
{
	return getHeight(p->right) - getHeight(p->left);
}

void fixHeight(treeNode* p)
{
	unsigned char hl = getHeight(p->left);
	unsigned char hr = getHeight(p->right);
	p->height = (hl > hr ? hl : hr) + 1;
}

treeNode* rotateRight(treeNode* p) // правый поворот вокруг p
{
	treeNode* q = p->left;
	p->left = q->right;
	q->right = p;
	fixHeight(p);
	fixHeight(q);
	return q;
}

treeNode* rotateLeft(treeNode* q) // левый поворот вокруг q
{
	treeNode* p = q->right;
	q->right = p->left;
	p->left = q;
	fixHeight(q);
	fixHeight(p);
	return p;
}

treeNode* balance(treeNode* p) // балансировка узла p
{
	fixHeight(p);
	if (getBalanceFactor(p) == 2)
	{
		if (getBalanceFactor(p->right) < 0)
			p->right = rotateRight(p->right);
		return rotateLeft(p);
	}
	if (getBalanceFactor(p) == -2)
	{
		if (getBalanceFactor(p->left) > 0)
			p->left = rotateLeft(p->left);
		return rotateRight(p);
	}
	return p; // балансировка не нужна
}

treeNode* insert(treeNode* p, Book* bookPtr, enum searchTypes compareType) {
	if (!p) {
		// Создаем новый стек для книг, туда добавляем одну книгу, указатель на следующий элемент стека пустой
		bookStackNode* newStackTop = (bookStackNode*)calloc(1, sizeof(bookStackNode));
		if (newStackTop == NULL) {
			puts("\nНевозможно добавить книгу в стек новой ноды: недостаточно памяти\n");
			return NULL;
		}

		newStackTop->currentBook = bookPtr;
		newStackTop->nextNode = NULL;

		// Создаем вершину (ноду) дерева, туда добавляем указатель на созданный нами ранее стек. Устанавливаем высоту данной вершины по умолчанию
		treeNode* newNode = (treeNode*)calloc(1, sizeof(treeNode));
		if (newNode == NULL) {
			puts("\nОшибка при добавлении ноды в дерево: недостаточно памяти для выделения\n");
			return NULL;
		}

		newNode->height = 1;
		newNode->stackTopPtr = newStackTop;
		newNode->left = newNode->right = NULL;
		return newNode;
	}

	/* Сравниваем книгу из текущей вершины дерева и добавляемую нами книгу по опредленному параметру (допустим, по цене или по автору).
	* Параметр сравнения - compareType.*/
	switch (compareBooks(p->stackTopPtr->currentBook, bookPtr, compareType)) {
		// Если, допустим, мы сравниваем по авторам и они совпали, то добавляем книгу в стек текущей ноды дерева
		case IS_EQUAL:
		{
			bookStackNode* newStackTop = (bookStackNode*)calloc(1, sizeof(bookStackNode));
			if (newStackTop == NULL) {
				puts("\nНевозможно добавить книгу в стек: недостаточно памяти\n");
				break;
			}

			newStackTop->currentBook = bookPtr;
			newStackTop->nextNode = p->stackTopPtr;
			p->stackTopPtr = newStackTop;
		}
		break;
		/* Если книга, которую мы добавляем, по сравниваемой характеристике "меньше" книги в текущей вершине,
		* то пытаемся вставить ее в левое поддерево текущей вершины */
		case FIRST_IS_SUPERIOR:
			p->left = insert(p->left, bookPtr, compareType);
			break;
		// Если же книга, напротив, по заданному параметру больше книги из текущей вершины, то переходим в правое поддерево для вставки туда
		case SECOND_IS_SUPERIOR:
			p->right = insert(p->right, bookPtr, compareType);
			break;
	}

	// Проверяем, не нарушен ли баланс АВЛ-дерева, если нарушен - балансируем
	return balance(p);
}

treeNode* findMin(treeNode* p) // поиск узла с минимальным ключом в дереве p 
{
	return p->left ? findMin(p->left) : p;
}

treeNode* removeMin(treeNode* p) // удаление узла с минимальным ключом из дерева p
{
	if (p->left == 0)
		return p->right;
	p->left = removeMin(p->left);
	return balance(p);
}

treeNode* delete(treeNode* p, Book* bookPtr, enum searchTypes compareType) {
	if (!p) return NULL;

	switch (compareBooks(p->stackTopPtr->currentBook, bookPtr, compareType)) {
		case IS_EQUAL: {
			/* Если в стеке книг лежит не один элемент (то есть, например, есть несколько книг этого автора),
			* мы ищем нужную по id и просто удаляем ее из стека*/
			if (p->stackTopPtr->nextNode != NULL) {
				bookStackNode* currentStackNode = p->stackTopPtr;
				bookStackNode* previousStackNode = p->stackTopPtr;
				// Идем по стеку вниз и ищем нужную книгу по id. После завершения цикла в currentNode будет исходная книга, либо нода будет пустой
				while (currentStackNode->currentBook->id != bookPtr->id && currentStackNode != NULL) {
					previousStackNode = currentStackNode;
					currentStackNode = previousStackNode->nextNode;
				}

				// Если мы пытаемся удалить книгу, которой нет, следовательно, ничего делать не надо
				if (currentStackNode == NULL) return p;

				/* Убираем данный элемент из стека, "пропуская" его: в ссылку на следующий элемент из предыдущего элемента ставим
				* ссылку на элемент, который следует за текущим. Если надо удалить вершину стека, прямо заменяем ее*/
				if (currentStackNode == p->stackTopPtr) p->stackTopPtr = currentStackNode->nextNode;
				else previousStackNode->nextNode = currentStackNode->nextNode;

				// Очищаем текущую ноду
				free(currentStackNode);
			}
			// Если в стеке один элемент (одна книга), то надо удалять вершину дерева полностью вместе со стеком
			else {
				free(p->stackTopPtr);

				/* Запоминаем корни q и r левого и правого поддеревьев узла p; удаляем узел p; если правое поддерево пустое,
				то возвращаем указатель на левое поддерево; если правое поддерево не пустое, то находим там минимальный элемент min,
				потом его извлекаем оттуда, слева к min подвешиваем q, справа — то, что получилось из r, возвращаем min после его балансировки.*/
				treeNode* l = p->left;
				treeNode* r = p->right;
				free(p);

				if (!r) return l;
				treeNode* min = findMin(r);
				min->right = removeMin(r);
				min->left = l;
				return balance(min);
			}
			break;
		}
		case FIRST_IS_SUPERIOR:
			p->left = delete(p->left, bookPtr, compareType);
			break;
		case SECOND_IS_SUPERIOR:
			p->right = delete(p->right, bookPtr, compareType);
			break;
	}
	
	return balance(p);
}

bookStackNode* search(treeNode* p, Book* bookPtr, enum searchTypes searchType) {
	if (!p) return NULL;

	// Сравнениваем текущую книгу из данной вершины дерева с значением параметра, по которому мы ищем
	switch (compareBooks(p->stackTopPtr->currentBook, bookPtr, searchType)) {
		case IS_EQUAL:
			return p->stackTopPtr;
		case FIRST_IS_SUPERIOR:
			return search(p->left, bookPtr, searchType);
		case SECOND_IS_SUPERIOR:
			return search(p->right, bookPtr, searchType);
	}
}