Building on Windows
----------------

You can download and compile the Rigs of Rods sources completely with tools that are available for free.

Required Software
-----------------

Download and install all required software first:

- [Visual Studio 2022 Community
  ](https://visualstudio.microsoft.com/downloads/) (In the "Workloads" tab enable **Desktop development with C++**)
- [Git](https://git-scm.com)
- [CMake](https://cmake.org/download/)
- [Conan](https://conan.io/downloads.html)
- Latest RoR sources: Clone them with
  git: `git clone --recursive https://github.com/RigsOfRods/rigs-of-rods.git C:\dev\rigsofrods-source` )

Note: make sure that you installed cmake with the "add to path" option enabled, otherwise you will get errors when
building RoR  
![add to path](https://cgold.readthedocs.io/en/latest/_images/installer-03.png)

(Restart your computer after installing all the above tools)

Setting up the environment
--------------------------

Since 0.39.2 it is possible to build Rigs of Rods under Windows with precompiled dependencies for 32 bit and 64 bit.
Depending on what you compile, some steps might slightly differ. Also, you can only build x64 (64-bit version) on a real
64-bit platform. However, you can still build x86 (32 bit) on a 64-bit platform due to backwards compatibility.

Using CMake
-----------

- Start cmake from the Windows start menu: START-->Programs-->CMake-->cmake-gui
- *Where is the source code*: ```C:\dev\rigsofrods-source\```
- *Where to build the binaries*: ```C:\dev\rigsofrods-source\build```

- Click the ```Configure``` button.
    - Generator: ```Visual Studio 17 2022```
    - Platform: ```x64```
    - Select ```Use default native compilers```.

- Say ```yes``` if it asked if the directory should be created.
- Click ```Configure``` twice until all entries are white

- Click ```Generate``` and close CMake

Compiling the source code
-------------------------

- Navigate in Windows Explorer to the folder ```C:\dev\rigsofrods-source\build``` and open the file RoR.sln. Visual
  Studio should open. (if you are asked, open with Visual Studio for C++ or Visual Studio 2022)

- In Visual Studio, do the following: Set build-type to **Release** and then from the menu: Build -&gt; build Solution.

- The compilation is done when you can read something like the following in the bottom text output window:  
  ```========== Build: succeeded ==========```

Done! After building you will find RoR ready to use in the bin directory. You can navigate with the Windows Explorer
to ```C:\dev\rigsofrods-source\bin\``` and use it the same way as game directory if it was installed there.
Note: the only difference is that the resources will reside one directory higher, but RoR should figure that out itself.

Troubleshooting
---------------

- If the compile step is ok but you don't see any executable or the log writes some missing libs, it may be something
  wrong with the linking step.
- It may happen that RoR won't start in the bin directory. Simply put the new executable into main release folder and
  try launch from there. If it doesn't, you simply missed something.
- These compile steps and hints are valid **only** with Visual Studio and Windows, although they may be similar for
  other configurations.
- If you need any help compiling or have any other questions, join our discord(https://discord.gg/rigsofrods) and ask
  your question in the #dev channel
