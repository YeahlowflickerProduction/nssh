#include <stdio.h>

#define DB_DIR "/home/yeahlowflicker/.nssh_db"
#define MAX_RECORD_COUNT 1000
#define MAX_SERVERNAME_LENGTH 64
#define MAX_HOST_LENGTH 256
#define MAX_USERNAME_LENGTH 64
#define MAX_PORT_LENGTH 5

typedef struct {
    char servername[MAX_SERVERNAME_LENGTH];
    char host[MAX_HOST_LENGTH];
    char default_username[MAX_USERNAME_LENGTH];
	char default_port[MAX_PORT_LENGTH];
} HostRecord;

HostRecord* LoadDatabase();

int AppendToDatabase(const HostRecord* const record);

int WriteDatabaseToFile(const HostRecord* const records);
