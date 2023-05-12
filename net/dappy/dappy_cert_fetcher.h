#ifndef DAPPY_CERT_FETCHER_H_
#define DAPPY_CERT_FETCHER_H_

#include <memory>
#include <string>

#include "base/memory/ref_counted.h"
#include "net/url_request/url_request.h"
#include "url/gurl.h"

namespace dappy {

class DappyCertFetcher : public net::URLRequest::Delegate {
 private:
  std::unique_ptr<net::URLRequest> url_request_;
  scoped_refptr<net::X509Certificate>* response_cert;
  base::OnceClosure on_done_;
  std::string response_data_;
  std::unique_ptr<net::URLRequestContext> context;
  const size_t read_buffer_size_ = 4096;
  scoped_refptr<net::IOBuffer> read_buffer_;
  // scoped_refptr<base::SequencedTaskRunner> owning_sequence_;  

 public:
  DappyCertFetcher(std::unique_ptr<net::URLRequestContext> context);
  ~DappyCertFetcher() override;

  void FetchCertificate(const GURL& url, scoped_refptr<net::X509Certificate>* cert, base::OnceClosure on_done);

  void OnResponseStarted(net::URLRequest* request, int net_error) override;
  void OnReadCompleted(net::URLRequest* request, int bytes_read) override;
};

}  // namespace dappy

#endif  // DAPPY_CERT_FETCHER_H_
