# Coin Test Framework

What lies in this folder is a simple functional testing framework, covering the RPC behavior of the coin.

Every contributor should make sure that their work doesn't break anything here before submitting a PR (we will check it before merging your modifications, and may close your PR without warning if the tests broke). Additionally, we ask every contributor to write tests when they add a new feature, and to consider writing tests for already-existing-but-still-uncovered features.

## How to use

First enter `npm install` (requires npm), then just run `node _runner <module>`. For example, `node _runner test-dynamic-checkpointing`.

## The current test list

This list is not exhausive, but we will do our best to keep it sync'd with the actual test content. Feel free to report any error. In case of doubt, the actual test source code is the reference.

  - **test-dynamic-checkpointing** checks that a dynamic checkpoint is generated at each new PoW block when the private master key is fed into the client command line.
  - **test-reward-maturation** checks that the balance is not yet updated until the mined coins mature.
  - **test-simple-transaction** checks that it is actually possible to mine coins and send them to an other account.

The following tests *may* fail even if they are actually right (it should be rare, tho). That's an issue that we haven't been able to solve yet, that is due to how the blocks are generated. The best thing to do is to run them multiple time and check for a majority of success.

  - **test-pos-block-generation** checks that PoS blocks are actually generated at the correct interval (takes a long time to execute).

## How to write tests ?

Each test file is an ES6 file, which has to export an asynchronous `test` function (`async function test() { ... }`). This function will be called by the runner, and will execute some commands, validating its expectations at each step.

The framework contains a few utilities, splitted in the various files in the `framework` directory. They will not be listed here, but you can check the source code of the current tests and grep the framework files to get the actual list (`grep -R export framework/`).

The `expect` symbol is available inside all test files, check [its actual API](http://chaijs.com/api/bdd/) for more informations.

## Why Javascript and not Python ?

... Why Python and not Javascript? Or Perl? Or Bash? Or OCaml?

ES6 is a great language, which has the benefits of being extremely easy to write and to read. Both are important when dealing with tests, since people usually hate both writing them and spending time to actually understand them.

If the goal is not reached and the community actually think that another language would be a better fit for the tasks, the tests are enough decoupled from the actual language that they can be easily ported to another one.
