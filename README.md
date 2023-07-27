# nssh

`nssh` is a command-line tool built for Linux systems. It aims to simply the SSH workflow by implementing a simple storage. It is developed and maintained by Yeahlowflicker Production.

The tool allows users to save their frequently used SSH info in a persistent storage. They do not have to memorize server info like hostnames, usernames and ports, etc. Instead, they can simply type in the record name and this tool will construct and execute the corresponding SSH command.

Data is stored in the file `.nssh_db` located at the user's home directory. It does not store any password credentials or keys.


## Features
- Persistent server info storage
- SSH command construction
- SSH command execution

## Command Usage

[Required args] <Optional args>

#### Connecting to a Server
`nssh [server_name] -u <override_username> -p <override_port>`

#### Listing All Records
`nssh -L`

#### Adding a New Record
`nssh -A -n [server_name] -h [host (e.g. ip or domain)] -u [default_username] -p [default_port]`

#### Updating an Existing Record
`nssh -U -i [index] -n [server_name] -h [host (e.g. ip or domain)] -u [default_username] -p [default_port]``

#### Deleting an Existing Record
`nssh -D -i [index]`

#### Show Help
`nssh -h`


## Quick Installation
The file `nssh` is a compiled executable. Copy it to the `/usr/bin` folder and you will be able to run `nssh` globally on your system.

## Compilation and Installation
> Be sure to double-check the content of `build.sh` script before executing it on your system

If you prefer compiling `nssh` by yourself, make sure your system has `gcc` installed. Then, run the `build.sh` script and `nssh` will be compiled and copied to the `/usr/bin` directory. You can execute `nssh` globally on your system afterwards.
