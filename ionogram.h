// Статическое присоединение daqdrv.dll посредством библиотеки импортирования.
// Build me with "g++ ionogram.cpp -g -Wall -o ionogram -L. -ldaqdrv"
// ===========================================================================
// Запись ионограммы в файл.
// ===========================================================================
#ifndef __IONOGRAM_H__
#define __IONOGRAM_H__

#include <windows.h>
#include <stdexcept>
#include <clocale>
#include <iostream>
#include <fstream>
#include <climits>

// Включение в проект библиотеки импорта daqdrv.dll
#include "daqdef.h"
#pragma comment(lib, "daqdrv.lib") // for Microsoft Visual C++
#include "m14x3mDf.h"

// Внутренние заголовочные файлы
#include <config.h>
#include <parus.h>

#define IONOGRAM_CONFIG_DEFAULT_FILE_NAME "ionogram.conf"

#endif // __IONOGRAM_H__