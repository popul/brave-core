#ifndef DAPPY_CERT_FETCHER_H_
#define DAPPY_CERT_FETCHER_H_

#include <memory>
#include <string>

#include "base/memory/ref_counted.h"
#include "base/run_loop.h"
#include "net/url_request/url_request.h"
#include "url/gurl.h"

namespace dappy {

class DappyCertFetcher : public net::URLRequest::Delegate {
 public:
  DappyCertFetcher(std::unique_ptr<net::URLRequestContext> context);
  ~DappyCertFetcher() override;

  std::string FetchCertificate(const GURL& url);

  void OnResponseStarted(net::URLRequest* request, int net_error) override;
  void OnReadCompleted(net::URLRequest* request, int bytes_read) override;

  base::RunLoop run_loop_;
  std::unique_ptr<net::URLRequest> url_request_;
  std::string response_data_;
  std::unique_ptr<net::URLRequestContext> context;
  const size_t read_buffer_size_ = 4096;
  scoped_refptr<net::IOBuffer> read_buffer_;
};

}  // namespace dappy

#endif  // DAPPY_CERT_FETCHER_H_
