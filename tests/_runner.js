if ( ! require( 'fs' ).existsSync( __dirname + '/_config.js' ) )
    throw new Error( 'You need a _config.js file to run the tests. You may want to copy and edit the _config.js.example file.' );

require( 'babel/register' )( { stage : 1 } );

GLOBAL.expect = require( 'chai' ).expect;

var name = process.argv[ 2 ];

if ( ! name.startsWith( '.' ) && ! name.startsWith( '/' ) )
    name = './' + name;

var m = require( name );

if ( ! m || ! m.test || typeof m.test !== 'function' )
    throw new Error( 'Invalid test module' );

Promise.resolve( m.test( ) ).then( function ( ) {

    console.log( 'Everything seems fine! Congratz.' );

    return 0;

}, function ( error ) {

    console.error( error.stack || error );

    return 1;

} ).then( function ( status ) {

    process.on( 'SIGTERM', function ( ) { } );
    process.kill( -process.pid );

    process.stdout.write( '\x07' );

    process.exit( status );

} );
