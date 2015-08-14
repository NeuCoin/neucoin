import { CLIENT_PATH }   from '../_config';
import { MAKEFILE_NAME } from '../_config';

import { merge }         from './lodash';

var ChildProcess = require( 'child_process' );
var Path = require( 'path' );
var Fs = require( 'fs' );

function escapeRegexpString( string ) {

    return string.replace( /[-[\]{}()*+?.,\\^$|#\s]/g, '\\$&' );

}

function updateConstants( constants, parameters ) {

    for ( var name of Object.keys( parameters ) ) {

        var nameMatch = escapeRegexpString( name );
        var value = parameters[ name ];

        var macroRegexp = new RegExp( `(#define\\s+${nameMatch}\\s+).*`, 'g' );
        var variableRegexp = new RegExp( `([a-zA-Z0-9:_<>]+(?:\\s+const)?\\s+${nameMatch}\\s+=\\s+).*`, 'g' );

        constants = constants.replace( macroRegexp, ( _, keep ) => `${keep}${value}` );
        constants = constants.replace( variableRegexp, ( _, keep ) => `${keep}${value};` );

    }

    return constants;

}

export function compile( options = '' ) {

    return new Promise( ( resolve, reject ) => {

        console.log( 'Starting client compilation' );

        var path = Path.dirname( CLIENT_PATH );
        var builder = ChildProcess.exec( `cd ${JSON.stringify(path)} && make -f ${JSON.stringify(MAKEFILE_NAME)} ${options}`, ( error, stdout, stderr ) => {

            if ( error ) {
                console.log( `Compilation failed with status code #${error.code}` );
                reject( error );
            } else {
                console.log( `Compilation achieved` );
                resolve( );
            }

        } );

    } );

}

export async function compileWith( ... parameterList ) {

    if ( parameterList.some( set => ! set ) )
        throw new Error( 'Some of your compilation parameters are empty - it\'s probably an error' );

    var parameters = merge( ... parameterList );

    var path = Path.dirname( CLIENT_PATH );
    var constantsPath = Path.join( path, 'constants.cpp' );
    var modifiedConstantsPath = Path.join( path, 'constants.test.cpp' );

    var constants = Fs.readFileSync( constantsPath ).toString( );

    var modifiedConstants = updateConstants( constants, parameters );
    Fs.writeFileSync( modifiedConstantsPath, modifiedConstants );

    await compile( 'CONSTANTS=constants.test.cpp TESTING=1' );

}
