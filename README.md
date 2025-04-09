# C2 Client - Command and Control Client Implementation

## Overview
A lightweight command and control client written in C that establishes a secure connection to a remote server, processes commands, and executes various system tasks. This client implements a robust architecture for remote system management.

## Features
- ✅ System information collection (username, hostname, OS details)
- ✅ Command execution via secure channels
- ✅ File operations (read, delete, move)
- ✅ Reverse shell capabilities
- ✅ Base64 encoding/decoding for secure parameter transmission
- ✅ Persistent connection with heartbeat mechanism
- ✅ Task-based execution model with unique IDs

## Architecture
The client follows a modular design with components for:
- Connection declaration and maintenance
- Command fetching and processing
- Task execution
- Result reporting
- Security encoding/decoding

## Dependencies
- GCC compiler
- OpenSSL library (for Base64 encoding/decoding)
- Linux-based operating system

## Building
```bash
make clean && make
```

## Usage
Simply run the compiled binary:
```bash
./bot_client
```

The client automatically:
1. Declares itself to the C2 server
2. Retrieves commands periodically
3. Executes received commands
4. Reports results back to the server

## Supported Commands
| Command | Description | Format |
|---------|-------------|--------|
| SLEEP | Pause execution | `SLEEP,<task_id>,<seconds_base64>,<random_base64>` |
| CAT | Read file contents | `CAT,<task_id>,<file_path_base64>` |
| RM | Delete a file | `RM,<task_id>,<file_path_base64>` |
| MV | Move/rename a file | `MV,<task_id>,<src_path_base64>,<dst_path_base64>` |
| EXECVE | Execute system command | `EXECVE,<task_id>,<command_base64>,<args_base64>` |
| REVSHELL | Create reverse shell | `REVSHELL,<task_id>,<port_base64>,<ip_base64>` |

## Security Notes
- All sensitive parameters are Base64 encoded
- Command execution is sandboxed
- Results are reported securely to the C2 server

## Communication Protocol
The client uses a simple text-based protocol for server communication:
1. `DECLARE,<username>,<hostname>,<os_info>` - Client registration
2. `FETCH,<user_id>` - Command polling
3. `RESULT,<user_id>,<task_id>,<result_data>` - Command results

## License
This project is licensed under the GNU General Public License v3 (GPL-3.0)
