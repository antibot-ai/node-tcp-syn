import { retrieve as _retrieve } from './addon.js';

const _cache = new Map();

const retrieve = (socket, cache) => {
  if (cache) {
    const buffer = _cache.get(socket);

    if (buffer) {
      return buffer;
    }
  }

  const buffer = _retrieve(socket);

  if (cache) {
    _cache.set(socket, buffer);

    socket.on('close', () => {
      _cache.delete(socket);
    });
  }

  return buffer;
};

export default retrieve;
