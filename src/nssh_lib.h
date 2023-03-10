#include <stdio.h>
#include "db.h"

void ListRecords(HostRecord* const records);

HostRecord* GetRecordByName(HostRecord* const records, const char* servername);

char* ConstructSSHCommand(const HostRecord* const record, const char* ovr_username);

int AddRecord(HostRecord* const records, const char* servername, const char* host);

int UpdateRecord(HostRecord* const records, const char* old_servername, const char* servername, const char* host);

int DeleteRecord(HostRecord* const records, const char* servername);
