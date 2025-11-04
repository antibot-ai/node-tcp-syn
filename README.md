# TCP SYN packet collector for Node.js

A Node.js N-API addon that collects the initial SYN packet of a client establishing a connection. It seamlessly integrates with built-in Node.js servers and does not require the setup of external packet-capturing interfaces like libpcap or BPF-based ones.

The module's primary purpose is to provide a foundation for in-band passive connection fingerprinting within the standard network processing flow. It returns a SYN packet as a raw Uint8Array, representing the exact bytes as they're seen on a network socket. Further packet parsing and analysis must be performed by dependent modules like fingerprinters.

## Compatibility

The distributed npm package contains native code that works only on Linux systems with Kernel [4.2+](https://lwn.net/Articles/644906/) and requires installed compiler toolchain to build. Other OSes and kernels are not supported nor planned.

## Usage

To collect SYN packets, you must first enable the feature on a `net.Server` instance using the `enable()` function. Once enabled, you can retrieve the collected SYN packets by calling the `retrieve()` function on a client `net.Socket` instance.

Follow the next example, also available to run as [examples/server.js](examples/server.js).

```js
import { createServer } from 'node:net';
import { enable, retrieve } from '@antibot-ai/node-tcp-syn';

const server = createServer((socket) => {
  console.log('New client is connecting');

  // Retrieve the collected SYN packet
  const synPacket = retrieve(socket);

  console.log('SYN packet:', synPacket);

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
```

## API

### `enable(server: net.Server): boolean`

Enables the server to save the initial SYN packet on its client sockets. A return value indicates whether collecting was enabled.

### `retrieve(socket: net.Socket, cache: boolean): Uint8Array`

Retrieves saved SYN packet from the client socket. Retrieval effectively frees memory and deletes saved packet by the kernel, so if you need to get the same SYN multiple times from the same socket, use `cache: true` option.

## License

This project originally emerged from the R&D process at [Antibot LLC](https://antibot.ru/). Now it's licensed under the [MIT license](LICENSE), allowing free use for any purpose with credit to the original authors.
