// Test CLI for pgf2webp

#include <iostream>
#include <vector>

#include "pgf2webp.h"

static std::vector<uint8_t> read_file(const char *path) {
  FILE *fd = std::fopen(path, "r");
  if (!fd) throw std::runtime_error("error opening file");

  std::vector<uint8_t> data;
  std::vector<uint8_t> buffer(65536);

  while (true) {
    const std::size_t n = std::fread(&buffer[0], sizeof buffer[0], buffer.size(), fd);
    if (n == 0) break;

    data.reserve(data.size() + n);
    data.insert(data.end(), buffer.begin(), buffer.begin()+n);
  }

  if (std::ferror(fd)) {
    std::fclose(fd);
    throw std::runtime_error("error reading file");
  }

  std::fclose(fd);

  return data;
}

static void write_file(const char *path, const std::vector<uint8_t> data) {
  FILE *fd = std::fopen(path, "w+");
  if (!fd) throw new std::runtime_error("error opening file");

  size_t written = 0;

  while (written != data.size()) {
    const size_t w = fwrite(&data[written], sizeof data[0], data.size()-written, fd);
    if (w == 0) {
      std::fclose(fd);
      throw new std::runtime_error("0-byte write");
    }

    written += w;
  }

  std::fclose(fd);
}

static void usage(const char *argv0) {
    std::cout << "Usage: " << argv0 << " IN_FILE OUT_FILE" << std::endl << std::endl;
    std::cout << "Converts a PGF file to WebP" << std::endl;
}

int main(int argc, char **argv) {
  if (argc != 3) {
    usage(argv[0]);
    return 1;
  }

  const char *in_path = argv[1];
  const char *out_path = argv[2];

  const auto data = read_file(in_path);

  std::cout << "loaded " << data.size() << " bytes of PGF" << std::endl;

  uint8_t *webp_data;
  size_t webp_datalen;
  char *error = NULL;

  if (pgf_to_webp(&data[0], data.size(), 64, &webp_data, &webp_datalen, &error) != 0) {
    std::cerr << "error decoding: " << error << std::endl;
    std::free(error);
    return 1;
  }

  std::cout << "encoded back to " << webp_datalen << " bytes of WebP" << std::endl;

  write_file(out_path, std::vector<uint8_t>(webp_data, webp_data+webp_datalen));

  pgf_to_webp_free(webp_data);

  return 0;
}
