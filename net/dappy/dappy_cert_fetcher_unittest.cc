#include "dappy_cert_fetcher.h"

#include "content/public/test/browser_task_environment.h"
#include "testing/gtest/include/gtest/gtest.h"

#include "base/logging.h"
#include "base/functional/bind.h"
#include "base/run_loop.h"
#include "base/threading/thread.h"
#include "base/task/single_thread_task_runner.h"
#include "base/test/task_environment.h"
#include "net/cert/x509_certificate.h"
#include "net/url_request/url_request.h"
#include "net/url_request/url_request_context.h"
#include "net/url_request/url_request_context_builder.h"

namespace dappy {

class DappyCertFetcherTest : public testing::Test {
 protected:
  DappyCertFetcherTest() = default;
  ~DappyCertFetcherTest() override = default;

  void SetUp() override {
    net::URLRequestContextBuilder context_builder;
    context_builder.set_user_agent("dappy");
    context_builder.DisableHttpCache();
    context = context_builder.Build();
  }
  base::test::TaskEnvironment task_environment{
    base::test::TaskEnvironment::MainThreadType::IO
  };

  std::unique_ptr<net::URLRequestContext> context;

};

TEST_F(DappyCertFetcherTest, FetchCertificateNonEmpty) {
  // base::test::SingleThreadTaskEnvironment task_environment;
  base::RunLoop run_loop;
  DappyCertFetcher cert_fetcher(std::move(context));
  
  scoped_refptr<net::X509Certificate> cert;

  cert_fetcher.FetchCertificate(GURL("http://localhost:8090/dappy.d"), &cert, run_loop.QuitClosure());

  run_loop.Run();

  std::string pem_encoded;
  net::X509Certificate::GetPEMEncoded(cert->cert_buffer(), &pem_encoded);
  std::cout << "PEM:\n" << pem_encoded << std::endl;
  
  EXPECT_TRUE(cert);
  EXPECT_TRUE(pem_encoded.size() > 0);
}

}  // namespace dappy
