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

void ShowHelp() {
	printf("\nUsage: [\n    Connect: nssh [server_name] -p <username> -p <port>\n\n    List: nssh -L\n\n    Add: nssh -A -n [server_name] -h [host] \n            -u [default_username] -p [default_port]\n\n    Update: nssh -U -i [index] -n [server_name] -h [host]\n             -u [default_username] -p [default_port]\n\n    Delete: nssh -D -i [index]\n]\n");
}

int main(int argc, char** argv) {

	system("clear");
	PrintCyan("\nNSSH by Yeahlowflicker.\n");

	HostRecord* records = LoadDatabase();

	char opt;
	char* mode = argv[1];


	//	Show help
	if (!argv[1] || strcmp(mode, "-h") == 0) {
		ShowHelp();
		Exit();
	}

	//	List records
	else if (strcmp(mode, "-L") == 0) {
		ListRecords(records);
		Exit();
	}

	//	Add a new record
	else if (strcmp(mode, "-A") == 0) {
		char* server_name = NULL;
		char* host = NULL;
		char* default_username = NULL;
		char* default_port = NULL;

		while ((opt = getopt(argc, (char **)argv, "n:h:u:p:")) != -1) {
			switch (opt) {
				case 'n': server_name = optarg; break;
				case 'h': host = optarg; break;
				case 'u': default_username = optarg; break;
				case 'p': default_port = optarg; break;
			}
		}

		if (!server_name || !host || !default_username || !default_port) {
			LogError("Insufficient arguments. Program will be exited.\n\n");
			Exit();
		}

		AddRecord(records, server_name, host, default_username, default_port);
		Exit();
	}


	//	Update an existing record
	else if (strcmp(mode, "-U") == 0) {
		int index = -1;
		char* server_name = NULL;
		char* host = NULL;
		char* default_username = NULL;
		char* default_port = NULL;

		while ((opt = getopt(argc, (char **)argv, "i:n:h:u:p:")) != -1) {
			switch (opt) {
				case 'i': index = atoi(optarg); break;
				case 'n': server_name = optarg; break;
				case 'h': host = optarg; break;
				case 'u': default_username = optarg; break;
				case 'p': default_port = optarg; break;
			}
		}

		if (index == -1 || !server_name || !host || !default_username || !default_port) {
			LogError("Insufficient arguments. Program will be exited.\n\n");
			Exit();
		}

		UpdateRecord(records, index, server_name, host, default_username, default_port);
		Exit();
	}


	//	Delete a record
	else if (strcmp(mode, "-D") == 0) {
		int index = -1;

		while ((opt = getopt(argc, (char **)argv, "i:n:h:u:p:")) != -1) {
			switch (opt) {
				case 'i': index = atoi(optarg); break;
			}
		}

		if (index == -1) {
			LogError("Insufficient arguments. Program will be exited.\n\n");
			Exit();
		}

		DeleteRecord(records, index);
		Exit();
	}

	//	Default: connect mode
	else if (argv[1]) {
		const char* servername = argv[1];
		const char* override_username = "";
		const char* override_port =  "";

		while ((opt = getopt(argc, (char **)argv, "u:p:")) != -1) {
			switch (opt) {
				case 'u': override_username = optarg; break;
				case 'p': override_port = optarg; break;
			}
		}


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
		printf("· Username: %s\n", override_username ? override_username : record->default_username);
		printf("· Port: %s\n\n", override_port ? override_port : record->default_port);


		// Construct SSH Command
		LogInfo("Constructing SSH command... ");
		const char* cmd = ConstructSSHCommand(record, override_username, override_port);
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
	}

	return 0;
}
