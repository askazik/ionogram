// ===========================================================================
// Измерение и запись ионограммы в файл.
// ===========================================================================
#include "ionogram.h"

void SetPriorityClass(void);

int main(void){	
	setlocale(LC_ALL,"Russian"); // настройка локали на вывод сообщений по-русски
	SetPriorityClass(); // назначаем приоритет процесса

	// ===========================================================================================
    // 1. Читаем файл конфигурации.
    // ===========================================================================================
    parusConfig conf(IONOGRAM_CONFIG_DEFAULT_FILE_NAME); // используем имя файла конфигурации по умолчанию

    std::cout << "Используем конфигурационный файл: <" << conf.getFileName() << ">." << std::endl;
	for(int i=0; i < conf.getModulesCount(); i++)
		std::cout << "Частоты зондирования (кГц): " << conf.getFreq(i) << " - " << conf.getFreqEnd(i) << std::endl;
	std::cout << "Измерения начаты." << std::endl;
	std::cout << "Если не слышно жужжания - произошёл сбой. Можно закрывать это окно." << std::endl;

    // ===========================================================================================
    // 2. Конфигурирование и запуск сеанса.
    // ===========================================================================================
	int RetStatus = 0;
	try	{
		// Подготовка аппаратуры к зондированию.
		// Открытие выходнго файла данных и запись заголовка.
		parusWork *work = new parusWork(conf);

		DWORD msTimeout = 4;
		myModule* curModule = conf.getModule(0);
		// Определение параметров зондирования	
		unsigned f1 = curModule->frq;
		unsigned f2 = curModule->fend;
		unsigned df = curModule->fstep;
		unsigned ks = (f2-f1)/df + 1;	// Число строк ионограммы

		// Смена частоты синтезатора
		unsigned f = f1;
		work->adjustSounding(f); // настроим синтезатор
		work->ASYNC_TRANSFER(); // запустим АЦП
		for (unsigned j = 0; j < ks; j++) // счётчик циклов перебора по частоте
		{ 				
			// Цикл проверки до появления результатов в буфере.
			// READ_BUFISCOMPLETE - сбоит на частоте 47 Гц
			while(work->READ_ISCOMPLETE(msTimeout) == NULL);

			// Остановим АЦП
			work->READ_ABORTIO();

			// Запишем данные в файл.
			switch(conf.getVersion()){
			case 0: // выборка только двух отражений из измеренных данных
				work->fillUnits(); // копирование данных из аппаратного буфера в локальные хранилища для последующей обработки
				work->savePackedLine();
				break;
			case 1: 
				//
				break;
			}

			// Смена частоты синтезатора
			if(j < ks){
				f += df;
				work->adjustSounding(f); // настроим синтезатор
				work->ASYNC_TRANSFER(); // запустим АЦП
			}
		} // цикл перебора по частотам
		delete work;
	}
	catch(std::exception &e)
	{
		std::cerr << std::endl;
		std::cerr << "Сообщение: " << e.what() << std::endl;
		std::cerr << "Тип      : " << typeid(e).name() << std::endl;
		RetStatus = -1;
	}
 	Beep( 1500, 300 );

	return RetStatus;
}

void SetPriorityClass(void){
	HANDLE procHandle = GetCurrentProcess();
	DWORD priorityClass = GetPriorityClass(procHandle);

	if (!SetPriorityClass(procHandle, HIGH_PRIORITY_CLASS))
		std::cerr << "SetPriorityClass" << std::endl;

	priorityClass = GetPriorityClass(procHandle);
	std::cerr << "Priority Class is set to : ";
	switch(priorityClass)
	{
	case HIGH_PRIORITY_CLASS:
		std::cerr << "HIGH_PRIORITY_CLASS\r\n";
		break;
	case IDLE_PRIORITY_CLASS:
		std::cerr << "IDLE_PRIORITY_CLASS\r\n";
		break;
	case NORMAL_PRIORITY_CLASS:
		std::cerr << "NORMAL_PRIORITY_CLASS\r\n";
		break;
	case REALTIME_PRIORITY_CLASS:
		std::cerr << "REALTIME_PRIORITY_CLASS\r\n";
		break;
	default:
		std::cerr <<"Unknown priority class\r\n";
	}
}