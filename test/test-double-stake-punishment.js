import { compileWith }                          from './framework/compilation';
import { mineSomePowBlocks, mintSomePosBlocks } from './framework/mining';
import { sendRpcQuery }                         from './framework/query';
import { spawnClient }                          from './framework/spawn';
import { delayExecution }                       from './framework/time';

import { fastChain, smallChain }                from './_environments';

export async function test( ) {

    await compileWith( fastChain, smallChain );

    var client1 = await spawnClient( { } );
    var client2 = await spawnClient( { connect : client1.target } );

    await mineSomePowBlocks( client1, 1 );
    await delayExecution( 15 );

    // We have to mine at least 63 more blocks before getting enough entropy for the
    // stake modifier generation - and without stake modifier, there is no PoS block :)

    await mineSomePowBlocks( client2, 64 );
    await delayExecution( 15 );

    // Check that we have a single UTXO available

    var rpc = await sendRpcQuery( client1, { method : 'listunspent' } );
    expect( rpc.result ).to.have.length( 1 );
    var address = rpc.result[ 0 ].address;

    // Store the current head of the blockchain

    var { result : head } = await sendRpcQuery( client1, { method : 'getbestblockhash' } );

    // Export the main private key

    var { result : privateKey } = await sendRpcQuery( client1, { method : 'dumpprivkey', params : [ address ] } );

    // Start two independent clients (both connected an additional node to be able to be able to mint)

    var client3 = await spawnClient( { connect : [ client1.target ] } );
    var client3X = await spawnClient( { connect : [ client3.target ] } );

    var client4 = await spawnClient( { connect : [ client1.target ] } );
    var client4X = await spawnClient( { connect : [ client4.target ] } );

    // We kill their common ancestor because we want to keep client3 and client4 independent from each other

    client1.kill( 'SIGKILL' );

    // Import the private key on each client so that they are able to mint with the UTXO from the first block we mined

    await sendRpcQuery( client3, { method : 'importprivkey', params : [ privateKey ] } );
    await sendRpcQuery( client4, { method : 'importprivkey', params : [ privateKey ] } );

    // Mint a block on each blockchain. Since they only possess a single UTXO (the same), the minted blocks will use duplicate stakes

    await mintSomePosBlocks( client3, 1 );
    await mintSomePosBlocks( client4, 1 );

    // Ensure that their head has changed (there is 0 reason it wouldn't have, but still)

    var { result : currentHead3 } = await sendRpcQuery( client3, { method : 'getbestblockhash' } );
    var { result : currentHead4 } = await sendRpcQuery( client4, { method : 'getbestblockhash' } );

    expect( currentHead3 ).to.not.equal( head );
    expect( currentHead4 ).to.not.equal( head );

    // Now, we will connect the nodes together; they will share their blocks, and the duplicate stake should be detected and canceled

    await sendRpcQuery( client3, { method : 'addnode', params : [ client4.target, 'onetry' ] } );
    await sendRpcQuery( client4, { method : 'addnode', params : [ client3.target, 'onetry' ] } );

    // Leave them a few seconds to sync

    await delayExecution( 15 );

    // Then expect the heads to have rollback

    var { result : currentHead3 } = await sendRpcQuery( client3, { method : 'getbestblockhash' } );
    var { result : currentHead4 } = await sendRpcQuery( client4, { method : 'getbestblockhash' } );

    expect( currentHead3 ).to.equal( head );
    expect( currentHead4 ).to.equal( head );
}
