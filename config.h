// -*-coding: cp1251;-*-
// Заголовочный файл для работы с конфигурационными файлами.
#ifndef __CONFIG_H__
#define __CONFIG_H__

#define CONFIG_DEFAULT_FILE_NAME "ionogram.conf"

// Параметры зондирования модуля. Версия 0.
struct moduleIonogram {  
    unsigned fbeg;   // начальная частота модуля, кГц
    unsigned fend;   // конечная частота модуля, кГц	
    unsigned fstep;  // шаг по частоте ионограммы, кГц
    unsigned pulse_count; // импульсов зондирования на каждой частоте
    unsigned attenuation; // ослабление (аттенюатор) 1/0 = вкл/выкл
    unsigned gain;	// усиление (g = value/6, 6дБ = приращение в 4 раза по мощности)
    unsigned pulse_frq; // частота зондирующих импульсов, Гц
    unsigned pulse_duration; // длительность зондирующих импульсов, мкс
};

// Класс доступа к конфигурационному файлу зондирования.
class confIonogram {
    std::string whitespaces;
    std::string fullFileName;
    
    unsigned ver; // номер версии
    unsigned height_step; // шаг по высоте
    unsigned height_count; // количество высот
    unsigned switch_frequency; // частота переключения антенн ионозонда
    unsigned modules_count; // количество модулей зондирования
    moduleIonogram *ptModulesArray; // указатель на массив модулей ионограммы

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
