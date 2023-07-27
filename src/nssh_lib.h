#include <stdio.h>
#include "db.h"

#define ANSI_COLOR_RED     		"\x1b[31m"
#define ANSI_COLOR_GREEN   		"\x1b[32m"
#define ANSI_COLOR_YELLOW  		"\x1b[33m"
#define ANSI_COLOR_CYAN    		"\x1b[36m"
#define ANSI_COLOR_DARK_GRAY	"\x1b[90m"
#define ANSI_COLOR_RESET   		"\x1b[0m"

void ListRecords(HostRecord* const records);

HostRecord* GetRecordByName(HostRecord* const records, const char* servername);

char* ConstructSSHCommand(const HostRecord* const record, const char* ovr_username, const char* ovr_port);

int AddRecord(HostRecord* const records, const char* servername, const char* host, const char* default_username, const char* default_port);

int UpdateRecord(HostRecord* const records, const int index, const char* servername, const char* host, const char* default_username, const char* default_port);

int DeleteRecord(HostRecord* const records, const int index);

void Exit();

void PrintGreen(const char* msg);
void PrintRed(const char* msg);
void PrintYellow(const char* msg);
void PrintCyan(const char* msg);

void LogInfo(const char* msg);

void LogError(const char* msg);

void LogOK(const char* msg);
