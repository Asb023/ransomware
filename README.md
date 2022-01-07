# ransom

As part of our studies in system security at Henallux, we have to realize a ransomware


**Make sure you have the OpenSSL library installed on your machines**
```shell
user@hostname$ sudo apt-get install libssl-dev
```
## Compilation of ransom.c and server.c
**On Victim machine :**
```shell
gcc -Wall -o ransom ransom.c ransomlib.c -lcrypto
```
**On Server machine :**
```shell
gcc -o server server.c
```
## Installation
You must have 2 virtual machine (victim and server). Put **server** file into server VM and the **ransom** file into victim VM.

## Execution
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
