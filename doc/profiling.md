# How to do profiling

## Build with profiling enabled
Either edit src/Makefile.default or start make like this:
```
PROFILING=1 make -f Makefile.yourOS
```

## Advice
neucoin uses a lot of CPU (mostly scrypt) while reading the block database
during startup. This might bias the tests you are doing, so it is best
to first collect profiling data of a startup sequence and copy this
away to not be overwritten. Then start neucoin again and actually do
your test. Later you can use the data from the startup sequence to
calibrate your test data.
