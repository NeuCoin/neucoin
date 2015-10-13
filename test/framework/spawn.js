import { CLIENT_PATH, MINER_PATH }       from '../_config';
import { BASE_DATADIR }                  from '../_config';
import { BASE_PORT, BASE_RPC_PORT }      from '../_config';
import { RPC_USER, RPC_PASSWORD }        from '../_config';

import { compact, flatten, last, merge } from './lodash';
import { delayExecution }                from './time';

var gClientId = 0;

var spawn = require( 'child_process' ).spawn;

function checkPort( port ) {

    return new Promise( ( resolve, reject ) => {

        let sock = connect( { port }, ( ) => {

            resolve( true );
            sock.end( );

        } ).on( 'error', ( ) => {

            resolve( false );

        } );

    } );

}

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
        rpcallowip : '*',

        gen : false,
        stakegen : false,

        dnsseed : false

    }, options );

    await resetPath( last( compact( flatten( [ options.datadir ] ) ) ) );

    var commandLine = [ ];

    for ( var option of Object.keys( options ) ) {

        var values = flatten( [ options[ option ] ] );

        for ( var value of values ) {

            if ( value == null )
                continue ;

            if ( value === true ) {
                commandLine.push( `-${option}` );
            } else if ( value === false ) {
                commandLine.push( `-${option}=0` );
            } else {
                commandLine.push( `-${option}=${value}` );
            }

        }

    }

    var client = spawn( CLIENT_PATH, commandLine );

    var removeFromKillList = GLOBAL.addToKillList( client.pid );
    client.on( 'exit', removeFromKillList );

    client.port = last( compact( flatten( [ options.port ] ) ) );
    client.rpcPort = last( compact( flatten( [ options.rpcport ] ) ) );

    client.target = `127.0.0.1:${client.port}`;

    console.log( 'Client spawned. Delaying execution to leave it some time to boot' );
    await delayExecution( 40 );
    console.log( 'Done, thanks for waiting! Resuming test execution' );

    return client;

}

export async function spawnMiner( client ) {

    var miner = spawn( MINER_PATH, [ `-o`, `http://127.0.0.1:${client.rpcPort}`, `-u`, RPC_USER, `-p`, RPC_PASSWORD, `-t1` ] );
    GLOBAL.addToKillList( miner.pid );

    console.log( 'Miner spawned' );

    return miner;

}
