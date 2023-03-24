#include "brave/components/dappy/utils.h"

#include "base/strings/string_util.h"
#include "brave/net/dappy/constants.h"
#include "components/prefs/pref_service.h"
#include "url/gurl.h"

namespace dappy {

bool IsDappyTLD(const GURL& url) {
  return base::EndsWith(url.host_piece(), kDappyDomain);
}

bool IsResolveMethodDoH(PrefService* local_state) {
  return local_state != nullptr;
}


}  // namespace dappy
