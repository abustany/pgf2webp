#include <napi.h>
#include <iostream>

#include "pgf2webp.h"

Napi::Value pgf2webp(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() != 2) {
    Napi::TypeError::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsBuffer() || !info[1].IsNumber()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  const auto targetDecodeSize = info[1].As<Napi::Number>().Int64Value();
  if (targetDecodeSize < 0) {
    Napi::TypeError::New(env, "Target decode size is negative").ThrowAsJavaScriptException();
    return env.Null();
  }

  const auto pgfData = info[0].As<Napi::Buffer<uint8_t>>();

  uint8_t *webpData;
  size_t webpDataLen;
  char *errorMsg = nullptr;

  const auto res = pgf_to_webp(
    pgfData.Data(), pgfData.Length(),
    static_cast<size_t>(targetDecodeSize),
    &webpData, &webpDataLen,
    &errorMsg
  );

  if (res != 0) {
    Napi::TypeError::New(env, errorMsg).ThrowAsJavaScriptException();
    free(errorMsg);
    return env.Null();
  }

  return Napi::Buffer<uint8_t>::New(
    env,
    webpData, webpDataLen,
    [](Napi::Env env, uint8_t* webpData) {
      pgf_to_webp_free(webpData);
    }
  );
}

Napi::Object init(Napi::Env env, Napi::Object exports) {
  exports.Set("pgf2webp", Napi::Function::New(env, pgf2webp));
  return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, init)
