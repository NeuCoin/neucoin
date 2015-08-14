# Frequently Asked Questions

## How can i run the client with `someoption=XXX`?

You can either run the client with this option directly on the command line (`./neucoind -someoption=XXX`) or, if you want a more permanent solution, you may put this option in the coin configuration file (which is located inside a folder inside your home directory).

## How can i mine in Proof-of-Work with the client?

Just don't. If you really must, launch the client with `gen=1`, but really, don't. The embedded miner is extremely inefficient, and should only be used for testing purposes.

## How can i disable stake generation?

Run the client with `stakegen=0`. Note that you won't mint anything as long as this option is set.
