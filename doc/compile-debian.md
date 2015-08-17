# How to build this client for Debian / Ubuntu

  - **[Debian 7.X Wheezy]** Ensure that aptitude is using the backport packages by adding this line into `/etc/apt/sources.list` if it isn't already there:

    ```
    $> deb http://http.debian.net/debian wheezy-backports main
    ```

  - Install the dependencies

    ```
    $> sudo apt-get install g++ make libboost-system-dev libboost-filesystem-dev libboost-program-options-dev libboost-thread-dev libdb++-dev libssl-dev libqrencode-dev libpng-dev qt5-default qttools5-dev-tools pkg-config
    ```

  - Build the binaries by going into the repository's `src` directory and running:

    ```
    $> make -f Makefile.unix
    ```

    Or, if you wish to build the Qt client:

    ```
    $> make -f Makefile.unix neucoin-qt
    ```

    Or, if you wish to build everything:

    ```
    $> make -f Makefile.unix all
    ```
