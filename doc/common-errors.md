# Common errors

## Build errors

### All architectures

#### `make: Circular qt/bitcoin.qrc <- qt/bitcoin.qrc.cpp dependency dropped`

It seems to be normal. I haven't yet been able to find why Make does think that there is a circular dependency, and the warning is just a warning and doesn't prevent a successful compilation.

Should you know how to fix this, feel free to open an issue.
