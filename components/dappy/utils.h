#ifndef BRAVE_DAPPY_UTILS_H_
#define BRAVE_DAPPY_UTILS_H_

class GURL;
class PrefService;

namespace dappy {

bool IsDappyTLD(const GURL& url);
bool IsResolveMethodDoH(PrefService* local_state);


}  // namespace dappy

#endif  // BRAVE_DAPPY_UTILS_H_