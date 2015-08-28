# RPC List

## Summary

<table><tr>
<td><a href="#addcoldmintingaddress">addcoldmintingaddress</a></td>
<td><a href="#addmultisigaddress">addmultisigaddress</a></td>
<td><a href="#addnode">addnode</a></td>
</tr><tr>
<td><a href="#backupwallet">backupwallet</a></td>
<td><a href="#checkwallet">checkwallet</a></td>
<td><a href="#createrawtransaction">createrawtransaction</a></td>
</tr><tr>
<td><a href="#decoderawtransaction">decoderawtransaction</a></td>
<td><a href="#dumpprivkey">dumpprivkey</a></td>
<td><a href="#encryptwallet">encryptwallet</a></td>
</tr><tr>
<td><a href="#getaccount">getaccount</a></td>
<td><a href="#getaccountaddress">getaccountaddress</a></td>
<td><a href="#getaddressesbyaccount">getaddressesbyaccount</a></td>
</tr><tr>
<td><a href="#getbalance">getbalance</a></td>
<td><a href="#getbestblockhash">getbestblockhash</a></td>
<td><a href="#getblock">getblock</a></td>
</tr><tr>
<td><a href="#getblockcount">getblockcount</a></td>
<td><a href="#getblockhash">getblockhash</a></td>
<td><a href="#getblocktemplate">getblocktemplate</a></td>
</tr><tr>
<td><a href="#getcheckpoint">getcheckpoint</a></td>
<td><a href="#getconnectioncount">getconnectioncount</a></td>
<td><a href="#getdifficulty">getdifficulty</a></td>
</tr><tr>
<td><a href="#getgenerate">getgenerate</a></td>
<td><a href="#gethashespersec">gethashespersec</a></td>
<td><a href="#getinfo">getinfo</a></td>
</tr><tr>
<td><a href="#getmininginfo">getmininginfo</a></td>
<td><a href="#getnetworkghps">getnetworkghps</a></td>
<td><a href="#getnewaddress">getnewaddress</a></td>
</tr><tr>
<td><a href="#getpeerinfo">getpeerinfo</a></td>
<td><a href="#getrawmempool">getrawmempool</a></td>
<td><a href="#getrawtransaction">getrawtransaction</a></td>
</tr><tr>
<td><a href="#getreceivedbyaccount">getreceivedbyaccount</a></td>
<td><a href="#getreceivedbyaddress">getreceivedbyaddress</a></td>
<td><a href="#gettransaction">gettransaction</a></td>
</tr><tr>
<td><a href="#getwork">getwork</a></td>
<td><a href="#help">help</a></td>
<td><a href="#importprivkey">importprivkey</a></td>
</tr><tr>
<td><a href="#keypoolrefill">keypoolrefill</a></td>
<td><a href="#listaccounts">listaccounts</a></td>
<td><a href="#listreceivedbyaccount">listreceivedbyaccount</a></td>
</tr><tr>
<td><a href="#listreceivedbyaddress">listreceivedbyaddress</a></td>
<td><a href="#listsinceblock">listsinceblock</a></td>
<td><a href="#listtransactions">listtransactions</a></td>
</tr><tr>
<td><a href="#listunspent">listunspent</a></td>
<td><a href="#makekeypair">makekeypair</a></td>
<td><a href="#move">move</a></td>
</tr><tr>
<td><a href="#repairwallet">repairwallet</a></td>
<td><a href="#reservebalance">reservebalance</a></td>
<td><a href="#sendalert">sendalert</a></td>
</tr><tr>
<td><a href="#sendfrom">sendfrom</a></td>
<td><a href="#sendmany">sendmany</a></td>
<td><a href="#sendrawtransaction">sendrawtransaction</a></td>
</tr><tr>
<td><a href="#sendtoaddress">sendtoaddress</a></td>
<td><a href="#setaccount">setaccount</a></td>
<td><a href="#setgenerate">setgenerate</a></td>
</tr><tr>
<td><a href="#settxfee">settxfee</a></td>
<td><a href="#signmessage">signmessage</a></td>
<td><a href="#signrawtransaction">signrawtransaction</a></td>
</tr><tr>
<td><a href="#stop">stop</a></td>
<td><a href="#submitblock">submitblock</a></td>
<td><a href="#validateaddress">validateaddress</a></td>
</tr><tr>
<td><a href="#verifymessage">verifymessage</a></td>
<td><a href="#walletlock">walletlock</a></td>
<td><a href="#walletpassphrase">walletpassphrase</a></td>
</tr><tr>
<td><a href="#walletpassphrasechange">walletpassphrasechange</a></td>
</tr></table>

## Full details
### addcoldmintingaddress

```
addcoldmintingaddress <minting address> <spending address> [account]
```

