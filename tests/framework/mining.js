import { spawnMiner } from './spawn';

/**
 * This function guarantee that at least N blocks will be mined, but cannot guarantee that exactly N blocks will be mined.
 */

export function mineSomePowBlocks( client, max ) {

    var target = '(yay!!!)';

    var bufferedOutput = '';
    var mined = 0;

    return new Promise( ( resolve, reject ) => {

        spawnMiner( client ).then( miner => {

            miner.stderr.on( 'data', data => {

                try {

                    bufferedOutput += data.toString( );

                    for ( var index; ( index = bufferedOutput.indexOf( target ) ) !== -1; bufferedOutput = bufferedOutput.slice( index + target.length ) )
                        mined += 1;

                    if ( mined >= max ) {
                        console.log( `At least ${mined} blocks have been mined (${max} expected)` );
                        miner.kill( );
                        resolve( );
                    }

                } catch ( error ) {

                    miner.kill( );
                    reject( error );

                }

            } );

        } ).catch( error => {

            reject( error );

        } );

    } );

}
