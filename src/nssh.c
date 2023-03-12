#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "nssh_lib.h"

#define HOST_MAXLENGTH 100

void ValidateArgumentCount(char** const argv, const int count) {
	for (int i = 0; i < count; i++) {
		if (!argv[i]) {
			LogError("Insufficient arguments. Program will be exited.\n\n");
			Exit();
		}
	}
}

int main(int argc, char** argv) {

	system("clear");
	PrintCyan("\nNSSH by Yeahlowflicker.\n");

	if (!argv[1]) {
		printf("\nCommands: [\n    nssh [server_name] <username> <port>\n    nssh list/ls\n    nssh add [server_name] [host]\n    nssh update [old_server_name] [server_name] [host]\n    nssh remove/rm [server_name]\n]\n");
		Exit();
	}

	HostRecord* records = LoadDatabase();

	if (strncmp(argv[1], "list", 4) == 0 || strncmp(argv[1], "ls", 2) == 0) {
		ListRecords(records);
		Exit();
	}

	if (!argv[1]) {
		printf("\nUsage: nssh <server-name> [username] [port]\n\n");
		Exit();
	};


	if (strncmp(argv[1], "add", 3) == 0) {
		ValidateArgumentCount(argv, 5);
		AddRecord(records, argv[2], argv[3], argv[4], argv[5]);
		Exit();
	}

	if (strncmp(argv[1], "update", 6) == 0) {
		ValidateArgumentCount(argv, 6);
		UpdateRecord(records, argv[2], argv[3], argv[4], argv[5], argv[6]);
		Exit();
	}

	if (strncmp(argv[1], "remove", 6) == 0 || strncmp(argv[1], "rm", 6) == 0) {
		ValidateArgumentCount(argv, 2);
		DeleteRecord(records, argv[2]);
		Exit();
	}

	const char* servername = argv[1];
	const char* ovr_username = argv[2] ? argv[2] : "";
	const char* ovr_port = (argv[2] && argv[3]) ? argv[3] : "";


	// Find record
	LogInfo("Resolving destination ip address... ");

	HostRecord* const record = GetRecordByName(records, servername);
	if (!record) {
		PrintRed("FAILED\n");
		sleep(1);
		LogError("Could not find any matching records.\n");
		Exit();
	}
	const char* ip = record->host;
	if (strlen(ip) == 0) {
		LogError("Host could not be found.\n\n");
		Exit();
	}

	PrintGreen("OK");


	// Show connection info
	sleep(1);
	printf("\n\n\n==== Connection Info ====\n");
	printf("· Server Name: %s\n", record->servername);
	printf("· Host: %s\n", record->host);
	printf("· Username: %s\n", argv[2] ? argv[2] : record->default_username);
	printf("· Port: %s\n\n", (argv[2] && argv[3]) ? argv[3] : record->default_port);


	// Construct SSH Command
	LogInfo("Constructing SSH command... ");
	const char* cmd = ConstructSSHCommand(record, ovr_username, ovr_port);
	sleep(1);
	PrintGreen("OK");

	sleep(1);
	printf("\n\n> ");
	PrintYellow(cmd);
	printf("\n\n");


	LogInfo("SSH session started.\n\n");
	int status = system(cmd);
	LogInfo("SSH session ended.\n\n");

	if (status == 255) {
		printf("\nThe identification key for this host has been changed. The system will now re-generate the key...\n");
		char* reset_command = (char*)calloc(HOST_MAXLENGTH, sizeof(char));
		strcat(reset_command, "ssh-keygen -R");
		strcat(reset_command, ip);
		printf("Key generation complete. Retrying SSH connection...\n\n");
		status = system(cmd);
	}

	LogInfo("SSH session has been closed.\n\n\n");

	return 0;
}
