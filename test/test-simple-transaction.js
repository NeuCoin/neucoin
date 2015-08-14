import { compileWith }           from './framework/compilation';
import { mineSomePowBlocks }     from './framework/mining';
import { sendRpcQuery }          from './framework/query';
import { spawnClient }           from './framework/spawn';
import { delayExecution }        from './framework/time';

import { fastChain, smallChain } from './_environments';

export async function test( ) {

    await compileWith( fastChain, smallChain );

    var client1 = await spawnClient( { } );
    var client2 = await spawnClient( { addnode : client1.target } );

    var rpc = await sendRpcQuery( client1, { method : 'getbalance' } );

    expect( rpc.result ).to.equal( 0 );

    await mineSomePowBlocks( client1, 10 );
    await mineSomePowBlocks( client1, 1 );
    await delayExecution( 15 );

    var rpc = await sendRpcQuery( client1, { method : 'getbalance' } ), balance = rpc.result;

    expect( rpc.result ).to.be.equal( 10 );

    var { result : address } = await sendRpcQuery( client2, { method : 'getnewaddress' } );
    var rpc = await sendRpcQuery( client1, { method : 'sendtoaddress', params : [ address, 3 ] } );

    expect( rpc.error ).to.be.null;
    expect( rpc.result ).to.be.ok;

    var rpc = await sendRpcQuery( client1, { method : 'getbalance' } );

    expect( rpc.result ).to.be.equal( 7 );

    await mineSomePowBlocks( client1, 1 ); // the block which will contain the transaction
    await mineSomePowBlocks( client1, 1 ); // the block which will confirm the transaction
    await delayExecution( 15 );

    var rpc = await sendRpcQuery( client2, { method : 'getbalance' } );

    expect( rpc.result ).to.be.equal( 3 );


}
