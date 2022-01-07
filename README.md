# Ransomware Project
> by Aurélien and Alexandre

As part of our studies in system security at Henallux, we have to realize a ransomware

## Installation

**Make sure you have the OpenSSL library installed on your machines**
```shell
user@hostname$ sudo apt-get install libssl-dev
```
You must have 2 virtual machines (victim and server) in internal network to test the program. Put **ransom** files into victim VM and the **server** files into server VM.

* Victim VM has IP address 192.168.10/24
* Server VM has IP address 192.168.20/24

On each VMs, run the following command to clone the repository:
```shell
user@hostname$ git clone https://github.com/Asb023/ransomware.git
```
**PS :** You will have direct access to the executable files by cloning this GitHub repository so no need to recompile the files

## Compilation of ransom.c and server.c
**On Victim machine :**
```shell
gcc -Wall -o ransom ransom.c ransomlib.c -lcrypto
```
**On Server machine :**
```shell
gcc -o server server.c
```
## Execution : server

```shell
user@hostname$ ./server
```
**PS :** Run the server before starting the encryption
## Execution : ransom

### Encryption

```shell
user@hostname$ ./ransom -e [path]
```
- ***-e*** : Ransomware encryption mode
- ***[path]*** : Start path of encryption [char *]

### Payment

```shell
user@hostname$ ./ransom -pay
```
- ***-pay*** : Payment mode

**Warning :** Without running this command, you will not be able to decrypt!

### Decryption
```shell
user@hostname$ ./ransom -d [path] [key] [iv]
```
- ***-d*** : Ransomware decryption mode
- ***[path]*** : Start path of encryption [char *]
- ***[key]*** : AES-256-CBC symetric key in hexadecimal [char *] - optional parameter
- ***[iv]*** : Initializing vector in hexadecimal [char *] - optional parameter

**PS :** If you pay the ransom, you can execute this without filling in the key and the iv.
### Help

```shell
user@hostname$ ./ransom -usage
```
- ***-usage*** : Display the ransomware help
