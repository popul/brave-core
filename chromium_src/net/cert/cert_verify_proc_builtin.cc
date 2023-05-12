#include "net/url_request/url_request_context.h"
#include "net/url_request/url_request_context_builder.h"
#include "net/cert/x509_certificate.h"
#include "base/task/thread_pool.h"
#include "base/synchronization/waitable_event.h"
#include "brave/net/dappy/dappy_cert_fetcher.h"
#include "content/public/browser/browser_thread.h"
#include "base/threading/platform_thread.h"
#include "base/task/sequenced_task_runner.h"
#include "base/task/thread_pool.h"
#include "content/public/browser/browser_task_traits.h"

namespace {

std::unique_ptr<net::URLRequestContext> CreateUrlRequestContext() {
  net::URLRequestContextBuilder context_builder;
  context_builder.set_user_agent("dappy");
  context_builder.DisableHttpCache();
  return context_builder.Build();
}

void DappyTask(std::unique_ptr<net::URLRequestContext> context, scoped_refptr<net::X509Certificate>* cert, base::OnceClosure on_done) {
  dappy::DappyCertFetcher cert_fetcher(std::move(context));
  cert_fetcher.FetchCertificate(GURL("http://localhost:8090/dappy.d"), cert, std::move(on_done));
}

void MainTask(scoped_refptr<net::X509Certificate>* cert, base::OnceClosure on_done) {
  std::unique_ptr<net::URLRequestContext> context = CreateUrlRequestContext();
  DappyTask(std::move(context), cert, std::move(on_done));
}

void FetchCertificateTask(scoped_refptr<net::X509Certificate>* cert, base::OnceClosure on_done) {
  base::ThreadPool::CreateSequencedTaskRunner({base::MayBlock()})
    ->PostTask(FROM_HERE, base::BindOnce(&MainTask, cert, std::move(on_done)));
  base::PlatformThread::Sleep(base::Seconds(2));
}

void OnEndFetchCertificateTask(base::WaitableEvent* signal_on_complete) {
  signal_on_complete->Signal();
}

}

#define BRAVE_DAPPY_CERT_VERIFY                                                                       \
  if (hostname.size() > 2 && hostname.compare(hostname.size() - 2, 2, ".d") == 0) {                   \
    verify_result->cert_status = 0;                                                                   \
    return OK;                                                                                        \
    scoped_refptr<net::X509Certificate> cert;                                                         \
    base::WaitableEvent signal_on_complete;                                                           \
    FetchCertificateTask(                                                                             \
      &cert,                                                                                          \
      base::BindOnce(                                                                                 \
        &OnEndFetchCertificateTask,                                                                   \
        base::Unretained(&signal_on_complete)                                                         \
      )                                                                                               \
    );                                                                                                \
    signal_on_complete.Wait();                                                                        \
    input_cert = cert.get();                                                                          \
  }                                                                                     

#include "src/net/cert/cert_verify_proc_builtin.cc"

#undef BRAVE_DAPPY_CERT_VERIFY