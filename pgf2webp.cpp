#include <cstdlib>
#include <cstring>
#include <sstream>
#include <vector>

#include <PGFimage.h>
#include <encode.h>

extern "C" {

static void set_error(char **error_message, const std::string &message) {
  if (error_message == NULL) return;

  *error_message = static_cast<char*>(std::malloc(1+message.size()));
  std::strncpy(*error_message, message.c_str(), 1+message.size());
}

static int pgf_to_webp_unsafe(
  const uint8_t *data,
  const std::size_t datalen,
  const std::size_t target_decode_size,
  uint8_t **webp_data,
  size_t *webp_datalen,
  char **error_msg
) {
  CPGFMemoryStream stream((UINT8*)data, datalen);
  CPGFImage img;
  img.Open(&stream);

  bool has_alpha;

  switch (img.Mode()) {
    case ImageModeRGBColor:
    case ImageModeRGB48:
      has_alpha = false;
      break;
    case ImageModeRGBA:
      has_alpha = true;
      break;
    default:
      set_error(error_msg, "unsupported image mode");
      return 1;
  }

  switch (img.Channels()) {
    case 3: case 4:
      break;
    default:
      set_error(error_msg, "unsupported number of channels");
      return 1;
  }

  switch (img.BPP()) {
    case 24: case 32:
      break;
    default:
      set_error(error_msg, "unsupported BPP");
      return 1;
  }

  int level;
  uint32_t decodedWidth = 0, decodedHeight = 0;

  for (level = img.Levels() - 1; level >= 0; --level) {
    decodedWidth = img.Width(level);
    decodedHeight = img.Height(level);

    if (std::max(decodedWidth, decodedHeight) >= target_decode_size) break;
  }

  if (level < 0) level = 0;

  img.Read(level);

  const auto stride = decodedWidth*4;

  // By default, GetBitmap decodes to BGR(A). WebP expects RGB(A).
  static const int channel_map[] = {2, 1, 0, 3};

  std::vector<uint8_t> bitmap_data(stride*decodedHeight);
  img.GetBitmap(stride, &bitmap_data[0], 32 /* BPP */, const_cast<int*>(channel_map));
  img.Destroy();

  auto webp_encode_func = has_alpha ? WebPEncodeRGBA : WebPEncodeRGB;
  
  *webp_datalen = webp_encode_func(&bitmap_data[0], decodedWidth, decodedHeight, stride, 80, webp_data);

  return 0;
}

int pgf_to_webp(
  const uint8_t *data,
  const std::size_t datalen,
  const std::size_t target_decode_size,
  uint8_t **webp_data,
  size_t *webp_datalen,
  char **error_msg
) {
  if (error_msg != NULL) *error_msg = NULL;

  try {
    return pgf_to_webp_unsafe(data, datalen, target_decode_size, webp_data, webp_datalen, error_msg);
  } catch (const std::exception &e) {
    set_error(error_msg, e.what());
    return 1;
  } catch (const IOException &e) {
    std::stringstream msg;
    msg << "PGF error code: " << e.error;
    set_error(error_msg, msg.str().c_str());
    return 1;
  } catch (...) {
    set_error(error_msg, "unexpected error");
    return 1;
  }
}

void pgf_to_webp_free(uint8_t *data) {
  WebPFree(data);
}

}
