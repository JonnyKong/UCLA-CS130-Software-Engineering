/**
 * Common definitions for meme-related handlers.
 */

#ifndef MEME_COMMON_H
#define MEME_COMMON_H

typedef struct MemeEntry_ {
  std::string image;
  std::string top;
  std::string bottom;
  int id;
  MemeEntry_(std::string image_, std::string top_, std::string bottom_, int id_ = 0) {
      image = image_;
      top = top_;
      bottom = bottom_;
      id = id_;
  }
} MemeEntry;


void maybeInit(std::string database_name);
std::map<std::string, std::string> parseRESTParams(const std::string &uri);
std::string urlDecode(const std::string &url_encoded);
std::string escape(const std::string &data);

#endif  // MEME_COMMON_H