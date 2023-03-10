#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "db.h"

#define HOST_MAXLENGTH 100

void ListRecords(HostRecord* const records) {
	printf("#\tName\tHost\n==================================\n");
	for (int i = 0; i < MAX_RECORD_COUNT; ++i)
		if (strlen(records[i].servername) > 0)
			printf("[%d]\t%s\t%s\t%s\n", i, records[i].servername, records[i].host, records[i].default_username);
}

HostRecord* GetRecordByName(HostRecord* const records, const char* servername) {
	for (int i = 0; i < MAX_RECORD_COUNT; ++i) {
		if (strncmp(records[i].servername, servername, 100) == 0)
			return &(records[i]);
	}
	return NULL;
}

char* ConstructSSHCommand(const HostRecord* const record, const char* ovr_username) {
    char* cmd = (char*)calloc(HOST_MAXLENGTH, sizeof(char));
	strcat(cmd, "ssh ");
	strcat(cmd, strlen(ovr_username) > 0 ? ovr_username : record->default_username);
	strcat(cmd, "@");
	strcat(cmd, record->host);
    return cmd;
}


int AddRecord(HostRecord* const records, const char* servername, const char* host, const char* default_username) {
	if (GetRecordByName(records, servername)) {
		printf("\nERROR: Record already exists. Aborting...\n\n");
		return 1;
	}

	HostRecord* const record = (HostRecord*)calloc(1, sizeof(HostRecord));
	strcpy(record->servername, servername);
	strcpy(record->host, host);
	strcpy(record->default_username, default_username);
	return AppendToDatabase(record);
}

int UpdateRecord(HostRecord* const records, const char* old_servername, const char* servername, const char* host, const char* default_username) {
	if (!GetRecordByName(records, old_servername)) {
		printf("\nERROR: Record cannot be found. Aborting...\n\n");
		return 1;
	}

	if (GetRecordByName(records, servername)) {
		printf("\nERROR: New server name is already in use. Aborting...\n\n");
		return 1;
	}

	HostRecord* const record = GetRecordByName(records, old_servername);
	if (!record) return 1;
    strcpy(record->servername, servername);
	strcpy(record->host, host);
	strcpy(record->default_username, default_username);
	WriteDatabaseToFile(records);
	return 0;
}

int DeleteRecord(HostRecord* const records, const char* servername) {
	HostRecord* const record = GetRecordByName(records, servername);
	if (!record) return 1;
	strcpy(record->servername, "");
	strcpy(record->host, "");
	strcpy(record->default_username, "");
	WriteDatabaseToFile(records);
	return 0;
}
