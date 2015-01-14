import { RPC_USER, RPC_PASSWORD } from '../_config';

export function sendRpcQuery( client, json ) {

    return new Promise( ( resolve, reject ) => {

        var data = JSON.stringify( json );

        var request = require( 'http' ).request( {

            method : 'POST',

            host : '127.0.0.1',
            port : client.rpcPort,
            path : '/',

            auth : `${RPC_USER}:${RPC_PASSWORD}`,

            headers : {
                'Content-Type' : 'application/json',
                'Content-Length' : data.length
            }

        } );

        request.on( 'response', response => {

            var stringBuffer = '';

            response.on( 'data', data => {

                stringBuffer += data.toString( );

            } );

            response.on( 'end', ( ) => {

                try {
                    resolve( JSON.parse( stringBuffer ) );
                } catch ( error ) {
                    reject( error );
                }

            } );

            response.on( 'error', error => {

                reject( error );

            } );

        } );

        request.on( 'error', error => {

            reject( error );

        } );

        request.write( data );
        request.end( );

    } );

}
