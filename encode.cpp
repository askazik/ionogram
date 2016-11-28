// -*-coding: cp1251;-*-
// ����������� ������ ��������� � �������� �� ��������� ���.
#include <fstream>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <windows.h>
#include "encode.h"

// �������� ������ ���������� �� ��������� �������-���.
// data - ����� ������ ����������
// conf - ����������� ������������
// numModule - ����� ������ ������������
// curFrq - ������� ������������, ���
void saveLine(unsigned char *data, unsigned _Count, unsigned _HeightStep, moduleIonogram curModule, unsigned short curFrq, HANDLE hFile)
{
	// ������������ ������ �� ������� �������
	FrequencyData tmpFrequencyData;
	tmpFrequencyData.gain_control = static_cast<unsigned short>(curModule.gain);												// !< �������� ���������� �������� ����������� ��.
    tmpFrequencyData.pulse_time = static_cast<unsigned short>(floor(1000./curModule.pulse_frq));//!< ����� ������������ �� ����� �������, [��].
    tmpFrequencyData.pulse_length = static_cast<unsigned char>(curModule.pulse_duration);		//!< ������������ ������������ ��������, [��c].
    tmpFrequencyData.band = static_cast<unsigned char>(floor(1000000./curModule.pulse_duration)); //!< ������ �������, [���].
    tmpFrequencyData.type = 0;																	//!< ��� ��������� (0 - ������� �������, 1 - ���).

    // ������� ����� ��� ����������� ������. �������, ��� ����������� ������ �� ������ ��������.
	unsigned n = _Count;
	unsigned char *tmpArray = new unsigned char [n];
    unsigned char *tmpLine = new unsigned char [n];
	unsigned char *tmpAmplitude = new unsigned char [n];
    
    unsigned j;
    unsigned char *dataLine = data;
    unsigned char thereshold;
    SignalResponse tmpSignalResponse;
	unsigned char countOutliers;
	unsigned short countFrq = 0; // ���������� ���� � ����������� ��������� �������

	// ��������� ������� ������� ��������.
	thereshold = getThereshold(dataLine, n);            
	tmpFrequencyData.frequency = curFrq;
	tmpFrequencyData.threshold_o = thereshold;
	tmpFrequencyData.threshold_x = 0;
	tmpFrequencyData.count_o = 0; // ����� ����������
	tmpFrequencyData.count_x = 0; // ������� � ��� ���� �- � �- ���������� ����������.

	unsigned short countLine = 0; // ���������� ���� � ����������� ������
	countOutliers = 0; // ������� ���������� �������� � ������� ������
	j = 0;
	while(j < n) // ������� �������
	{
		if(dataLine[j] > thereshold) // ������ ������ - ������������ ���.
		{
			tmpSignalResponse.height_begin = static_cast<unsigned long>(floor(1.0 * j * _HeightStep));
			tmpSignalResponse.count_samples = 1;
			tmpAmplitude[tmpSignalResponse.count_samples-1] = dataLine[j];
			j++; // ������� � ���������� ��������
			while(dataLine[j] > thereshold && j < n)
			{
				tmpSignalResponse.count_samples++;
				tmpAmplitude[tmpSignalResponse.count_samples-1] = dataLine[j];
				j++; // ������� � ���������� ��������
			}
			countOutliers++; // ��������� ���������� �������� � ������

			// �������� ����������� �������
			// ��������� �������.
			unsigned short nn = sizeof(SignalResponse);
			memcpy(tmpLine+countLine, &tmpSignalResponse, nn);
			countLine += nn;
			// ������ �������.
			nn = tmpSignalResponse.count_samples*sizeof(unsigned char);
			memcpy(tmpLine+countLine, tmpAmplitude, nn);
			countLine += nn;
		}
		else
			j++; // ���� ��������� ������
	}

	// ������ ��� ������� ������� �������� � �����.
	// ��������� ������ ������� ������, ���� ���� ������� �����������.
	tmpFrequencyData.count_o = countOutliers;
	// ��������� �������.
	unsigned nFrequencyData = sizeof(FrequencyData);
	memcpy(tmpArray, &tmpFrequencyData, nFrequencyData);
	// ��������� ����� �������������� ������� ��������
	if(countLine)
		memcpy(tmpArray+nFrequencyData, tmpLine, countLine);

	// Writing data from buffers into file (unsigned long = unsigned int)
	BOOL	bErrorFlag = FALSE;
	DWORD	dwBytesWritten = 0;	
	bErrorFlag = WriteFile( 
		hFile,									// open file handle
		reinterpret_cast<char*>(tmpArray),		// start of data to write
		countLine + nFrequencyData,				// number of bytes to write
		&dwBytesWritten,						// number of bytes that were written
		NULL);									// no overlapped structure
	if (!bErrorFlag) 
		throw std::runtime_error("�� ���� ��������� ������ ���������� � ����.");
	
    delete [] tmpLine;
	delete [] tmpAmplitude;
	delete [] tmpArray;
} 

// ��������� �����
int comp(const void *i, const void *j)
{
    return *(unsigned char *)i - *(unsigned char *)j;
}

// ���������� ������� ����� �������� ����������� ������.
unsigned char getThereshold(unsigned char *arr, unsigned n)
{
    unsigned char *sortData = new unsigned char [n]; // �������� ������ ��� ����������
    unsigned char Q1, Q3, dQ;
    unsigned short maxLim = 0;

    // 1. ���������� ������ �� �����������.
    memcpy(sortData, arr, n);
    qsort(sortData, n, sizeof(unsigned char), comp);
    // 2. �������� (n - ������, ������� ������!!!)
    Q1 = min(sortData[n/4],sortData[n/4-1]);
    Q3 = max(sortData[3*n/4],sortData[3*n/4-1]);
    // 3. �������������� ��������
    dQ = Q3 - Q1;
    // 4. ������� ������� ��������.
    maxLim = Q3 + 3 * dQ;    

    delete [] sortData;

    return (maxLim>=255)? 255 : static_cast<unsigned char>(maxLim);
}
