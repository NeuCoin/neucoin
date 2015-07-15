import { sendRpcQuery } from './query';
import { spawnMiner }   from './spawn';

/**
 * This function guarantee that at least N blocks will be mined, but cannot guarantee that exactly N blocks will be mined.
 */

export async function mineSomePowBlocks( client, count ) {

    let req = await sendRpcQuery( client, { method : 'generatework', params : [ count ] } );

    if ( req.error )
        throw new Error( 'An error happened' );

    if ( req.result.length < count )
        throw new Error( 'The client hasn\'t been able to mine enough blocks' );

    return req.result;

}

/**
 */

export async function mintSomePosBlocks( client, count ) {

    let req = await sendRpcQuery( client, { method : 'generatestake', params : [ count ] } );

    if ( req.error )
        throw new Error( 'An error happened' );

    if ( req.result.length < count )
        throw new Error( 'The client hasn\'t been able to mine enough blocks' );

    return req.result;

}
