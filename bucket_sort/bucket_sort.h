//   Copyright 2022 Noam Weiss
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.

#ifndef __BUCKET_SORT__
#define __BUCKET_SORT__

#include <iterator>
#include <map>
#include <vector>

namespace FunAndGames {

template<std::random_access_iterator I,
         std::sentinel_for<I> S,
         class Comp = std::ranges::less,
         class Proj = std::identity,
         class VecAlloc = std::allocator<std::size_t>,
         class MapAlloc = std::allocator<std::pair<
           const typename std::remove_reference<decltype(std::declval<Proj>()(
             std::declval<std::iter_value_t<I>>()))>::type,
           std::vector<std::size_t, VecAlloc>>>>
requires std::sortable<I, Comp, Proj>
void
bucket_sort(I first,
            S last,
            Proj proj = {},
            Comp comp = {},
            VecAlloc vec_alloc = {},
            MapAlloc map_alloc = {})
{
  if (first == last)
    return;

  auto count = std::distance(first, last);

  std::vector<std::size_t, VecAlloc> indexes;
  indexes.reserve(count);
  for (std::size_t i = 0; i < count; ++i) {
    indexes.push_back(i);
  }
  auto inv_indexes = indexes;

  std::map<typename std::remove_reference<decltype(proj(*first))>::type,
           std::vector<std::size_t, VecAlloc>,
           Comp,
           MapAlloc>
    buckets(comp, map_alloc);

  for (auto [iter, index] = std::pair{ first, std::size_t(0) }; iter != last;
       ++iter, ++index) {
    buckets[proj(*iter)].push_back(index);
  }

  std::size_t curr_index = 0;
  for (auto const& bucket : buckets) {
    for (auto const& orig_new_index : bucket.second) {
      auto permutated_new_index = indexes[orig_new_index];
      if (curr_index != permutated_new_index) {
        std::swap(*(first + curr_index), *(first + permutated_new_index));
        std::swap(indexes[inv_indexes[curr_index]], indexes[orig_new_index]);
        std::swap(inv_indexes[curr_index], inv_indexes[permutated_new_index]);
      }
      ++curr_index;
    }
  }
}

} // namespace FunAndGames

#endif // __BUCKET_SORT__
