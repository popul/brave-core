#define BRAVE_DAPPY_CERT_VERIFY                                                       \
  if (hostname.size() > 2 && hostname.compare(hostname.size() - 2, 2, ".d") == 0) {   \
    verify_result->cert_status = 0;                                                   \
    return OK;                                                                        \
  }

#include "src/net/cert/cert_verify_proc_builtin.cc"

#undef BRAVE_DAPPY_CERT_VERIFY