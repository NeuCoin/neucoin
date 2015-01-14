function delay( seconds ) {

    return new Promise( ( resolve, reject ) => {

        setTimeout( ( ) => {

            resolve( );

        }, seconds * 1000 );

    } );

}

export async function delayExecution( seconds ) {

    if ( seconds < 5 * 60 )
        return await delay( seconds );

    let aboutMinutes = Math.round( seconds / 60 );
    let maxStep = ( aboutMinutes / 5 ) * 60;

    process.stdout.write( `Scheduled to wait for quite a long time (about ${aboutMinutes} minutes). Don't fear.\n` );

    while ( seconds > 0 ) {

        let step = Math.min( seconds, maxStep );
        seconds -= step;

        await delay( step );

        if ( seconds > 0 ) {

            let amount = seconds < 60 ? seconds : Math.round( seconds / 60 );
            let unit = seconds < 60 ? 'second' : 'minute';

            if ( amount !== 1 )
                unit += 's';

            process.stdout.write( `About ${amount} ${unit} left.\n` );

        }

    }
}
