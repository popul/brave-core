#include "dappy_cert_fetcher.h"

#include "net/base/io_buffer.h"
#include "net/url_request/url_request_context.h"
#include "net/traffic_annotation/network_traffic_annotation.h"
#include "net/cert/x509_certificate.h"
#include "base/task/single_thread_task_runner.h"
#include "base/task/thread_pool.h"
#include "base/threading/platform_thread.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/browser_task_traits.h"

namespace dappy {

DappyCertFetcher::DappyCertFetcher(
    std::unique_ptr<net::URLRequestContext> context)
    : context(std::move(context)) {
      read_buffer_ = base::MakeRefCounted<net::IOBuffer>(read_buffer_size_);
      // owning_sequence_ = base::SequencedTaskRunner::GetCurrentDefault();
    }


void DappyCertFetcher::FetchCertificate(const GURL& url, scoped_refptr<net::X509Certificate>* cert, base::OnceClosure on_done) {
  on_done_ = std::move(on_done);
  response_cert = cert;

  url_request_ = std::unique_ptr<net::URLRequest>(context->CreateRequest(url, net::DEFAULT_PRIORITY, this, NO_TRAFFIC_ANNOTATION_YET));
  url_request_->set_method("GET");

  url_request_->Start();
  base::PlatformThread::Sleep(base::Seconds(1));
}

DappyCertFetcher::~DappyCertFetcher() {
  if (url_request_) {
    url_request_.reset();
  }
}

void DappyCertFetcher::OnResponseStarted(net::URLRequest* request, int net_error) {
  if (net_error != net::OK) {
    url_request_.reset();
    std::move(on_done_).Run();
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
  base::span<const uint8_t> cert_bytes(reinterpret_cast<const uint8_t*>(response_data_.data()),
                                       response_data_.size());

  *response_cert = net::X509Certificate::CreateFromBytes(cert_bytes);

  DCHECK(*response_cert);

  url_request_.reset();
  std::move(on_done_).Run();
}

}  // namespace dappy
