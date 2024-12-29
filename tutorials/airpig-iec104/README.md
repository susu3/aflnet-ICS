# IEC104 Protocol Implementation

This repository contains an implementation of the IEC 60870-5-104 (IEC104) protocol, which is commonly used in power system automation for telecontrol and data acquisition.

## Quick Start

### Prerequisites
- Linux environment
- GCC compiler
- Git

### Building from Source
1. Clone the repository:
```bash
git clone https://github.com/airpig2011/IEC104.git
cd IEC104
```

2. Apply the following diff to `test/main.c`:

```diff
@@ -155,7 +155,8 @@ void *Iec104_Client(void *arg)
     struct sockaddr_in    servaddr;  
     //uint8_t *dstip = "220.160.62.206";
     int err;
-    uint8_t *dstip = "192.168.1.114";
+    //uint8_t *dstip = "192.168.1.114";
+    uint8_t *dstip = "127.0.0.1";
     pthread_t tid1; 

     int staid = StaCount++;
@@ -179,7 +180,7 @@ void *Iec104_Client(void *arg)

     memset(&servaddr, 0, sizeof(servaddr));  
     servaddr.sin_family = AF_INET;  
-    servaddr.sin_port = htons(6666);  
+    servaddr.sin_port = htons(10000);  
     if(inet_pton(AF_INET, dstip, &servaddr.sin_addr) <= 0){  
         printf("inet_pton error for %s\n",dstip);  
         exit(0);  
```

This changes:
- Server IP from 192.168.1.114 to 127.0.0.1 (localhost)
- Port number from 6666 to 10000

3. Build the project:
```bash
cd test
make
```

### Running the Application

1. Start the server:
```bash
./iec104_monitor -m server -p 10000 -d 127.0.0.1 -n 1
```

2. Start the client:
```bash
./iec104_monitor -m client -p 10000 -d 127.0.0.1 -n 1
```

## Command Line Arguments

- `-m`: Mode (server/client)
- `-p`: Port number
- `-d`: IP address
- `-n`: Number of connections

## Notes
- The server must be started before the client
- Default configuration uses localhost (127.0.0.1) and port 10000
- Make sure no other service is using the specified port
