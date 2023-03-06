#ifndef PGF2WEBP_H
#define PGF2WEBP_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// pgf_to_webp converts the PGF data to a WebP image.
//
// target_decode_size specifies the target size of the large side of the
// decoded image, the actual decoded size may be bigger.
//
// The WebP data is returned in webp_data, and must be freed using
// pgf_to_webp_free.
//
// pgf_to_webp returns 0 or success or 1 in case of error. If error_msg is not
// null, an error message is stored there in case of error. error_msg must be
// freed by the caller using free() if set by this function.
int pgf_to_webp(
  const uint8_t *data,
  const size_t datalen,
  const size_t target_decode_size,
  uint8_t **webp_data,
  size_t *webp_datalen,
  char **error_msg
);

// pgf_to_webp_free frees the WebP data returned by pgf_to_webp.
void pgf_to_webp_free(uint8_t *data);

#ifdef __cplusplus
}
#endif

#endif
