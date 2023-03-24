#include "base/strings/string_util.h"
#include "brave/net/dappy/constants.h"
#include "net/dns/dns_config.h"
#include "net/dns/dns_server_iterator.h"

namespace {

bool GetNextIndex(const std::string& hostname,
                  const net::DnsConfig& config,
                  net::DnsServerIterator* dns_server_iterator,
                  size_t* doh_server_index) {
  // Skip dappy domains resolver for other domains.
  if (config.doh_config.servers()[*doh_server_index].server_template() ==
          dappy::kDoHResolver &&
      !base::EndsWith(hostname, dappy::kDappyDomain)) {
    // No next available index to attempt.
    if (!dns_server_iterator->AttemptAvailable()) {
      return false;
    }

    *doh_server_index = dns_server_iterator->GetNextAttemptIndex();
  }

  return true;
}

}  // namespace

#define BRAVE_MAKE_HTTP_ATTEMPT                                       \
  if (!GetNextIndex(hostname_, session_->config(),              \
                    dns_server_iterator_.get(), &doh_server_index)) { \
    return AttemptResult(ERR_BLOCKED_BY_CLIENT, nullptr);             \
  }

#include "src/net/dns/dns_transaction.cc"

#undef BRAVE_MAKE_HTTP_ATTEMPT