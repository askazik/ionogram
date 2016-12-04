// ===========================================================================
// ��������� � ������ ���������� � ����.
// ===========================================================================
#include "ionogram.h"

void SetPriorityClass(void);

int main(void){	
	setlocale(LC_ALL,"Russian"); // ��������� ������ �� ����� ��������� ��-������
	SetPriorityClass(); // ��������� ��������� ��������

	// ===========================================================================================
    // 1. ������ ���� ������������.
    // ===========================================================================================
    parusConfig conf(IONOGRAM_CONFIG_DEFAULT_FILE_NAME); // ���������� ��� ����� ������������ �� ���������

    std::cout << "���������� ���������������� ����: <" << conf.getFileName() << ">." << std::endl;
	for(int i=0; i < conf.getModulesCount(); i++)
		std::cout << "������� ������������ (���): " << conf.getFreq(i) << " - " << conf.getFreqEnd(i) << std::endl;
	std::cout << "��������� ������." << std::endl;
	std::cout << "���� �� ������ �������� - ��������� ����. ����� ��������� ��� ����." << std::endl;

    // ===========================================================================================
    // 2. ���������������� � ������ ������.
    // ===========================================================================================
	int RetStatus = 0;
	try	{
		// ���������� ���������� � ������������.
		// �������� �������� ����� ������ � ������ ���������.
		parusWork *work = new parusWork(conf);

		DWORD msTimeout = 4;
		myModule* curModule = conf.getModule(0);
		// ����������� ���������� ������������	
		unsigned f1 = curModule->frq;
		unsigned f2 = curModule->fend;
		unsigned df = curModule->fstep;
		unsigned ks = (f2-f1)/df + 1;	// ����� ����� ����������

		// ����� ������� �����������
		unsigned f = f1;
		work->adjustSounding(f); // �������� ����������
		work->ASYNC_TRANSFER(); // �������� ���
		for (unsigned j = 0; j < ks; j++) // ������� ������ �������� �� �������
		{ 				
			// ���� �������� �� ��������� ����������� � ������.
			// READ_BUFISCOMPLETE - ����� �� ������� 47 ��
			while(work->READ_ISCOMPLETE(msTimeout) == NULL);

			// ��������� ���
			work->READ_ABORTIO();

			// ������� ������ � ����.
			switch(conf.getVersion()){
			case 0: // ������� ������ ���� ��������� �� ���������� ������
				work->fillUnits(); // ����������� ������ �� ����������� ������ � ��������� ��������� ��� ����������� ���������
				work->savePackedLine();
				break;
			case 1: 
				//
				break;
			}

			// ����� ������� �����������
			if(j < ks){
				f += df;
				work->adjustSounding(f); // �������� ����������
				work->ASYNC_TRANSFER(); // �������� ���
			}
		} // ���� �������� �� ��������
		delete work;
	}
	catch(std::exception &e)
	{
		std::cerr << std::endl;
		std::cerr << "���������: " << e.what() << std::endl;
		std::cerr << "���      : " << typeid(e).name() << std::endl;
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