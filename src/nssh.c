#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nssh_lib.h"

#define HOST_MAXLENGTH 100

void Exit() {
	printf("\n");
	exit(0);
}

int main(int argc, char** argv) {

	printf("\nNSSH by Yeahlowflicker.\n\n");

	if (!argv[1]) {
		printf("Commands: [\n    nssh <server_name> <username>\n    nssh add <server_name> <host>\n    nssh update <old_server_name> <server_name> <host>\n    nssh delete <server_name>\n]\n");
		Exit();
	}

	HostRecord* records = LoadDatabase();

	if (strncmp(argv[1], "list", 4) == 0 || strncmp(argv[1], "ls", 2) == 0) {
		ListRecords(records);
		Exit();
	}

	if (!argv[1]) {
		printf("\nUsage: nssh <server-name> [username]\n\n");
		Exit();
	};


	if (strncmp(argv[1], "add", 3) == 0) {
		AddRecord(records, argv[2], argv[3]);
		Exit();
	}

	if (strncmp(argv[1], "update", 6) == 0) {
		UpdateRecord(records, argv[2], argv[3], argv[4]);
		Exit();
	}

	if (strncmp(argv[1], "remove", 6) == 0) {
		DeleteRecord(records, argv[2]);
		Exit();
	}

	const char* servername = argv[1];
	const char* ovr_username = argv[2] ? argv[2] : "";


	printf("\nResolving destination ip address for \"%s\"...\n", servername);
	HostRecord* const record = GetRecordByName(records, servername);
	const char* ip = record->host;

	if (strlen(ip) == 0) {
		printf("\nERROR: Host cannot be found.\n\n");
		Exit();
	}

	printf("Constructing SSH command...\n");

	const char* cmd = ConstructSSHCommand(record, ovr_username);

	printf("\nRunning command: %s\n\n", cmd);
	printf("================================\n\n");
	int status = system(cmd);
	printf("\n==============================\n");

	if (status == 255) {
		printf("\nThe identification key for this host has been changed. The system will now re-generate the key...\n");
		char* reset_command = (char*)calloc(HOST_MAXLENGTH, sizeof(char));
		strcat(reset_command, "ssh-keygen -R");
		strcat(reset_command, ip);
		printf("Key generation complete. Retrying SSH connection...\n\n");
		status = system(cmd);
	}

	printf("\nSSH session has been closed.\n\n");

	return 0;
}
