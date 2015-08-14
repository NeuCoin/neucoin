# How to build this client for Ubuntu

  - Install the dependencies

    ```
    $> sudo apt-get install g++ make libboost-system-dev libboost-filesystem-dev libboost-program-options-dev libboost-thread-dev libdb++-dev libssl-dev libqrencode-dev libpng-dev qt5-default pkg-config
    ```

  - Build the binaries by going into the repository's `src` directory and running:

    ```
    $> make -f Makefile.unix
    ```

    Or, if you wish to build the Qt client:

    ```
    $> make -f Makefile.unix neucoin-client
    ```
