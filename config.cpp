
// -*-coding: cp1251;-*-
// ===========================================================================
// Операции с конфигурационным файлом зондирования.
// g++ config.cpp -g -Wall -std=c++11 -o config
// ===========================================================================
#include <stdexcept>
#include <string>
#include <iostream>
#include <fstream>
#include "config.h"

confIonogram::confIonogram(void)
{
	init();
	fullFileName = std::string(CONFIG_DEFAULT_FILE_NAME);
	loadConf(fullFileName);
}
    
confIonogram::confIonogram(std::string fullName)
{
	init();
	fullFileName = fullName;
	loadConf(fullFileName);
}
    
confIonogram::~confIonogram(void)
{
	std::cout << "Выполнен деструктор конфигурационного файла." << std::endl;
	if(modules_count) delete [] ptModulesArray;
}

void confIonogram::init(void)
{
    modules_count = 0;
    ptModulesArray = nullptr;
    whitespaces = std::string(" \t\f\v\n\r");
}

// Является ли файл конфигурационным файлом для ионограммы.
bool confIonogram::isValidConf(std::string fullName)
{
	bool key = false;

	// Откроем файл для чтения.
	std::ifstream fin(fullName.c_str());
    if(!fin)
        throw std::runtime_error("Error: Не могу открыть конфигурационный файл " + fullName);

    std::string line;
	while(getline(fin, line))
		{
			// trim from end of string (right)
			line.erase(line.find_last_not_of(whitespaces) + 1);
			if(line[0] != '#' && line.size()) // пропуск комментариев и пустых строк
				{
                    std::size_t pos = line.find("IONOGRAM");
					if (pos != std::string::npos && pos == 0) // тег найден
						{
							key = true;
							break; // заканчиваем цикл после нахождения первой значащей строки
						}
				}			
		}
    fin.close();
    
	return key;
}

// Загружаем конфигурационный файл по полному пути.
void confIonogram::loadConf(std::string fullName)
{
	int i = 0; // счетчик значений
    bool key = false;
	
	if(isValidConf(std::string(CONFIG_DEFAULT_FILE_NAME)))
		{
			// Откроем файл для чтения.
			std::ifstream fin(fullName.c_str());
			if(!fin)
                throw std::runtime_error("Error: Не могу открыть конфигурационный файл " + fullName);
			
            std::string line;
            while(!key)
				{
                    getline(fin, line);
					// trim from end of string (right)
					line.erase(line.find_last_not_of(whitespaces) + 1);
					if(line[0] != '#' && line.size()) // пропуск комментариев и пустых строк
						{
							i++;
							switch(i)
								{									
								case 1: // пропускаем тег принадлежности
									break;
								case 2: // версия формата
									ver = getValueFromString(line);
									break;
								case 3: // шаг по высоте, м
									height_step = getValueFromString(line);
									break;
								case 4: // количество отсчётов высоты, не более 4096
									height_count = getValueFromString(line);
									break;
								case 5: // частота переключения антенн ионозонда, кГц
									switch_frequency = getValueFromString(line);
                                    key = true; // заканчиваем сканирование
									break;
								}
						}                        
				}
            getModules(fin); // войдем в сканирование модулей
            fin.close();
		}
	else
        throw std::runtime_error("Error: Это не конфигурационный файл для ионограмм." + fullName);
}

// Определяет количество модулей зондирования. 
void confIonogram::getModulesCount(std::ifstream &fin)
{
    modules_count = 0;
    std::string line;
    while(getline(fin, line))
        {          
            // trim from end of string (right)
			line.erase(line.find_last_not_of(whitespaces) + 1);
			if(line.size() && line[0] != '#') // пропуск комментариев и пустых строк
                {
                    std::size_t pos = line.find("%%%");
					if (pos != std::string::npos && pos == 0) // тег найден
							modules_count++;                            
                }
        }
    fin.clear();
    fin.seekg(0, fin.beg);
}

// Извлекает модули зондирования.    
void confIonogram::getModules(std::ifstream &fin)
{
    getModulesCount(fin);
    ptModulesArray = new moduleIonogram [modules_count];

    // Считываем параметры в модулях.
    int i = 0;
    std::string line;
    while(getline(fin, line))
        {
            // trim from end of string (right)
			line.erase(line.find_last_not_of(whitespaces) + 1);
			if(line.size() && line[0] != '#') // пропуск комментариев и пустых строк
                {
                    std::size_t pos = line.find("%%%");
					if (pos != std::string::npos && pos == 0) // тег найден
                        {
                            ptModulesArray[i] = getCurrentModule(fin);
                            i++;
                        }
                }
        }    
        
}

// Извлекает текущий модуль зондирования.    
moduleIonogram confIonogram::getCurrentModule(std::ifstream &fin)
{
    bool key = false;
    moduleIonogram out;
    int i = 0;

    std::string line;
    while(!key)
        {
            getline(fin, line);
            // trim from end of string (right)
			line.erase(line.find_last_not_of(whitespaces) + 1);
			if(line.size() && line[0] != '#' ) // пропуск комментариев и пустых строк
                {
                    i++;
					switch(i)
                        {									
							case 1: // начальная частота модуля, кГц
                                out.fbeg = getValueFromString(line);
                                break;
							case 2: // конечная частота модуля, кГц
								out.fend = getValueFromString(line);
								break;
							case 3: // шаг по частоте ионограммы, кГц
								out.fstep = getValueFromString(line);
								break;
							case 4: // импульсов зондирования на каждой частоте
								out.pulse_count = getValueFromString(line);
								break;
							case 5: // ослабление (аттенюатор) 1/0 = вкл/выкл
								out.attenuation = getValueFromString(line);
								break;
							case 6: // усиление (g = value/6, 6дБ = приращение в 4 раза по мощности)
								out.gain = getValueFromString(line);
								break;
							case 7: // частота зондирующих импульсов, Гц
								out.pulse_frq = getValueFromString(line);
								break;
							case 8: // длительность зондирующих импульсов, мкс
								out.pulse_duration = getValueFromString(line);
                                key = true;
								break;
                        }
				}					
        }    

    return out;
}

// Определение суммарного количества частот зондирования по всем модулям.
unsigned confIonogram::getCount_freq(void)
{
    unsigned count = 0;
    for(unsigned i = 0; i < modules_count; i++)
        {
            unsigned fcount = static_cast<unsigned>((ptModulesArray[i].fend - ptModulesArray[i].fbeg)/ptModulesArray[i].fstep + 1);
            count += fcount;
        }

    return count;
}

// Получение целого значения из строки.
unsigned confIonogram::getValueFromString(std::string line)
{
	return std::stoi(line,nullptr);
}

// Возвращение указанного модуля.
moduleIonogram confIonogram::getModule(unsigned num)
{
	moduleIonogram out;
	moduleIonogram *pt;
	pt = ptModulesArray+num;
	memcpy(&out, pt, sizeof(moduleIonogram));

	return out;
}
// Тест класса confIonogram.
// int main(void)
// {
// 	confIonogram a;
	
// 	return 0;
// }
