#include "dappy_cert_fetcher.h"

#include "base/test/task_environment.h"
#include "content/public/test/browser_task_environment.h"
#include "testing/gtest/include/gtest/gtest.h"

#include "base/threading/thread.h"
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
    context_builder.set_user_agent("test");
    context_builder.DisableHttpCache();
    context = context_builder.Build();
  }

  content::BrowserTaskEnvironment task_environment_{
      base::test::TaskEnvironment::MainThreadType::IO
  };
  std::unique_ptr<net::URLRequestContext> context;
};

TEST_F(DappyCertFetcherTest, FetchCertificateNonEmpty) {
  DappyCertFetcher cert_fetcher(std::move(context));
  scoped_refptr<net::X509Certificate> cert = cert_fetcher.FetchCertificate(
      GURL("http://localhost:8090/certificate.der"));

  std::string pem_encoded;
  net::X509Certificate::GetPEMEncoded(cert->cert_buffer(), &pem_encoded);
  std::cout << "PEM:\n" << pem_encoded << std::endl;
  
  EXPECT_TRUE(cert);
  EXPECT_TRUE(pem_encoded.size() > 0);
}

}  // namespace dappy
