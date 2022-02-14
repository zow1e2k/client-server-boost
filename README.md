client-server-boost
==============================================
Client-server application to rule unix server via windows

Lib folders
-----------

The lib folders containing **Boost** & **CEF** binaries were removed from the repository because
**GitHub** does not allow files bigger than 100MB, and some of them are.

#### ***[CEF 90.6.5](https://drive.google.com/drive/folders/1-Ckq7oFHzkhuX8S0lbdqZ-AL--_arrdq?usp=sharing)***

#### ***[Boost 1.77.0](https://www.boost.org/users/history/version_1_77_0.html)***

Download and unpack it to **Client/CEF** & **Client/BOOST**

Windows (Win-10 example)
-------

First of all you need install **Visual Studio 2022 (v143)** with Windows SDK 10.0

Include **CEF** & **BOOST** libraries (directory with **.lib** files) in **Project->Properties->Linker->Additional Library Directories**

Add included **CEF** & **BOOST** directories (directory with **.cpp** & **.h** files) in **Project-Properties->VC++ Directories->Included Directories**

Add included **CEF** & **BOOST** directories (**.lib** files) in **Project->Properties->Linker->Input->Additional Dependencies**

Setup subsystem **Windows** in **Project->Properties->Linker->Subsytem**

Go to **Project->Properties->C/C++->Code Creator->Runtime Library** and choose **Multithreaded (/MT)**

Build **Client**

```
cd (/d) path
msbuild Project.sln /p:Configuration=Release
```

Download and extract **chromium** packages & **html** to **x64/Release**

#### **[Release](https://drive.google.com/drive/folders/1IqVCon5dEvwX3LRxNVwC23-G2L5cGsSp?usp=sharing)**

Run **Client**

```
Client.exe
```

Unix (Ubuntu 20.04 example)
-------

Install the latest **g++** & **boost**

```
sudo apt-get update
sudo apt-get install g++

sudo apt-get update
sudo apt-get install libboost-all-dev

mkdir Server
```

Upload **Server/Server.cpp** to **Server** directory on your unix system

Via **PuTTy** by SSH-connection
```
pscp Server.cpp ip@user:/path
```

Compile **Server**
```
g++ -pthread -o Server Server.cpp
```

Log in as root and allow to run **Server** only by root user
```
chmod u+x Server
```

Run **Server**
```
./Server
```

LICENSE
-------
(MIT)

Copyright (c) 2022 Artem Bahmutenko

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
