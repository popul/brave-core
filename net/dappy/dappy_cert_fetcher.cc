#include "dappy_cert_fetcher.h"

#include "net/base/io_buffer.h"
#include "net/url_request/url_request_context.h"
#include "net/traffic_annotation/network_traffic_annotation.h"

namespace dappy {

DappyCertFetcher::DappyCertFetcher(
    std::unique_ptr<net::URLRequestContext> context)
    : context(std::move(context)) {
      read_buffer_ = base::MakeRefCounted<net::IOBuffer>(read_buffer_size_);
    }

DappyCertFetcher::~DappyCertFetcher() = default;

std::string DappyCertFetcher::FetchCertificate(const GURL& url) {
  url_request_ = std::unique_ptr<net::URLRequest>(context->CreateRequest(url, net::DEFAULT_PRIORITY, this, NO_TRAFFIC_ANNOTATION_YET));
  url_request_->set_method("GET");
  url_request_->Start();

  // Sync call
  run_loop_.Run();

  return response_data_;
}

void DappyCertFetcher::OnResponseStarted(net::URLRequest* request, int net_error) {
  if (net_error != net::OK) {
    run_loop_.Quit();
    return;
  }

  int bytes_read = 0;

  do {
    bytes_read = request->Read(read_buffer_.get(), read_buffer_size_);
    response_data_.append(read_buffer_->data(), bytes_read);
  } while (bytes_read > 0); 

  OnReadCompleted(request, bytes_read);
}

void DappyCertFetcher::OnReadCompleted(net::URLRequest* request, int bytes_read) {
  url_request_.reset();
  run_loop_.Quit();
}

}  // namespace dappy
