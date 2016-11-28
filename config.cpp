
// -*-coding: cp1251;-*-
// ===========================================================================
// �������� � ���������������� ������ ������������.
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
	std::cout << "�������� ���������� ����������������� �����." << std::endl;
	if(modules_count) delete [] ptModulesArray;
}

void confIonogram::init(void)
{
    modules_count = 0;
    ptModulesArray = nullptr;
    whitespaces = std::string(" \t\f\v\n\r");
}

// �������� �� ���� ���������������� ������ ��� ����������.
bool confIonogram::isValidConf(std::string fullName)
{
	bool key = false;

	// ������� ���� ��� ������.
	std::ifstream fin(fullName.c_str());
    if(!fin)
        throw std::runtime_error("Error: �� ���� ������� ���������������� ���� " + fullName);

    std::string line;
	while(getline(fin, line))
		{
			// trim from end of string (right)
			line.erase(line.find_last_not_of(whitespaces) + 1);
			if(line[0] != '#' && line.size()) // ������� ������������ � ������ �����
				{
                    std::size_t pos = line.find("IONOGRAM");
					if (pos != std::string::npos && pos == 0) // ��� ������
						{
							key = true;
							break; // ����������� ���� ����� ���������� ������ �������� ������
						}
				}			
		}
    fin.close();
    
	return key;
}

// ��������� ���������������� ���� �� ������� ����.
void confIonogram::loadConf(std::string fullName)
{
	int i = 0; // ������� ��������
    bool key = false;
	
	if(isValidConf(std::string(CONFIG_DEFAULT_FILE_NAME)))
		{
			// ������� ���� ��� ������.
			std::ifstream fin(fullName.c_str());
			if(!fin)
                throw std::runtime_error("Error: �� ���� ������� ���������������� ���� " + fullName);
			
            std::string line;
            while(!key)
				{
                    getline(fin, line);
					// trim from end of string (right)
					line.erase(line.find_last_not_of(whitespaces) + 1);
					if(line[0] != '#' && line.size()) // ������� ������������ � ������ �����
						{
							i++;
							switch(i)
								{									
								case 1: // ���������� ��� ��������������
									break;
								case 2: // ������ �������
									ver = getValueFromString(line);
									break;
								case 3: // ��� �� ������, �
									height_step = getValueFromString(line);
									break;
								case 4: // ���������� �������� ������, �� ����� 4096
									height_count = getValueFromString(line);
									break;
								case 5: // ������� ������������ ������ ���������, ���
									switch_frequency = getValueFromString(line);
                                    key = true; // ����������� ������������
									break;
								}
						}                        
				}
            getModules(fin); // ������ � ������������ �������
            fin.close();
		}
	else
        throw std::runtime_error("Error: ��� �� ���������������� ���� ��� ���������." + fullName);
}

// ���������� ���������� ������� ������������. 
void confIonogram::getModulesCount(std::ifstream &fin)
{
    modules_count = 0;
    std::string line;
    while(getline(fin, line))
        {          
            // trim from end of string (right)
			line.erase(line.find_last_not_of(whitespaces) + 1);
			if(line.size() && line[0] != '#') // ������� ������������ � ������ �����
                {
                    std::size_t pos = line.find("%%%");
					if (pos != std::string::npos && pos == 0) // ��� ������
							modules_count++;                            
                }
        }
    fin.clear();
    fin.seekg(0, fin.beg);
}

// ��������� ������ ������������.    
void confIonogram::getModules(std::ifstream &fin)
{
    getModulesCount(fin);
    ptModulesArray = new moduleIonogram [modules_count];

    // ��������� ��������� � �������.
    int i = 0;
    std::string line;
    while(getline(fin, line))
        {
            // trim from end of string (right)
			line.erase(line.find_last_not_of(whitespaces) + 1);
			if(line.size() && line[0] != '#') // ������� ������������ � ������ �����
                {
                    std::size_t pos = line.find("%%%");
					if (pos != std::string::npos && pos == 0) // ��� ������
                        {
                            ptModulesArray[i] = getCurrentModule(fin);
                            i++;
                        }
                }
        }    
        
}

// ��������� ������� ������ ������������.    
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
			if(line.size() && line[0] != '#' ) // ������� ������������ � ������ �����
                {
                    i++;
					switch(i)
                        {									
							case 1: // ��������� ������� ������, ���
                                out.fbeg = getValueFromString(line);
                                break;
							case 2: // �������� ������� ������, ���
								out.fend = getValueFromString(line);
								break;
							case 3: // ��� �� ������� ����������, ���
								out.fstep = getValueFromString(line);
								break;
							case 4: // ��������� ������������ �� ������ �������
								out.pulse_count = getValueFromString(line);
								break;
							case 5: // ���������� (����������) 1/0 = ���/����
								out.attenuation = getValueFromString(line);
								break;
							case 6: // �������� (g = value/6, 6�� = ���������� � 4 ���� �� ��������)
								out.gain = getValueFromString(line);
								break;
							case 7: // ������� ����������� ���������, ��
								out.pulse_frq = getValueFromString(line);
								break;
							case 8: // ������������ ����������� ���������, ���
								out.pulse_duration = getValueFromString(line);
                                key = true;
								break;
                        }
				}					
        }    

    return out;
}

// ����������� ���������� ���������� ������ ������������ �� ���� �������.
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

// ��������� ������ �������� �� ������.
unsigned confIonogram::getValueFromString(std::string line)
{
	return std::stoi(line,nullptr);
}

// ����������� ���������� ������.
moduleIonogram confIonogram::getModule(unsigned num)
{
	moduleIonogram out;
	moduleIonogram *pt;
	pt = ptModulesArray+num;
	memcpy(&out, pt, sizeof(moduleIonogram));

	return out;
}
// ���� ������ confIonogram.
// int main(void)
// {
// 	confIonogram a;
	
// 	return 0;
// }
