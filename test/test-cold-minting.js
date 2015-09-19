import { compileWith }                                             from './framework/compilation';
import { mineSomePowBlocks, mintSomePosBlocks, mineForMaturation } from './framework/mining';
import { sendRpcQuery }                                            from './framework/query';
import { spawnClient }                                             from './framework/spawn';
import { delayExecution }                                          from './framework/time';

import { fastChain, smallChain }                                   from './_environments';

export async function test( ) {

    await compileWith( fastChain, smallChain );

    var client1 = await spawnClient( );
    var client2 = await spawnClient( { addnode : client1.target } );

    await mineSomePowBlocks( client1, 63 );
    await mineForMaturation( client1, 1 );

    var rpc = await sendRpcQuery( client1, { method : 'getbalance' } );

    expect( rpc.result ).to.equal( 63 );

    var { result : spendingAddress } = await sendRpcQuery( client1, { method : 'getnewaddress' } );
    var { result : mintingAddress } = await sendRpcQuery( client1, { method : 'getnewaddress' } );

    var { result : mintingPrivateKey } = await sendRpcQuery( client1, { method : 'dumpprivkey', params : [ mintingAddress ] } );
    await sendRpcQuery( client2, { method : 'importprivkey', params : [ mintingPrivateKey ] } );

    var { result : coldMintingAddress1 } = await sendRpcQuery( client1, { method : 'addcoldmintingaddress', params : [ mintingAddress, spendingAddress ] } );

    var rpc = await sendRpcQuery( client1, { method : 'sendtoaddress', params : [ coldMintingAddress1, 30 ] } );

    await mineSomePowBlocks( client1, 1 );
    await mineSomePowBlocks( client1, 1 );

    var rpc = await sendRpcQuery( client1, { method : 'getbalance' } );

    expect( rpc.result ).to.equal( 65 );

    await delayExecution( 15 );

    var { result : coldMintingAddress2 } = await sendRpcQuery( client2, { method : 'addcoldmintingaddress', params : [ mintingAddress, spendingAddress ] } );

    expect( coldMintingAddress1 ).to.equal( coldMintingAddress2 );
    await mintSomePosBlocks( client2, 1 );
    await delayExecution( 15 );
    await mineForMaturation( client1, 1 );

    var rpc = await sendRpcQuery( client1, { method : 'getbalance' } );

    expect( rpc.result ).to.be.above( 66 );

}
