#!/usr/bin/env node

import { createServer } from 'node:net';
import { enable, retrieve } from '../src/index.js';

const server = createServer((socket) => {
  console.log('New client is connecting');

  // Retrieve the collected SYN packet
  const synPacket = retrieve(socket, true);

  console.log('SYN packet:', synPacket);

  // Attempts to retrieve from cache
  console.assert(retrieve(socket, true).length > 0);
  console.assert(retrieve(socket, true).length > 0);
  // Attempt to retrieve without cache
  console.assert(retrieve(socket).length === 0);

  socket.end('HTTP/1.1 200 OK\r\n\r\n');
});

server.listen(3000, () => {
  console.log('TCP server is listening on port 3000');

  // Enable SYN packet collection
  const enabled = enable(server);

  if (enabled) {
    console.log('SYN collecting is enabled');
  }
  else {
    console.warn('SYN collecting is not supported');
  }
});
