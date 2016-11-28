// -*-coding: cp1251;-*-
// ������������ ���� ��� ������ � ����������������� �������.
#ifndef __CONFIG_H__
#define __CONFIG_H__

#define CONFIG_DEFAULT_FILE_NAME "ionogram.conf"

// ��������� ������������ ������. ������ 0.
struct moduleIonogram {  
    unsigned fbeg;   // ��������� ������� ������, ���
    unsigned fend;   // �������� ������� ������, ���	
    unsigned fstep;  // ��� �� ������� ����������, ���
    unsigned pulse_count; // ��������� ������������ �� ������ �������
    unsigned attenuation; // ���������� (����������) 1/0 = ���/����
    unsigned gain;	// �������� (g = value/6, 6�� = ���������� � 4 ���� �� ��������)
    unsigned pulse_frq; // ������� ����������� ���������, ��
    unsigned pulse_duration; // ������������ ����������� ���������, ���
};

// ����� ������� � ����������������� ����� ������������.
class confIonogram {
    std::string whitespaces;
    std::string fullFileName;
    
    unsigned ver; // ����� ������
    unsigned height_step; // ��� �� ������
    unsigned height_count; // ���������� �����
    unsigned switch_frequency; // ������� ������������ ������ ���������
    unsigned modules_count; // ���������� ������� ������������
    moduleIonogram *ptModulesArray; // ��������� �� ������ ������� ����������

    void init(void);
	bool isValidConf(std::string fullName);
	unsigned getValueFromString(std::string line);
    void getModulesCount(std::ifstream &fin);
	void getModules(std::ifstream &fin);
    moduleIonogram getCurrentModule(std::ifstream &fin);
public:
    confIonogram(void);
	confIonogram(std::string fullName);
	~confIonogram(void);

    void loadConf(std::string fullName);
    std::string getFileName(void){return fullFileName;}
	unsigned getModulesCount(void){return modules_count;}
	unsigned getHeightStep(void){return height_step;}
    void setHeightStep(double value){height_step = static_cast<unsigned>(value);}
	unsigned getHeightCount(void){return height_count;}
	unsigned getVersion(void){return ver;}
	unsigned getSwitchFrequency(void){return switch_frequency;}
    moduleIonogram getModule(unsigned num);

    unsigned getFreq_min(void){return ptModulesArray[0].fbeg;}
    unsigned getFreq_max(void){return ptModulesArray[modules_count-1].fend;}
    unsigned getCount_freq(void);
};

#endif // __CONFIG_H__
