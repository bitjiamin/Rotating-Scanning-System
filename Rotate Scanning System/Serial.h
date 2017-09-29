#include "afxwin.h"
#include <string> 
using namespace std;

#pragma once
class Serial
{
public:
	Serial();
	~Serial();
	bool open_serial();
	void setup_serial();
	CString read_serial();
	bool write_serial(CString data);
	void close_serial();

	CString ReadAngleWave();
	CString ReadTimeWave();

	void ReadCSV(CListBox*);
	void WriteCSV(CString);

	bool print_command(CStringA str);
	HANDLE hCom;
};
extern Serial com;

