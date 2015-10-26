# Common errors

## Mining errors

### I can't mine with CPU Miner 2.4+

The 2.4 branch unfortunately uses a new RPC that is not yet ready to be used in Peercoin / Neucoin. To fix this, just use a lower version (we did our tests with the v2.3.2 and everything went fine), or try using the `--no-gbt` option when starting CPU Miner.

## Build errors

### All architectures

#### `make: Circular qt/bitcoin.qrc <- qt/bitcoin.qrc.cpp dependency dropped`

It seems to be normal. I haven't yet been able to find why Make does think that there is a circular dependency, and the warning is just a warning and doesn't prevent a successful compilation.

Should you know how to fix this, feel free to open an issue.
