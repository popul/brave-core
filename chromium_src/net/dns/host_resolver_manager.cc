#define BRAVE_DAPPY_DOMAIN_CHECK                        \
  if (GetHostname(job_key.host) == "dappyfake.d") {         \
    HostCache::Entry entry = HostCache::Entry(OK, {     \
      IPEndPoint(IPAddress(134,209,84,129), 0)}, {},    \
      HostCache::Entry::SOURCE_DNS);                    \
    request->set_results(                               \
      entry.CopyWithDefaultPort(                        \
        request->request_host().GetPort()));            \
    request->set_error_info(OK, false);                 \
    return OK;                                          \
  }

#include "src/net/dns/host_resolver_manager.cc"
#undef BRAVE_DAPPY_DOMAIN_CHECK