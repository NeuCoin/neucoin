# Frequently Asked Questions

## How can i run the client with `someoption=XXX`?

You can either run the client with this option directly on the command line (`./neucoind -someoption=XXX`) or, if you want a more permanent solution, you may put this option in the coin configuration file (which is located inside a folder inside your home directory).

## What is a "RPC request"?

A RPC ("Remote Procedure Call") is a way to perform low-level operations on a running node. You can send RPC requests using either the command line, or the debug window (Qt client only).

## Can i mine Proof-of-Work blocks with the client?

Just don't. If you really must, launch the client with `gen=1`, but really, don't. The embedded miner is extremely inefficient, and should only be used for testing purposes.

## How can i disable stake generation?

Run the client with `stakegen=0`. Note that you won't mint anything as long as this option is set.

## How can i allow someone to mint for me without them being able to actually spend my coins?

What you're looking for is called "cold minting". The procedure for this to work is a bit complicated:

  - First, you have to generate an address; we will call it the "spending address".
  - The other party has to generate an other address; we will call it the "minting address".
  - Finally, both you and the other party have to do the following RPC command (be careful about the order of the two parameters! Otherwise, you could lock yourself out of your coins):

    ```
    addcoldmintingaddress "<minting address>" "<spending address>"
    ```

If everything went fine, the other party will now be able to mint your coins, sending the rewards on the same address (effectively collecting your interests for you). Your coins will still appear in your balance, and you will be able to spend them as usual (just note that they will be unavailable for a short period after each successful minting they contributed to, as always).
