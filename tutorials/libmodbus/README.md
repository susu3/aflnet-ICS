# Tutorial - Fuzzing libmodbus server
This assumes that you have read the AFLNet README.md before reading this tutorial.

This tutorial was tested on Ubuntu 20.04.

## Step-0. Server compilation
You can download the source code of libmodbus from the [libmodbus](https://github.com/stephane/libmodbus). To compile and set up the libmodbus for fuzzing, please use the following commands.

```bash
cd $WORKDIR
# Clone libmodbus repository
git clone https://github.com/stephane/libmodbus.git
# Move to the folder
cd libmodbus
git checkout 5c14f13
# Apply a patch to change C/C++ compiler to afl-clang-fast/afl-clang-fast++
patch -p1 < $AFLNET/tutorials/libmodbus/fuzzing.patch
# Compile source
./autogen.sh
./configure --enable-static
make clean all
```
When dcmqrscp server source code has been successfully compiled, the hardest part starts. You need to setup the server and check that everything works correctly.

## Step-1. Server setup
To setup dcmqrscp server follow commands:

Initialize DICOM database:

cd $WORKDIR/dcmtk/build/bin

# Create directory for DICOM database
mkdir ACME_STORE
Create configuration file:

cp $AFLNET/tutorials/dcmqrscp/dcmqrscp.cfg ./
Fix paths to ACME_STORE directory in configuration file dcmqrscp.cfg(line: 69).

Start dcmqrscp.

export DCMDICTPATH=$WORKDIR/dcmtk/dcmdata/data/dicom.dic
./dcmqrscp
Check that everything works correctly:

# Open new terminal.
# Install DICOM toolkit:
sudo apt install dcmtk

# Test connection:
echoscu -v localhost 5158
storescu -v localhost 5158 $AFLNET/tutorials/dcmqrscp/test.dcm
findscu -P localhost 5158 $AFLNET/tutorials/dcmqrscp/query.dcm
getscu localhost 5158 $AFLNET/tutorials/dcmqrscp/query.dcm
If everything works correctly, dcmqrscp won't write any logs.

Step-2. Prepare message sequences as seed inputs
You can find various DICOM queries to use as seed inputs in the aflnet/tutorials/dcmqrscp/in-dicom directory. If you want to create your own inputs, please follow the tutorial for fuzzing Live555 RTSP server included in the main AFLNet README.md.

Step-3. Fuzzing
cd $WORKDIR/dcmtk/build/bin
afl-fuzz -d -i $AFLNET/tutorials/dcmqrscp/in-dicom -o out-dicom -N tcp://127.0.0.1/5158 -P DICOM -D 10000 -E -K -R ./dcmqrscp
With this particular version of dcmqrscp, you should get a few crashes after waiting long enough.

 
