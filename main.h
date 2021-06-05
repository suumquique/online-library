#pragma once
#ifndef LIBRARY_H
#define LIBRARY_H

#include <stdio.h>
#include <string.h>
#include <Windows.h>

// Кодировка для консоли, чтобы корректно выводились кириллические символы без дополнительных манипуляций
#define RUS_ENCODING 1251

// Выводит справку для пользователя (описание функций программы и как юзер может ими воспользоваться)
void usage();

#endif // !LIBRARY_H
