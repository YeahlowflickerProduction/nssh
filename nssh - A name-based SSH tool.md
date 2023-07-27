# nssh

**nssh (name-based ssh)** is a simple tool, written in C, which allows users to save ssh hosts and connect to them without having to remember addresses.

This tool simply maps a user-selected name to a host address (IP/domain). It only constructs and executes regular SSH commands, and **does not alter the actual SSH connection**.

Records are stored in a local file located at the Linux home directory.

## Why This Tool
As a full-stack developer as well as a network manager, I often have to connect to various servers via SSH. It is exhausting to memorize all the IP addresses and domains.

I therefore decided to build this tool to simply the process (mainly because I'm lazy of memorizing). 


## Features
- Custom server names for hosts
- SSH command construction
- Updating/Deletion of records
- Auto-regeneration of SSH keys upon identity failures

## Commands
### Connecting to a host
```
nssh <server_name> <username>
```

### Listing all records
```
nssh list
```

### Adding a new record
```
nssh add <server_name> <username>
```

### Updating an existing record
```
nssh update <old_server_name> <new_server_name> <new_address>
```

### Deleting a record
```
nssh delete <server_name>
```

## Installation
The compiled file can be executed directly from the project folder. You can also copy it to the system bin folder to execute it without having to switch directory.

```
sudo cp ./nssh /usr/bin/nssh
```

## License & Contribution
**GPLv2**

You are welcomed to provide improvements regarding the performance and security of this tool, **but you have to give the changes back**.