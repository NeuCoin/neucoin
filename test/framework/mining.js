import { sendRpcQuery } from './query';
import { spawnMiner }   from './spawn';

/**
 * This function guarantee that at least N blocks will be mined, but cannot guarantee that exactly N blocks will be mined.
 */

export async function mineSomePowBlocks( client, count ) {

    let s = count === 1 ? '' : 's';
    console.log( `We will try to mine ${count} PoW block${s}` );

    let req = await sendRpcQuery( client, { method : 'generatework', params : [ count ] } );

    if ( req.error )
        throw new Error( 'An error happened' );

    if ( req.result.length < count )
        throw new Error( 'The client hasn\'t been able to mine enough blocks' );

    console.log( 'Mining successful!' );

    return req.result;

}

/**
 */

export async function mineForMaturation( client ) {

    console.log( `We will now try to confirm the coinbase transactions by mining a single block` );

    let req = await sendRpcQuery( client, { method : 'generatework', params : [ 1 ] } );

    if ( req.error )
        throw new Error( 'An error happened' );

    if ( req.result.length < 1 )
        throw new Error( 'The client hasn\'t been able to mine enough blocks' );

    console.log( 'Mining successful!' );

    return req.result;

}

/**
 */

export async function mintSomePosBlocks( client, count ) {

    let s = count === 1 ? '' : 's';
    console.log( `We will try to mine ${count} PoS block${s}` );

    let req = await sendRpcQuery( client, { method : 'generatestake', params : [ count ] } );

    if ( req.error )
        throw new Error( 'An error happened' );

    if ( req.result.length < count )
        throw new Error( 'The client hasn\'t been able to mine enough blocks' );

    return req.result;

}
