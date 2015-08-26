import { compileWith }           from './framework/compilation';
import { mineSomePowBlocks }     from './framework/mining';
import { sendRpcQuery }          from './framework/query';
import { spawnClient }           from './framework/spawn';
import { delayExecution }        from './framework/time';

import { fastChain, smallChain } from './_environments';

let PUBLIC_KEY = '042f43a2e1e8185eb0f25380f4caa6de24d6a638ceb01374450207c9d25bfa586a20cc1295c686d20e090b324054aa4b93b405c9fea50df8bca0b70cca6b6758fc';
let PRIVATE_KEY = '308201130201010420e8c90e40cdbebaaaa634302153ea1741a28227b1366196ca7a9a727a9e54b800a081a53081a2020101302c06072a8648ce3d0101022100fffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f300604010004010704410479be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798483ada7726a3c4655da4fbfc0e1108a8fd17b448a68554199c47d08ffb10d4b8022100fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364141020101a144034200042f43a2e1e8185eb0f25380f4caa6de24d6a638ceb01374450207c9d25bfa586a20cc1295c686d20e090b324054aa4b93b405c9fea50df8bca0b70cca6b6758fc';

export async function test( ) {

    await compileWith( fastChain, smallChain, { CHECKPOINT_PUBLIC_KEY : `"${PUBLIC_KEY}"` } );

    var client1 = await spawnClient( { checkpointkey : PRIVATE_KEY } );
    var client2 = await spawnClient( { addnode : client1.target } );

    var rpc1 = await sendRpcQuery( client1, { method : 'getcheckpoint' } );
    var rpc2 = await sendRpcQuery( client2, { method : 'getcheckpoint' } );

    expect( rpc1.result.checkpointmaster ).to.be.true;
    expect( rpc2.result.checkpointmaster ).to.be.undefined;

    expect( rpc1.result.height ).to.equal( 0 );
    expect( rpc2.result.height ).to.equal( 0 );

    expect( rpc1.result.synccheckpoint === rpc2.result.synccheckpoint ).to.be.true;

    await mineSomePowBlocks( client2, 10 );
    await delayExecution( 15 );

    var rpc1 = await sendRpcQuery( client1, { method : 'getcheckpoint' } );
    var rpc2 = await sendRpcQuery( client2, { method : 'getcheckpoint' } );

    expect( rpc1.result.height ).to.be.at.least( 9 );
    expect( rpc2.result.height ).to.be.at.least( 9 );

    expect( rpc1.result.height === rpc2.result.height ).to.be.true;
    expect( rpc1.result.synccheckpoint === rpc2.result.synccheckpoint ).to.be.true;

}
