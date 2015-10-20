# How to build this client for OSX

  - Install [Brew](http://brew.sh/)

  - Install the dependencies

    ```
    $> brew install berkeley-db libtool boost miniupnpc openssl protobuf qt5 md5sha1sum qrencode libpng
    ```

  - Build the binaries by going into the repository's `src` directory and running:

    ```
    $> make -f Makefile.osx
    ```

    Or, if you wish to build the Qt client:

    ```
    $> make -f Makefile.osx neucoin-qt
    ```

    Or, if you wish to build everything:

    ```
    $> make -f Makefile.osx all
    ```

  - Note that you can also build a .app OSX package (by using `make -f Makefile.osx neucoin-qt.app`) and/or a .dmg OSX bundle (by using `make -f Makefile.osx neucoin-qt.dmg`). In this case, be sure to install [appdmg](https://github.com/LinusU/node-appdmg) first.
