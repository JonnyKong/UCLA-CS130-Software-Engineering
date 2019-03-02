/**
 * Request handler for accepting meme creation form.
 */

#ifndef REQUEST_HANDLER_MEME_CREATE_H
#define REQUEST_HANDLER_MEME_CREATE_H

#include <iostream>
#include <map>
#include <sqlite3.h> 

#include "request_handler.h"


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



class RequestHandlerMemeCreate : public RequestHandler {
public:
    explicit RequestHandlerMemeCreate(const NginxConfig &config);    /* To conform with sibling class */
    std::unique_ptr<reply> handleRequest(const request &request_) noexcept override;

// Private:
    void maybeInit();
    std::string insertToStorage(const MemeEntry &entry);
    std::map<std::string, std::string> parseRESTParams(const std::string &uri);

    std::string database_name;  // TODO: Use absolute path?
};

#endif  // REQUEST_HANDLER_MEME_CREATE_H
