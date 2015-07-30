import { compileWith }       from './framework/compilation';
import { mineSomePowBlocks } from './framework/mining';
import { sendRpcQuery }      from './framework/query';
import { spawnClient }       from './framework/spawn';
import { delayExecution }    from './framework/time';

export async function test( ) {

    await compileWith( {

        COINBASE_MATURITY : 5,

        COIN_PREMINE      : `10 * COIN`,
        POW_BLOCK_REWARD  : ` 0 * COIN`,

        MIN_TX_FEES       : ` 0 * COIN`,
        MIN_RELAY_TX_FEES : ` 0 * COIN`,

        POW_TARGET_SPACING : ` 1 * MINUTE`,
        POS_TARGET_SPACING : ` 1 * MINUTE`,
        TARGET_TIMESPAN    : `10 * MINUTE`

    } );

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
