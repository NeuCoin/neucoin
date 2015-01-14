import { compileWith }       from './framework/compilation';
import { mineSomePowBlocks } from './framework/mining';
import { sendRpcQuery }      from './framework/query';
import { spawnClient }       from './framework/spawn';
import { delayExecution }    from './framework/time';

let BLOCKS_TO_WAIT = 500;

export async function test( ) {

    await compileWith( {

        COINBASE_MATURITY : 5,

        COIN_PREMINE      : `100000 * COIN`,
        POW_BLOCK_REWARD  : `100000 * COIN`,
        MIN_TX_FEES       : `     0 * COIN`,
        MIN_RELAY_TX_FEES : `     0 * COIN`,

        POW_TARGET_SPACING : `10 * SECOND`,
        POS_TARGET_SPACING : `10 * SECOND`,
        TARGET_TIMESPAN    : ` 1 * MINUTE`,

        POS_MAX_TARGET : 'target_t(~uint256(0))'

    } );

    var client1 = await spawnClient( { } );
    var client2 = await spawnClient( { addnode : `127.0.0.1:${client1.port}` } );

    // We mine some blocks that will give us multiple unspent output that we will expect
    // to mint in a close future

    await mineSomePowBlocks( client1, 20 );

    // We have to mine 63 blocks on top of the genesis before getting enough entropy for the
    // stake modifier generation - and without stake modifier, there is no PoS block :)

    await mineSomePowBlocks( client1, 63 );

    // Let's wait some time to var the PoS target stabilize around 10 seconds

    await delayExecution( 60 );

    // Just checking how much block we have - since PoS blocks will also have minted while we
    // were mining PoW blocks.

    var { result : blockCount } = await sendRpcQuery( client1, { method : 'getblockcount' } );

    // We wait a long time, and expect to get about 80% of the expected block count

    await delayExecution( 10 * BLOCKS_TO_WAIT );

    var rpc = await sendRpcQuery( client1, { method : 'getblockcount' } );

    expect( rpc.result - blockCount ).to.be.within( BLOCKS_TO_WAIT * 0.8, BLOCKS_TO_WAIT * 1.2 );

}
