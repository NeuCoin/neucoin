if ( ! require( 'fs' ).existsSync( __dirname + '/_config.js' ) )
    throw new Error( 'You need a _config.js file to run the tests. You may want to copy and edit the _config.js.example file.' );

require( 'babel-register' )();
require( 'babel-polyfill' );

GLOBAL.expect = require( 'chai' ).expect;
GLOBAL.bitcore = require( 'bitcore' );

var name = process.argv[ 2 ];

if ( ! name.startsWith( '.' ) && ! name.startsWith( '/' ) )
    name = './' + name;

var m = require( name );

if ( ! m || ! m.test || typeof m.test !== 'function' )
    throw new Error( 'Invalid test module' );

var killList = [ ];

GLOBAL.addToKillList = function ( pid ) {

    if ( killList.indexOf( pid ) === -1 )
        killList.push( pid );

    return function ( ) {
        killList.splice( killList.indexOf( pid ), 1 );
    };

};

function killChildren( ) {

    var currentKillList = killList;
    killList = [ ];

    currentKillList.forEach( function ( pid ) {
        process.kill( pid, 'SIGKILL' );
    } );

}

process.on( 'SIGINT', function ( ) {

    killChildren( );

    process.exit( 1 );

} );

Promise.resolve( m.test( ) ).then( function ( ) {

    console.log( 'Everything seems fine! Congratz.' );

    return 0;

}, function ( error ) {

    console.error( error.stack || error );

    return 1;

} ).then( function ( status ) {

    process.stdout.write( '\x07' );

    killChildren( );

    process.exit( status );

} );