Add a cold minting address to the wallet.
The coins sent to this address will be mintable only with the minting private key.
And they will be spandable only with the spending private key.
If [account] is specified, assign address to [account].

### addmultisigaddress

```
addmultisigaddress <nrequired> <'["key","key"]'> [account]
```

Adds a nrequired-to-sign multisignature address to the wallet.
Each key is a bitcoin address, or an hex-encoded public key.
If [account] is specified, assign address to [account].

### addnode

```
addnode <node> <add|remove|onetry>
```

Attempts to add or remove &lt;node&gt; from the addnode list, or try to connect to &lt;node&gt; once.

### backupwallet

```
backupwallet <destination>
```

Safely copies wallet.dat to &lt;destination&gt;, which can be a directory or a path with filename.

### checkwallet

```
checkwallet checkwallet
```

Checks the wallet integrity.

### createrawtransaction

```
createrawtransaction [{"txid":txid,"vout":n},...] {address:amount,...}
```

Create a transaction spending given inputs
(array of objects containing transaction id and output number),
sending to given address(es).
Returns hex-encoded raw transaction.
Note that the transaction&#39;s inputs are not signed, and
it is not stored in the wallet or transmitted to the network.

### decoderawtransaction

```
decoderawtransaction <hex string>
```

Return a JSON object representing the serialized, hex-encoded transaction.

### dumpprivkey

```
dumpprivkey <address>
```

Reveals the private key corresponding to &lt;address&gt;.

### encryptwallet

```
encryptwallet <passphrase>
```

Encrypts the wallet with &lt;passphrase&gt;.

### getaccount

```
getaccount <address>
```

Returns the account associated with the given address.

### getaccountaddress

```
getaccountaddress <account>
```

Returns an address that can be used to receive payments on the specified this account.
Note that this command will generate a new address if the previous has already received payments.

### getaddressesbyaccount

```
getaddressesbyaccount <account>
```

Returns the address list for the given account.

### getbalance

```
getbalance [account] [minconf=1]
```

If [account] is not specified, returns the server&#39;s total available balance.
If [account] is specified, returns the balance in the account.

### getbestblockhash

```
getbestblockhash getbestblockhash
```

Returns the hash of the best block from the longest blockchain.

### getblock

```
getblock <hash> [txinfo] [txdetails]
```

Returns various informations about a block.
If &lt;txinfo&gt; is 1 (not true!), the command will print more detailed transaction infos.
If &lt;txdetails&gt; is 1 (again, not true!), the command will print even more detailed transaction infos.

### getblockcount

```
getblockcount getblockcount
```

Returns the number of blocks in the longest block chain.

### getblockhash

```
getblockhash <index>
```

Returns the hash of the &lt;index&gt;-nth block from the longest blockchain.

### getblocktemplate

```
getblocktemplate [params]
```

Returns the data needed to construct a block to work on:

  - &quot;version&quot; : block version
  - &quot;previousblockhash&quot; : hash of current highest block
  - &quot;transactions&quot; : contents of non-coinbase transactions that should be included in the next block
  - &quot;coinbaseaux&quot; : data that should be included in coinbase
  - &quot;coinbasevalue&quot; : maximum allowable input to coinbase transaction, including the generation award and transaction fees
  - &quot;target&quot; : hash target
  - &quot;mintime&quot; : minimum timestamp appropriate for next block
  - &quot;curtime&quot; : current timestamp
  - &quot;mutable&quot; : list of ways the block template may be changed
  - &quot;noncerange&quot; : range of valid nonces
  - &quot;sigoplimit&quot; : limit of sigops in blocks
  - &quot;sizelimit&quot; : limit of block size
  - &quot;bits&quot; : compressed target of next block
  - &quot;height&quot; : height of the next block

See https://en.bitcoin.it/wiki/BIP_0022 for full specification.

### getcheckpoint

```
getcheckpoint getcheckpoint
```

Returns various informations about the latest synchronized checkpoint.

### getconnectioncount

```
getconnectioncount getconnectioncount
```

Returns the number of connections to other nodes.

### getdifficulty

```
getdifficulty getdifficulty
```

Returns the difficulty as a multiple of the minimum difficulty.

### getgenerate

```
getgenerate getgenerate
```

Returns true if the node has been set to mine PoW blocks.
Note that it is not recommanded for you to mine using the client. It is advised to use a dedicated software instead.

### gethashespersec

```
gethashespersec gethashespersec
```

Returns a recent hashes per second performance measurement while generating.

### getinfo

```
getinfo getinfo
```

Returns an object containing various state info.

### getmininginfo

```
getmininginfo getmininginfo
```

Returns an object containing mining-related information.

### getnetworkghps

```
getnetworkghps getnetworkghps
```

Returns a recent Ghash/second network mining estimate.

