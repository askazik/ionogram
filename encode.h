// -*-coding: cp1251;-*-
// ������������ ���� ��� ������ ������������ ������ ������������.

#ifndef __ENCODE_H__
#define __ENCODE_H__

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// ������, ��� ��������� � ������ ������������� � �����������
// �� �����������, ����������� � ����������� �������. 
// ����������� ����� � ������������� ������� ����������� ��������
// ������. �������� HDF, CDF �.�.�.
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#pragma pack(push, 1)

struct adcChannel {
    unsigned short b0: 1;  // ��������������� � 1 � ������ ������� ����� ��� ������� ������ ��� (�.�. ����� ������ ��� ��� ����� ������� ������ � �����-������� ������).
    unsigned short b1: 1;  // ��������������� � 1 � �������, ������� ������������� ���������� ������������� ����� (�� ������� ������������ ������� ������ � ������ ��������������) ��� ������� ������ ���.    
    short value : 14; // ������ �� ������ ������ ���, 14-��������� �����
};

struct adcTwoChannels {
    adcChannel re; // ������� ������ ����������
    adcChannel im; // ������� ������ ����������
};

// ������ �� 2015 �.
struct ionHeaderOld { 	        // === ��������� ����� ��������� ===   
	unsigned char st;		// Station  <������ = 3>     > 1
	unsigned int f_beg;     // ORIGN FREQUENCY     	     > 2
	unsigned int f_step;    // STEP FREQUENCY            > 4
	unsigned int f_end;     // Final frequency           > 6
	double		 dh;		// Height step, m            > 8
	unsigned int scale;  	// Scale type & lines        >10
							// Format: <S_ppppppppppppp>
							// S=0 -> line  S=1 -> log
	unsigned char   sec;   	// Seconds                   >12
	unsigned char  	min;   	// Minutes                   >13
	unsigned char  	hour; 	// Hours                     >14
	unsigned char  	day;	// Data                      >15
	unsigned char	mon;	// Month                     >16
	unsigned char	year; 	// Year                      >17
	unsigned char 	fr;    	// Rept_freq (Hz)            >18
	unsigned char	an;    	// Ant_type & polarisation flag  >19
							//  & Doppler flag & number of pulses
							// Format: <AAPDnnnn>
	unsigned char	ka;    	// Attenuation               >20
					        // Flags of configuration:
	unsigned char	p_len;	// Pulse length (n*50 mks)   >21
	unsigned char	power;	// Transmitter power n kWt   >22
	unsigned char	ch_r;  	//                       >23
    unsigned char	ver;   	// Version of program    >24
	unsigned int 	yday;	// Day of year (0 - 365; January 1 = 0) >25
	unsigned int    wday;	// Day of week (0 - 6; Sunday = 0)      >27
	unsigned int    ks;     //                       >29
	unsigned int    count;  //                       >31
};

struct ionHeaderNew1 { 	    // === ��������� ����� ��������� ===   
	char project_name[16];	// �������� ������� (PARUS)
	char format_version[8]; // ������ ������� (ION1, ION2,...)
	
	unsigned char   sec;   	// Seconds   
	unsigned char  	min;   	// Minutes 
	unsigned char  	hour; 	// Hours 
	unsigned char  	day;	// Data 
	unsigned char	mon;	// Month
	unsigned char	year; 	// Year
	
	unsigned char 	imp_frq;// ������� ������� ������������ ��������, �� (20...100)	
	unsigned char	imp_duration;// ������������ ����������� ���������, ���
	unsigned char	imp_count;// ����������� ��������� �� ������ �������
	unsigned char	att;    	// ���������� (����������)
	unsigned char	power;	// ��������

	unsigned short f_beg;   // ��������� �������, ���
	unsigned short f_step;  // ��� �� �������, ���
	unsigned short f_end;   // �������� �������, ���

	unsigned short  count;  // ���������� �������� ������
	double dh;              // ��� �� ������, ��
};

struct ionHeaderNew2 { 	    // === ��������� ����� ��������� ===
    unsigned ver; // ����� ������
    struct tm time_sound; // GMT ����� ��������� ����������
    unsigned height_min; // ��������� ������, �
    unsigned height_step; // ��� �� ������, �
    unsigned count_height; // ����� �����
    unsigned switch_frequency; // ������� ������������ ������ ��������� 
	unsigned freq_min; // ��������� �������, ��� (������� ������)
	unsigned freq_max; // �������� �������, ��� (���������� ������)   
	unsigned count_freq; // ����� ������ �� ���� �������
    unsigned count_modules; // ���������� ������� ������������

    // ��������� ������������� ���������.
    ionHeaderNew2(void)
    {
        ver = 2;
        height_min = 0; // ��� �� ��������, ��� ������������ �� �����������. ���� ��������!!!
        height_step = 0;
        count_height = 0;
        switch_frequency = 0;
        freq_min = 0;
        freq_max = 0;    
        count_freq = 0;
        count_modules = 0;
    }
};

struct ionPackedData { // ����������� ������ ����������.
	unsigned size; // ������ ����������� ���������� � ������.
	unsigned char *ptr;   // ��������� �� ���� ������ ����������� ����������.
};

/* =====================================================================  */
/* ������ ��������� ������ ���-������� */
/* =====================================================================  */
/* ������ ������ ���������� � ��������� ��������� ��������� */
struct FrequencyData {
    unsigned short frequency; //!< ������� ������������, [���].
    unsigned short gain_control; // !< �������� ���������� �������� ����������� ��.
    unsigned short pulse_time; //!< ����� ������������ �� ����� �������, [��].
    unsigned char pulse_length; //!< ������������ ������������ ��������, [��c].
    unsigned char band; //!< ������ �������, [���].
    unsigned char type; //!< ��� ��������� (0 - ������� �������, 1 - ���).
    unsigned char threshold_o; //!< ����� ��������� ������������ �����, ���� �������� ������� �� ����� ������������ � ����, [��/��. ���].
    unsigned char threshold_x; //!< ����� ��������� �������������� �����, ���� �������� ������� �� ����� ������������ � ����, [��/��. ���].
    unsigned char count_o; //!< ����� �������� ���������� O.
    unsigned char count_x; //!< ����� �������� ���������� X.
};

/* ������� ������� FrequencyData::count_o �������� SignalResponse, ����������� ������������ �����. */
/* C���� �� ����� ������������ ���� SignalResponse  � SignalSample  ��� ������������ �������� ������� FrequencyData::count_x */
/* �������� SignalResponse, ����������� �������������� ������� � �������� �������� SignalSample ����� ������ �� ���. �������� */
/* FrequencyData::count_o � FrequencyData::count_x ����� ���� ����� ����, ����� ��������������� ������ �����������. */
struct SignalResponse {
    unsigned long height_begin; //!< ��������� ������, [�]
    unsigned short count_samples; //!< ����� ���������
};

/* =====================================================================  */

#pragma pack(pop)

int comp(const void *i, const void *j);
unsigned char getThereshold(unsigned char *arr, unsigned n);

#endif // __ENCODE_H__
