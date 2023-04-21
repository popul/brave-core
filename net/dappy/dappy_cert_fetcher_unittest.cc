#include "dappy_cert_fetcher.h"

#include "base/test/task_environment.h"
#include "content/public/test/browser_task_environment.h"
#include "testing/gtest/include/gtest/gtest.h"

#include "base/threading/thread.h"
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
  std::string cert_data = cert_fetcher.FetchCertificate(
      GURL("http://localhost:8090/some_certificate.crt"));

  std::cout << "Cert data :\n" << cert_data << std::endl;
  EXPECT_FALSE(cert_data.empty());
}

}  // namespace dappy