### getnewaddress

```
getnewaddress [account]
```

Returns a new NeuCoin address that you can use to receive payments.
If [account] is specified (recommended), the address will be added to the address book, so that payments received on it will be credited to the right account.

### getpeerinfo

```
getpeerinfo getpeerinfo
```

Returns data about each node the client is connected to.

### getrawmempool

```
getrawmempool getrawmempool
```

Returns all transaction ids in memory pool.

### getrawtransaction

```
getrawtransaction <txid> [verbose=0]
```

If verbose=0, returns a string that is
serialized, hex-encoded data for &lt;txid&gt;.
If verbose is non-zero, returns an Object
with information about &lt;txid&gt;.

### getreceivedbyaccount

```
getreceivedbyaccount <account> [minconf=1]
```

Returns the total amount received by addresses from &lt;account&gt;, in transactions with at least [minconf] confirmations.

### getreceivedbyaddress

```
getreceivedbyaddress <address> [minconf=1]
```

Returns the total amount received by &lt;address&gt;, in transactions with at least [minconf] confirmations.

### gettransaction

```
gettransaction <txid>
```

Get detailed information about &lt;txid&gt;

### getwork

```
getwork [data]
```

If [data] is not specified, returns formatted hash data to work on:

  - &quot;midstate&quot; : precomputed hash state after hashing the first half of the data (DEPRECATED)
  - &quot;data&quot; : block data
  - &quot;hash1&quot; : formatted hash buffer for second hash (DEPRECATED)
  - &quot;target&quot; : little endian hash target

If [data] is specified, tries to solve the block and returns true if it was successful.
Note that this command is stateful : each time you call it without parameter, every previous result will be invalidated.

### help

```
help [command]
```

Without argument, lists the available RPC calls. With an argument, return a command full help.

### importprivkey

```
importprivkey <privkey> [label]
```

Adds a private key (as returned by dumpprivkey) to your wallet.

### keypoolrefill

```
keypoolrefill keypoolrefill
```

Fills the keypool.
If your wallet is encryped, this command requires the wallet passphrase to be set with walletpassphrase first.

### listaccounts

```
listaccounts [minconf=1]
```

Returns an object that contains an entry for each account, using the the account name as key, and the account balance as value.

### listreceivedbyaccount

```
listreceivedbyaccount [minconf=1] [includeempty=false]
```

Returns an array of objects containing:

  - &quot;account&quot; : the account name
  - &quot;amount&quot; : the total amount received by addresses from this account
  - &quot;confirmations&quot; : the confirmation count of the most recent transaction included

No payment will be accepted before having reached at least [minconf] confirmations.
If [includeempty] is true, the accounts that haven&#39;t received any payments will be included too.

### listreceivedbyaddress

```
listreceivedbyaddress [minconf=1] [includeempty=false]
```

Returns an array of objects containing:

  - &quot;address&quot; : receiving address
  - &quot;account&quot; : the account of the receiving address
  - &quot;amount&quot; : total amount received by the address
  - &quot;confirmations&quot; : the confirmation count of the most recent transaction included

No payment will be accepted before having reached at least [minconf] confirmations.
If [includeempty] is true, the addresses that haven&#39;t received any payments will be included too.

### listsinceblock

```
listsinceblock [blockhash] [target-confirmations]
```

Get all transactions in blocks since block [blockhash], or all transactions if omitted.

### listtransactions

```
listtransactions [account] [count=10] [from=0]
```

Returns up to [count] transactions, ordered by date descending, skipping the first [from], for account [account].

### listunspent

```
listunspent [minconf=1] [maxconf=9999999]  ["address",...]
```

Returns array of unspent transaction outputs
with between minconf and maxconf (inclusive) confirmations.
Optionally filtered to only include txouts paid to specified addresses.
Results are an array of Objects, each of which has:
{txid, vout, scriptPubKey, amount, confirmations}

### makekeypair

```
makekeypair [prefix]
```

Generates a new public/private key pair.
The [prefix] parameter is optional, and instructs the generation process to look for a key beginning with [prefix] (for vanity purposes).

### move

```
move <fromaccount> <toaccount> <amount> [minconf=1] [comment]
```

Moves &lt;amount&gt; coins from &lt;fromaccount&gt; to &lt;toaccount&gt;.
Note that no actual transaction is created: all that this command does is decrease an account balance, and increase another account balance.

### repairwallet

```
repairwallet repairwallet
```

Tries to repair the wallet if checkwallet reports any problem.
Remember to always backup first! You may use backupwallet for this.

### reservebalance

```
reservebalance [<reserve> [amount]]
```

