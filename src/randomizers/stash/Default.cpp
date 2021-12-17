#include "ZHM5Randomizer/src/randomizers/stash/Default.h"

#include "ZHM5Randomizer/src/Console.h"
#include "ZHM5Randomizer/src/Item.h"
#include "ZHM5Randomizer/src/RepositoryID.h"

namespace hitman_randomizer {

const RepositoryID *
DefaultStashRandomization::randomize(const RepositoryID *in_out_ID) {
  if (!repo_->contains(*in_out_ID)) {
    log::info(
        "DefaultStashRandomization::randomize: skipped (not in repo) [{}]",
        in_out_ID->toString().c_str());
    return in_out_ID;
  }

  auto in_item = repo_->getItem(*in_out_ID);

  auto sameType = [&in_item](const Item &item) {
    return in_item->getType() == item.getType() && 
      (item.isItemAcceptableDefaultItem() || item.isWeapon());
  };

  auto randomized_item = repo_->getRandom(sameType);
  log::info("DefaultStashRandomization::randomize: {} -> {}",
               repo_->getItem(*in_out_ID)->string().c_str(),
               repo_->getItem(*randomized_item)->string().c_str());

  return randomized_item;
};

} // namespace hitman_randomizer