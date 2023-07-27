#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nssh_lib.h"

#define HOST_MAXLENGTH 100

void ValidateArgument(const char* argv) {
	if (strlen(argv) == 0) {
		LogError("Invalid arguments.\n\n");
		Exit();
	}
}

void ListRecords(HostRecord* const records) {
	LogInfo("Listing records...\n");

	printf("\n #\t%-16s %-20s %-10s %-16s\n===================================================================\n", "Name", "Host", "Username", "Port");
	for (int i = 0; i < MAX_RECORD_COUNT; ++i)
		if (strlen(records[i].servername) > 0)
			printf("[%d]\t%-16s %-20s %-10s %-16s\n", i, records[i].servername, records[i].host, records[i].default_username, records[i].default_port);
}

HostRecord* GetRecordByName(HostRecord* const records, const char* servername) {
	ValidateArgument(servername);

	for (int i = 0; i < MAX_RECORD_COUNT; ++i) {
		if (strncmp(records[i].servername, servername, 100) == 0)
			return &(records[i]);
	}
	return NULL;
}

char* ConstructSSHCommand(const HostRecord* const record, const char* ovr_username, const char* ovr_port) {
    char* cmd = (char*)calloc(HOST_MAXLENGTH, sizeof(char));
	strcat(cmd, "ssh ");
	strcat(cmd, strlen(ovr_username) > 0 ? ovr_username : record->default_username);
	strcat(cmd, "@");
	strcat(cmd, record->host);
	strcat(cmd, " -p ");
	strcat(cmd, strlen(ovr_port) > 0 ? ovr_port : record->default_port);
    return cmd;
}


int AddRecord(HostRecord* const records, const char* servername, const char* host, const char* default_username, const char* default_port) {
	ValidateArgument(servername);
	ValidateArgument(host);

	if (GetRecordByName(records, servername)) {
		LogError("Record already exists. Aborting...\n\n");
		return 1;
	}

	LogInfo("Creating record...\n\n");
	HostRecord* const record = (HostRecord*)calloc(1, sizeof(HostRecord));
	strcpy(record->servername, servername);
	strcpy(record->host, host);
	strcpy(record->default_username, default_username);
	strcpy(record->default_port, strlen(default_port) > 0 ? default_port : "22");

	LogInfo("Writing record to database...\n\n");
	const int dbStatus = AppendToDatabase(record);
	LogOK("Successfully added new record.\n\n");

	return dbStatus;
}

int UpdateRecord(HostRecord* const records, const char* old_servername, const char* servername, const char* host, const char* default_username, const char* default_port) {
	ValidateArgument(old_servername);
	ValidateArgument(servername);
	ValidateArgument(host);
	ValidateArgument(default_username);
	ValidateArgument(default_port);

	LogInfo("Retrieving record...\n\n");

	HostRecord* const record = GetRecordByName(records, old_servername);

	if (!record) {
		LogError("Record cannot be found. Aborting...\n\n");
		return 1;
	}

	const HostRecord* existingCheckRecord = GetRecordByName(records, servername);
	if (existingCheckRecord != NULL) {
		if (strncmp(existingCheckRecord->servername, old_servername, MAX_SERVERNAME_LENGTH) != 0) {
			LogError("New server name is already in use. Aborting...\n\n");
			return 1;
		}
	}

	LogInfo("Updating record values...\n\n");
    strcpy(record->servername, servername);
	strcpy(record->host, host);
	strcpy(record->default_username, default_username);
	strcpy(record->default_port, default_port);

	LogInfo("Writing to database...\n\n");
	WriteDatabaseToFile(records);

	LogOK("Record updated successfully.\n\n");
	return 0;
}

int DeleteRecord(HostRecord* const records, const char* servername) {
	ValidateArgument(servername);

	LogInfo("Retrieving record...\n\n");
	HostRecord* const record = GetRecordByName(records, servername);
	if (!record) {
		LogError("Record could not be found.\n\n");
		return 1;
	}

	LogInfo("Erasing record...\n\n");
	strcpy(record->servername, "");
	strcpy(record->host, "");
	strcpy(record->default_username, "");
	strcpy(record->default_port, "");

	LogInfo("Deleting record...\n\n");
	WriteDatabaseToFile(records);

	LogOK("Delete operation complete.\n\n");

	return 0;
}



void PrintGreen(const char* msg) {
	printf("%s%s%s", ANSI_COLOR_GREEN, msg, ANSI_COLOR_RESET);
}
void PrintRed(const char* msg) {
	printf("%s%s%s", ANSI_COLOR_RED, msg, ANSI_COLOR_RESET);
}
void PrintYellow(const char* msg) {
	printf("%s%s%s", ANSI_COLOR_YELLOW, msg, ANSI_COLOR_RESET);
}
void PrintCyan(const char* msg) {
	printf("%s%s%s", ANSI_COLOR_CYAN, msg, ANSI_COLOR_RESET);
}
void LogInfo(const char* msg) {
	printf("\n\e[100m[LOG]   \e[0m  %s", msg);
}
void LogError(const char* msg) {
	printf("\n\e[41m[ERROR] \e[0m  %s%s%s", msg, ANSI_COLOR_RED, ANSI_COLOR_RESET);
}
void LogOK(const char* msg) {
	printf("\n\e[42m[OK]    \e[0m  %s%s%s", msg, ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
}

void Exit() {
	printf("\n");
	exit(0);
}
