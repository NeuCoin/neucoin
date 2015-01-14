import { CLIENT_PATH, MINER_PATH }       from '../_config';
import { BASE_DATADIR }                  from '../_config';
import { BASE_PORT, BASE_RPC_PORT }      from '../_config';
import { RPC_USER, RPC_PASSWORD }        from '../_config';

import { compact, flatten, last, merge } from './lodash';
import { delayExecution }                from './time';

var gClientId = 0;

var spawn = require( 'child_process' ).spawn;

function resetPath( path ) {

    return new Promise( ( resolve, reject ) => {

        require( 'rimraf' )( path, err => {

            if ( err )
                return reject( err );

            require( 'mkdirp' )( path, err => {

                if ( err )
                    return reject( err );

                resolve( );

            } );

        } );

    } );

}

export async function spawnClient( options = { } ) {

    var id = gClientId ++;

    options = merge( {

        v : true,
        datadir : `${BASE_DATADIR}/coin-datadir-${id}/`,

        port : BASE_PORT + id,
        rpcport : BASE_RPC_PORT + id,

        rpcuser : RPC_USER,
        rpcpassword : RPC_PASSWORD,
        rpcallowip : '*'

    }, options );

    await resetPath( last( compact( flatten( [ options.datadir ] ) ) ) );

    var commandLine = [ ];

    for ( var option of Object.keys( options ) ) {

        var values = compact( flatten( [ options[ option ] ] ) );

        for ( var value of values ) {

            if ( ! value )
                continue ;

            if ( typeof value === 'boolean' ) {
                commandLine.push( `-${option}` );
            } else {
                commandLine.push( `-${option}=${value}` );
            }

        }

    }

    var client = spawn( CLIENT_PATH, commandLine );
    client.unref( );

    client.port = last( compact( flatten( [ options.port ] ) ) );
    client.rpcPort = last( compact( flatten( [ options.rpcport ] ) ) );

    console.log( 'Client spawned. Delaying execution to leave it some time to boot' );
    await delayExecution( 40 );
    console.log( 'Done, thanks for waiting! Resuming test execution' );

    return client;

}

export async function spawnMiner( client ) {

    var miner = spawn( MINER_PATH, [ `-o`, `http://127.0.0.1:${client.rpcPort}`, `-u`, RPC_USER, `-p`, RPC_PASSWORD, `-t1`, `-a`, `sha256d` ] );

    console.log( 'Miner spawned' );

    return miner;

}