Enables or disables the reserve according to the boolean &lt;reserve&gt;. As long as the reserve is enabled, the client will prevent at least &lt;amount&gt; of your coins from minting, effectively preventing them from contributing to the network protection.
You may use this command if you want to keep a minimal amount of coins available at all time, as a safety fund.
Called without parameters, this function simply returns the reserve current settings.

### sendalert

```
sendalert <message> <privatekey> <minver> <maxver> <priority> <id> [cancelupto]
```

Signs &lt;message&gt; with &lt;privatekey&gt;, and sends it as an alert to the network nodes that match the &lt;minver&gt;-&lt;maxver&gt; version range.
Note that this command is only intended to be used by the coin administrators, who are the only one to possess the right &lt;privatekey&gt;.
The [cancelupto] parameter will cancel all alert id&#39;s up to the [cancelupto]-nth one.

### sendfrom

```
sendfrom <fromaddress> <toaddress> <amount> [minconf=1] [comment] [comment-to]
```

Creates a transaction transfering &lt;amount&gt; coins to &lt;toaddress&gt;, with &lt;amount&gt; being a real and rounded to the nearest 0.000001.
If your wallet is encryped, this command requires the wallet passphrase to be set with walletpassphrase first.

### sendmany

```
sendmany <fromaccount> {address:amount,...} [minconf=1] [comment]
```

Creates multiple transactions transfering &lt;amount&gt; coins from &lt;fromaccount&gt; to &lt;toaddress&gt;. Each transaction is a single key/value pair in a JSON object.
If your wallet is encryped, this command requires the wallet passphrase to be set with walletpassphrase first.

### sendrawtransaction

```
sendrawtransaction <hex string> [checkinputs=0]
```

Submits raw transaction (serialized, hex-encoded) to local node and network.
If checkinputs is non-zero, checks the validity of the inputs of the transaction before sending it.

### sendtoaddress

```
sendtoaddress <toaddress> <amount> [comment] [comment-to]
```

Creates a transaction transfering &lt;amount&gt; coins to &lt;toaddress&gt;, with &lt;amount&gt; being a real and rounded to the nearest 0.000001.
If your wallet is encryped, this command requires the wallet passphrase to be set with walletpassphrase first.

### setaccount

```
setaccount <address> <account>
```

Sets the account associated with the given address.

### setgenerate

```
setgenerate <generate> [genproclimit]
```

Enables or disables the PoW block generation, according to the &lt;generate&gt; switch.
You may also use this command to set a new miner processor usage limit (-1 is unlimited).

### settxfee

```
settxfee <amount>
```

Sets the base fee that the client will pay when creating a transaction, without &lt;amount&gt; being a real rounded to the nearest 0.01 (cent).
The minimum and default base transaction fee per KB is 1 cent.

### signmessage

```
signmessage <address> <message>
```

Signs a message with the private key of an address.

### signrawtransaction

```
signrawtransaction <hex string> [{"txid":txid,"vout":n,"scriptPubKey":hex},...] [<privatekey1>,...] [sighashtype="ALL"]
```

Sign inputs for raw transaction (serialized, hex-encoded).
Second optional argument (may be null) is an array of previous transaction outputs that
this transaction depends on but may not yet be in the blockchain.
Third optional argument (may be null) is an array of base58-encoded private
keys that, if given, will be the only keys used to sign the transaction.
Fourth optional argument is a string that is one of six values; ALL, NONE, SINGLE or
ALL|ANYONECANPAY, NONE|ANYONECANPAY, SINGLE|ANYONECANPAY.
Returns json object with keys:
  hex : raw transaction with signature(s) (hex-encoded string)
  complete : 1 if transaction has a complete set of signature (0 if not)

### stop

```
stop stop
```

Stops the NeuCoin server.

### submitblock

```
submitblock <hex data> [optional-params-obj]
```

Attempts to submit new block to network.
Note that the [optional-params-obj] parameter is currently ignored.
See https://en.bitcoin.it/wiki/BIP_0022 for full specification.

### validateaddress

```
validateaddress <address>
```

Returns various informations about &lt;address&gt;.

### verifymessage

```
verifymessage <address> <signature> <message>
```

Verifies a message previously signed via signmessage.

### walletlock

```
walletlock walletlock
```

Removes the wallet encryption key from memory, effectively locking the wallet.
After calling this method, you will need to call walletpassphrase again before being able to call any method requiring the wallet to be unlocked.

### walletpassphrase

```
walletpassphrase <passphrase> <timeout> [mintonly]
```

Stores the wallet decryption key in memory for &lt;timeout&gt; seconds.
The [mintonly] parameter is optional, and instructs the node to only allow block minting if set.

### walletpassphrasechange

```
walletpassphrasechange <oldpassphrase> <newpassphrase>
```

Changes the wallet passphrase from &lt;oldpassphrase&gt; to &lt;newpassphrase&gt;.

