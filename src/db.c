#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "db.h"

HostRecord* LoadDatabase() {
    HostRecord* data = (HostRecord*)calloc(MAX_RECORD_COUNT, sizeof(HostRecord));
    FILE* file = fopen(DB_DIR, "r");
	assert(file != NULL);

    int i = 0;

	while (fscanf(file, "%s %s %s %s", data[i].servername, data[i].host, data[i].default_username, data[i].default_port) != EOF)
        i++;

	return data;
}

int AppendToDatabase(const HostRecord* const record) {
    FILE* file = fopen(DB_DIR, "a+");
	assert(file != NULL);
    fprintf(file, "%s %s %s %s\n", record->servername, record->host, record->default_username, record->default_port);
    return 0;
}

int WriteDatabaseToFile(const HostRecord* const records) {
    FILE* file = fopen(DB_DIR, "w+");
	assert(file != NULL);

    for (int i = 0; i < MAX_RECORD_COUNT; ++i) {
        if (records[i].servername == "") break;
        fprintf(file, "%s %s %s %s\n", records[i].servername, records[i].host, records[i].default_username, records[i].default_port);
    }

    return 0;
}
