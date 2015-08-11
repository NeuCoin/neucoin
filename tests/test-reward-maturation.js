import { compileWith }           from './framework/compilation';
import { mineSomePowBlocks }     from './framework/mining';
import { sendRpcQuery }          from './framework/query';
import { spawnClient }           from './framework/spawn';
import { delayExecution }        from './framework/time';

import { fastChain, smallChain } from './_environments';

export async function test( ) {

    await compileWith( fastChain, smallChain, { COINBASE_MATURITY : 5, COIN_PREMINE : `10 * COIN` } );

    var client1 = await spawnClient( { } );
    var client2 = await spawnClient( { addnode : client1.target } );

    var rpc = await sendRpcQuery( client1, { method : 'getbalance' } );

    expect( rpc.result ).to.equal( 0 );

    await mineSomePowBlocks( client1, 1 );

    var rpc = await sendRpcQuery( client1, { method : 'getbalance' } );

    expect( rpc.result ).to.equal( 0 );

    await mineSomePowBlocks( client1, 4 );

    var rpc = await sendRpcQuery( client1, { method : 'getbalance' } );

    expect( rpc.result ).to.be.equal( 0 );

    await mineSomePowBlocks( client1, 1 );

    var rpc = await sendRpcQuery( client1, { method : 'getbalance' } );

    expect( rpc.result ).to.be.equal( 10 );

}
