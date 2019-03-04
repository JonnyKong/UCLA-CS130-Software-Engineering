/**
 * Common definitions for meme-related handlers.
 */

#ifndef MEME_COMMON_H
#define MEME_COMMON_H

typedef struct MemeEntry_ {
  std::string image;
  std::string top;
  std::string bottom;
  MemeEntry_(std::string image_, std::string top_, std::string bottom_) {
      image = image_;
      top = top_;
      bottom = bottom_;
  }
} MemeEntry;


/**
 * createTableIfNotExists() - When the server is deployed to a new server, need
 *  to create the database and table. This function is called by the constructor.
 */
void maybeInit(std::string database_name);

#endif  // MEME_COMMON_H