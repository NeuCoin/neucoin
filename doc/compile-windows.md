# How to build this client for Windows

## Prepare your build environment

  - Get a zip extractor such as 7zip, so that it will work with all kind of archives

    *If windows tells you that you cannot open a tar/gz file even after you installed 7zip, tell it to use the binary located at C:\Program Files\7-Zip\7zFM.exe*

  - Get the [Mingw package manager](http://sourceforge.net/projects/mingw/files/Installer/mingw-get-setup.exe/download)

    *When you install Mingw, be careful not to install it into Program Files: shell commands and spaces mix badly*

  - From the Mingw package manager, look for the following and mark them for install:

      - msys-base (bin)
      - msys-autoconf (bin)
      - msys-automake (bin)
      - msys-libtool (bin)

    Then confirm by going to "Installation" > "Apply Changes" > "Apply"

## Prepare the dependencies

### Fetch their sources

Select a folder where you will put all your dependencies. We will use `C:\Dependencies`, but you may want to use something else. The important things is that you better not use a path with spaces inside, since it could confuse the toolchain. So actually, just stick with the advised path and be done with it :)

In most case, you will have to download the tar files and put them in the dependency folder. Some dependencies are a bit different, but we will see that in time. Here's a [screenshot](http://i.imgur.com/JHc7yJW.png) of what you should have in the end.

  - Download this version of the [Win32 Mingw Toolchain](http://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/4.9.2/threads-posix/dwarf/i686-4.9.2-release-posix-dwarf-rt_v3-rev1.7z/download) and put it into your dependency folder

    *The other versions haven't been tested - it seems that some of them, such as i686-5.1-release-posix-dwarf, don't work. Please stick on this version for the time being*

    *You will have to extract it manually, so that the mingw32 folder should be at the root of your dependency folder*

  - Download the sources of the latest [OpenSSL 1.x library](https://www.openssl.org/sources/), and put them into your dependency folder

    *The 1.0.2 branch didn't worked when I tried it. You may want to fallback on the 1.0.1, which should compile fine (I tested with the 1.0.1m)*

  - Download the sources of the latest [BarkeleyDB library](http://www.oracle.com/technetwork/database/database-technologies/berkeleydb/downloads/index-082944.html), and put them into your dependency folder

    ***Take the NC ("without encryption") version of the library***

    *You can also rant against Oracle for requiring a registration to download a freaking library*

    *Did I mention that every single registration field is required? Even Work Phone? Geez*

  - Download the sources of the latest [Boost library](http://www.boost.org/users/download/), and put them into your dependency folder

  - Download the sources of the latest [Miniupnpc library](http://miniupnp.free.fr/files/), and put them into your dependency folder

  - Download the sources of the latest [Protobuf 2.x library](https://github.com/google/protobuf/releases), and put them into your dependency folder

    ***Do not use the 3.x releases for now***

  - Download the sources of the latest [Libpng library](http://sourceforge.net/projects/libpng/files/), and put them into your dependency folder

    *Note that at the time of writing (march 2015), the libpng 1.7 is still beta, which means that your probably shouldn't use it yet*

  - Download the sources of the latest [Libqrencode library](http://fukuchi.org/works/qrencode/), and put them into your dependency folder

  - Download the sources of the two following submodules from the [Qt5 library](http://download.qt.io/archive/qt/), and put them into your dependency folder:

      - qtbase-opensource
            - qttools-opensource

    **Only use the 5.4.X releases for now**

    **Take the zip version! It seems the others might be missing important files, for some reasons (configure.exe/bat)**

    *The helper script (check below) patches the Qt sources to make them compile (an "#include <cstdio>" line is missing, causing the compilation to abort)*

    *You will have to extract them manually into your dependency folder, the dependency compile script can't do it for you (did you know that there's no command line tool to extract zip archive in Windows?)*

If you have correctly followed the instructions, your dependency folder should look like [this](http://i.imgur.com/JHc7yJW.png).

### Compile them all

Now, we will have to compile these library. In order to do this, we will use a script file, just because there's no point in describing every single compilation lines.

Copy the [compile-dependencies.sh](https://github.com/neucoin/neucoin/blob/master/docs/contrib/windows/compile-dependencies.sh) script, and open it with a text editor. We will have to edit a single things:

  - *DEPENDENCY_PATH* is the path where we can find the dependencies we downloaded before. Here's mine:

    ```
    /c/Dependencies/
    ```

And that's all! You shouldn't have to edit the other things. Go in msys shell, and run the script with bash. If something breaks, it would be interesting that you investigate the problem and open an issue to tell us about it, so that we can improve this document.

## Finally build the coin client

### Compilation

Once again, we made a script for you to use. Copy the [compile-client.sh](https://github.com/neucoin/neucoin/blob/master/docs/contrib/windows/compile-client.sh) script, change the `DEPENDENCY_PATH`, and run it with bash from the repository `src/` directory (important).

If you wish to build the Qt client, just pass `neucoin-qt` as script first parameter and be done with it :)
