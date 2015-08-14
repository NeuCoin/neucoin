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
    $> make -f Makefile.osx neucoin-client
    ```
