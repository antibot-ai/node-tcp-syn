import { createRequire } from 'node:module';

const require = createRequire(import.meta.url);

const tcpSyn = require('../build/Release/tcp-syn.node');
const { enable, retrieve } = tcpSyn;

export { enable, retrieve };
