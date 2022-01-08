# Projet Ransomware
> par Aurélien and Alexandre

Dans le cadre de nos études en sécurité des systèmes à l'Henallux, nous devons réaliser un ransomware. Ce ransomware est uniquement à but pédagogique.

Nous vous rappelons qu'un ransomware est un logiciel malveillant qui va bloquer vos données en les chiffrant et en vous demandant un paiement en échange du déchiffrement de vos données.

## Installation

Vous devrez disposer de 2 machines virtuelles (***victime*** et ***serveur***) en réseau interne pour tester le programme.

Dans ***ransom_files***, vous trouverez le fichier exécutable du ransomware et son code source. Installez l'exécutable sur la **VM victime**.
 
Dans ***server_files***, vous trouverez le fichier exécutable du server et son code source. Installez l'exécutable sur le  ***serveur***.


> * Victim a comme adresse IP 192.168.10/24
> 
> * Server a comme adresse IP 192.168.20/24

Sur chaque VM, clonez le répertoire :
```shell
user@hostname$ git clone https://github.com/Asb023/ransomware.git
```

## Compilation de ransom.c et server.c

Vous pouvez également compiler le code source. Pour ce faire, assurez-vous que la bibliothèque OpenSSL est installée.
```shell
user@hostname$ sudo apt-get install libssl-dev
```
**Sur la VM Victim :**
```shell
gcc -Wall -o ransom ransom.c ransomlib.c -lcrypto
```
**Sur le server :**
```shell
gcc -o server server.c
```
## Execution : server

```shell
user@hostname$ ./server
```
**PS :** Lancer l'exécution du server avant de lancer l'exécution de ransom.
## Execution : ransom

### Chiffrement

```shell
user@hostname$ ./ransom -e [path]
```
- ***-e*** : Mode de chiffrement du ransomware
- ***[path]*** : Répertoire de départ du chiffrement [char *]

### Paiement

```shell
user@hostname$ ./ransom -pay
```
- ***-pay*** : Va envoyer le "paiement" au serveur

**Attention :** Sans lancer cette commande, vous ne saurez pas déchiffrer !

### Déchiffrement
```shell
user@hostname$ ./ransom -d [path] [key] [iv]
```
- ***-d*** : Mode de déchiffrement du ransomware
- ***[path]*** : Répertoire de départ du déchiffrement [char *]
- ***[key]*** : Clé de chiffrement symétrique AES-256-CBC en hexadecimal [char *] - paramètre optionnel
- ***[iv]*** : Vecteur d'initialisation en hexadecimal [char *] - paramètre optionnel

**PS :** Si la rançon est payée, vous pourrez exécuter cet option sans rensigner la clé de chiffrement et le vecteur d'initialisation.
### Aide

```shell
user@hostname$ ./ransom -usage
```
- ***-usage*** : Affiche l'aide
