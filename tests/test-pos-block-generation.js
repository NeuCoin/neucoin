import { compileWith }                 from './framework/compilation';
import { mineSomePowBlocks }           from './framework/mining';
import { sendRpcQuery }                from './framework/query';
import { spawnClient }                 from './framework/spawn';
import { delayExecution }              from './framework/time';

import { mediumFastChain, smallChain } from './_environments';

let BLOCKS_TO_WAIT = 200;

export async function test( ) {

    await compileWith( mediumFastChain, smallChain, { COIN_PREMINE : `1000 * COIN` } );

    var client1 = await spawnClient( { stakegen : true } );
    var client2 = await spawnClient( { addnode : client1.target } );

    // We have to mine at least 63 more blocks before getting enough entropy for the
    // stake modifier generation - and without stake modifier, there is no PoS block :)

    await mineSomePowBlocks( client1, 64 );

    // Just checking how much block we have - since PoS blocks will also have minted while we
    // were mining PoW blocks.

    var { result : blockCount } = await sendRpcQuery( client1, { method : 'getblockcount' } );

    // We wait a long time, and expect to get about 100%(~20%) of the expected block count

    await delayExecution( 6 * BLOCKS_TO_WAIT );

    var rpc = await sendRpcQuery( client1, { method : 'getblockcount' } );

    expect( rpc.result - blockCount ).to.be.within( BLOCKS_TO_WAIT * 0.8, BLOCKS_TO_WAIT * 1.2 );

    console.log( `For reference, ${rpc.result} have been mined instead of ${BLOCKS_TO_WAIT} (~${Math.floor(rpc.result / BLOCKS_TO_WAIT * 10000) / 100}%)` );

}
