#include <node_api.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <string.h>

#define SYN_BUF_SIZE 256

/*
 * Enables saving the initial TCP SYN packet on new client sockets
 */
static napi_value Enable(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value args[1];

  if (napi_get_cb_info(env, info, &argc, args, NULL, NULL) != napi_ok) {
    napi_throw_error(env, NULL, "Invalid argument");

    return NULL;
  }

  napi_valuetype argType;

  if (napi_typeof(env, args[0], &argType) != napi_ok || argType != napi_object) {
    napi_throw_error(env, NULL, "Argument is not an object");

    return NULL;
  }

  napi_value handle;

  if (napi_get_named_property(env, args[0], "_handle", &handle) != napi_ok) {
    napi_throw_error(env, NULL, "Object has no _handle property");

    return NULL;
  }

  napi_value handleFd;

  if (napi_get_named_property(env, handle, "fd", &handleFd) != napi_ok) {
    napi_throw_error(env, NULL, "Object has no _handle.fd property");

    return NULL;
  }

  int fd;

  if (napi_get_value_int32(env, handleFd, &fd) != napi_ok) {
    napi_throw_error(env, NULL, "Invalid file descriptor");

    return NULL;
  }

  int syn_mode = 1;

  if (setsockopt(fd, IPPROTO_TCP, TCP_SAVE_SYN, &syn_mode, sizeof(syn_mode)) != 0) {
    napi_throw_error(env, NULL, "Failed setsockopt()");

    return NULL;
  }

  napi_value enabled;

  if (napi_get_boolean(env, 1, &enabled) != napi_ok) {
    napi_throw_error(env, NULL, "Cannot return boolean");

    return NULL;
  }

  return enabled;
}

/*
 * Retrieves saved initial TCP SYN packet from client socket
 */
static napi_value Retrieve(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value args[1];

  if (napi_get_cb_info(env, info, &argc, args, NULL, NULL) != napi_ok) {
    napi_throw_error(env, NULL, "Invalid argument");

    return NULL;
  }

  napi_valuetype argType;

  if (napi_typeof(env, args[0], &argType) != napi_ok || argType != napi_object) {
    napi_throw_error(env, NULL, "Argument is not an object");

    return NULL;
  }

  napi_value handle;

  if (napi_get_named_property(env, args[0], "_handle", &handle) != napi_ok) {
    napi_throw_error(env, NULL, "Object has no _handle property");

    return NULL;
  }

  napi_value handleFd;

  if (napi_get_named_property(env, handle, "fd", &handleFd) != napi_ok) {
    napi_throw_error(env, NULL, "Object has no _handle.fd property");

    return NULL;
  }

  int fd;

  if (napi_get_value_int32(env, handleFd, &fd) != napi_ok) {
    napi_throw_error(env, NULL, "Invalid file descriptor");

    return NULL;
  }

  uint8_t syn_buf[SYN_BUF_SIZE];
  socklen_t syn_buf_size = sizeof(syn_buf);

  memset(syn_buf, 0, sizeof(syn_buf));

  if (getsockopt(fd, IPPROTO_TCP, TCP_SAVED_SYN, syn_buf, &syn_buf_size) != 0) {
    napi_throw_error(env, NULL, "Failed getsockopt()");

    return NULL;
  }

  void* tmp_buf;
  napi_value arrayBuffer;

  if (napi_create_arraybuffer(env, syn_buf_size, &tmp_buf, &arrayBuffer) != napi_ok) {
    napi_throw_error(env, NULL, "Cannot create ArrayBuffer");

    return NULL;
  }

  memcpy(tmp_buf, &syn_buf, syn_buf_size);

  napi_value uint8Array;

  if (napi_create_typedarray(env, napi_uint8_array, syn_buf_size, arrayBuffer, 0, &uint8Array) != napi_ok) {
    napi_throw_error(env, NULL, "Cannot create Uint8Array");

    return NULL;
  }

  return uint8Array;
}

/*
 * Initializes N-API addon
 */
static napi_value Init(napi_env env, napi_value exports) {
  napi_value enableFn;

  if (napi_create_function(env, "enable", NAPI_AUTO_LENGTH, Enable, NULL, &enableFn) != napi_ok) {
    return NULL;
  }

  napi_value retrieveFn;

  if (napi_create_function(env, "retrieve", NAPI_AUTO_LENGTH, Retrieve, NULL, &retrieveFn) != napi_ok) {
    return NULL;
  }

  if (napi_set_named_property(env, exports, "enable", enableFn) != napi_ok) {
    return NULL;
  }

  if (napi_set_named_property(env, exports, "retrieve", retrieveFn) != napi_ok) {
    return NULL;
  }

  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)
