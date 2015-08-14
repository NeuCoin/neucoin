import { compileWith }           from './framework/compilation';
import { mineSomePowBlocks }     from './framework/mining';
import { sendRpcQuery }          from './framework/query';
import { spawnClient }           from './framework/spawn';
import { delayExecution }        from './framework/time';

import { fastChain, smallChain } from './_environments';

export async function test( ) {

    await compileWith( fastChain, smallChain );

    var client1 = await spawnClient( { } );
    var client1X = await spawnClient( { addnode : client1.target } );

    var client2 = await spawnClient( { } );
    var client2X = await spawnClient( { addnode : client2.target } );

    await mineSomePowBlocks( client1, 5 );
    var { result : head1 } = await sendRpcQuery( client1, { method : 'getbestblockhash' } );

    await mineSomePowBlocks( client2, 10 );
    var { result : head2 } = await sendRpcQuery( client2, { method : 'getbestblockhash' } );

    var client3 = await spawnClient( { addnode : [ client1.target, client2.target ] } );

    await sendRpcQuery( client1, { method : 'addnode', params : [ client3.target, 'onetry' ] } );
    await sendRpcQuery( client2, { method : 'addnode', params : [ client3.target, 'onetry' ] } );

    await delayExecution( 10 );

    var { result : newHead1 } = await sendRpcQuery( client1, { method : 'getbestblockhash' } );
    var { result : newHead2 } = await sendRpcQuery( client2, { method : 'getbestblockhash' } );
    var { result : newHead3 } = await sendRpcQuery( client3, { method : 'getbestblockhash' } );

    expect( newHead1 ).to.equal( head2 );
    expect( newHead2 ).to.equal( head2 );
    expect( newHead3 ).to.equal( head2 );

    await mineSomePowBlocks( client1, 20 );
    var { result : finalHead } = await sendRpcQuery( client1, { method : 'getbestblockhash' } );

    await delayExecution( 10 );

    var { result : newHead2 } = await sendRpcQuery( client2, { method : 'getbestblockhash' } );
    var { result : newHead3 } = await sendRpcQuery( client3, { method : 'getbestblockhash' } );

    expect( newHead2 ).to.equal( finalHead );
    expect( newHead3 ).to.equal( finalHead );
}
