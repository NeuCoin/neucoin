import { compileWith }       from './framework/compilation';
import { mineSomePowBlocks } from './framework/mining';
import { sendRpcQuery }      from './framework/query';
import { spawnClient }       from './framework/spawn';
import { delayExecution }    from './framework/time';

export async function test( ) {

    await compileWith( {

        COINBASE_MATURITY : `1`,

        COIN_PREMINE : `0`,

        POW_BLOCK_REWARD : `1 * COIN`,

        MIN_TX_FEES       : `0`,
        MIN_RELAY_TX_FEES : `0`,

        POW_TARGET_SPACING : `1 * MINUTE`,
        POS_TARGET_SPACING : `1 * MINUTE`

    } );

    var client1 = await spawnClient( { } );
    var client2 = await spawnClient( { addnode : `127.0.0.1:${client1.port}` } );

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
